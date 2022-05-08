// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../vmath/zmath.h"

#include "glheaders.h"
#include "../memory/zmem.h"
#include "../structures/zlist.h"
#include "../structures/zvector.h"


#include "gx_sys.h"
#include <math.h>
#include <float.h>

#include "gx_image.h"

#include "gx_buffers.h"
#include "gx_drawstyle.h"
#include "gx_mesh.h"

#include <stdio.h>

//#define DOPRINTFS

zbool mesh_free(void* x)
{
	gx_mesh_t* m = x;
	gx_mesh_t* t;
	
	printf(" ENTER MESHFREE\n");
	ram_free(m->style);
	ram_free(m->data);
	
	m = m->next;  //get next element

	while(m) {
		t = m->next;  //look ahead one
		m->next = NULL;
		ram_free(m);
		m=t;
	}

	printf(" EXIT  MESHFREE\n");
	return ZTRUE; //free this first one
}

//define a mesh from a subset of a vbuffer.  The mesh has a particualr drawstyle applied to it.
gx_mesh_t*  gx_mesh_def(gx_vbuffer_t* v, gx_drawstyle_t* s, zuint32 drawstart, zuint32 drawend, zbool indexed)
{
	gx_mesh_t* m = NULL;

	m = ram_alloc(sizeof(gx_mesh_t), mesh_free);  
	
	if (m)
	{

		m->data = ram_addref(v);
		m->style = ram_addref(s);
		m->drawstart = drawstart;
		m->drawend = drawend;
		m->indexed = indexed;
		m->prim = gx_triangles;  //default triangles unless override
	}
	return m;
}

//draws a mesh
void gx_mesh_draw(gx_mesh_t* mesh_in)
{
	gx_mesh_t* mesh = mesh_in;
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(mesh)
	{
		
//		if (mesh->name)
//			printf(" Draw mesh %s\n", mesh->name);


		if (mesh->style)
			gx_drawstyle_activate(mesh->style);

		gx_vbuffer_draw(mesh->data, mesh->drawstart, mesh->drawend, mesh->prim, mesh->indexed);
		mesh = mesh->next;
		
		if (mesh == mesh_in)  
		{
#ifdef DOPRINTFS 
			printf(" Warning: breaking mesh cycle\n");
#endif
			//cycle detected!
			break;
		}
	}

}



//simple mesh loader (obj files)






//need a little vertex data

//store an obj file's vertex/texcoord/normal combination
typedef struct v_t_n_combo_s
{
	//linked list of combinations
	struct v_t_n_combo_s* next;
	//keep track of a OBJ file combination:	
	int v;
	int vt;
	int vn;

	//keep track of where this combination is in the vbuffer:
	gx_vbuffer_t*	vbuffer;
	int				vbuffer_vertex;

} v_t_n_combo_t;

//store 3-space number (positions or normals)
typedef struct coord3_s
{
	float x;
	float y;
	float z;
	v_t_n_combo_t * combos;  //used for positions to keep track of combinations used
} coord3_t;

//cleans up a coord3_t; pass into the allocation for a coord3_t
zbool _coord3_s_cleanup(void* x)
{
	coord3_t * s = x;
	if (s)
	{
		v_t_n_combo_t* combo = s->combos;
		v_t_n_combo_t* combo_next = NULL;
		while(combo)
		{
			combo_next = combo->next;
			ram_free(combo);
			combo = combo_next;
			
		}
	}
	return ZTRUE;
}

//store 2-space number (texcoords)
typedef struct coord2_s
{
	float s;
	float t;
} coord2_t;


//loads an OBJ file as a mesh
//todo: incomplete: it just parses the obj file and throws it away :(
//vbuf is the perferred vbuffer to put vertices/indices into
//behavior:

#define MESH_VERTEX_COUNT 65535
#define MESH_INDEX_COUNT  65535

//#define MESH_VERTEX_COUNT 1000
//#define MESH_INDEX_COUNT  1000



//face point limit: 3 - triangle 4-quad 6-hexagon etc
#define FACE_POINT_LIMIT 8
gx_mesh_t* gx_mesh_load_obj(gx_vbuffer_t* vbuf,  zchar* filename, zvec_t* drawstyles)
{
	gx_mesh_t* m = NULL;
	FILE* f = fopen(filename, "rb");
	int triangles=0;
	char matname[100];
	char buffer[100];
	char *objname;
	int delim;
	float x,y,z;
	float s,t;
	
	zvec_t normals;
	zvec_t vertices;
	zvec_t texcoords;

	gx_drawstyle_t* ds;
	int i;

	gx_mesh_t* mesh_start = NULL;

	gx_mesh_t* current_mesh = NULL;
	gx_mesh_t* new_mesh = NULL;



	v_t_n_combo_t* tmp_combo = NULL;

	v_t_n_combo_t* face_point[FACE_POINT_LIMIT];
	int face_point_count=0;
	
	gx_vbuffer_t* current_vbuffer = NULL;

	if (!f)
		return NULL;

	

	current_vbuffer = vbuf;  //start with the default 'preferred' vbuffer
	
	if (current_vbuffer)
	{
		//this part not tested (adding a mesh to an existing vbuffer)
		//TODO: TEST THIS
		
		current_mesh = ram_alloc(sizeof(gx_mesh_t), NULL);
		current_mesh->drawstart = current_vbuffer->index_count;
		current_mesh->drawend = current_mesh->drawstart;
		current_mesh->indexed = ZTRUE;
		current_mesh->data = ram_addref(current_vbuffer);  //keep vbuffer alive as long as this mesh exists
		current_mesh->prim = gx_triangles;

		mesh_start = current_mesh;
	}


	//store the vertices, normals and texcoords of the OBJ file  
	zvec_mk(&vertices, 100);
	zvec_mk(&normals, 100);
	zvec_mk(&texcoords, 100);
	
	

	while (1)
	{
		coord3_t* v3 = NULL;
		coord2_t* v2 = NULL;

		delim = gxi_read_to_delim(f, buffer, sizeof(buffer),  " \n\r\t" );
		if (delim < 0) //negative number is read error or end of file
			break;


		//material directive
		if ((!strcmp(buffer, "usemtl")) && drawstyles) {
			fscanf(f, " %99s", matname);
			for (i=0;i<zvec_count(drawstyles);i++) {
				ds = zvec_get_at(drawstyles, i);
				if (ds && ds->name && !strcmp(ds->name, matname)) {
					
					break;
				}
				ds = NULL;
			}	

			if (ds && ds->name){
				printf(" Found material [%s]\n",ds->name);
				if (current_mesh && current_mesh->drawstart == current_mesh->drawend) {
					current_mesh->style = ram_addref(ds);
					printf(" Set on nopoly mesh\n");
					//ds = NULL;
				}
			}
		}

		//object directive
		if (!strcmp(buffer, "o")) {
			fscanf(f, " %99s", buffer);

			objname = ram_strdup(buffer);

			printf(" Set object name [%s]\n",objname);
			if (current_mesh && current_mesh->drawstart == current_mesh->drawend) {
				current_mesh->name = objname;
				objname = NULL;
				printf(" Set name on nopoly mesh\n");
			}
		}


				
		//vertex
		if (!strcmp(buffer, "v"))
		{	
			if (v3 = zvec_add_or_free( &vertices, ram_alloc(sizeof(*v3), _coord3_s_cleanup) )   )
			{
				fscanf(f,"%f %f %f", &v3->x, &v3->y, &v3->z);

				//remove this scaling later!
			//	v3->x *=.05;
			//	v3->y *=.05;
			//	v3->z *=.05;
#ifdef DOPRINTFS
				printf("v(%d %f %f %f)", vertices.count, v3->x, v3->y, v3->z);
#endif
				
			}	
		}
		//texcoord
		else if (!strcmp(buffer, "vt"))
		{
			
		
			if (v2 = zvec_add_or_free( &texcoords, ram_alloc(sizeof(*v2), NULL) )   )
			{
				fscanf(f,"%f %f", &v2->s, &v2->t);
			}
			
#ifdef DOPRINTFS
				printf("vt(%d)", texcoords.count);
#endif

		}
		//surface normal
		else if (!strcmp(buffer, "vn"))
		{
			
			if (v3 = zvec_add_or_free( &normals, ram_alloc(sizeof(*v3), NULL) )   )
			{
				fscanf(f,"%f %f %f", &v3->x, &v3->y, &v3->z);
			
			}	

#ifdef DOPRINTFS
			printf("vn(%d)", normals.count);
#endif

		}
		//group command
		else if (!strcmp(buffer, "g"))
		{
			int igs;
			//the only support we have for groups is to exclude certain groups from the mesh
#if 0
			fscanf("%99s", buffer);

			if (ignoregroups)
			{

				while(ignoregroups[igs])
				{
					if (!strcmp(ignoregroups[igs], buffer))
					{	
						//we found something on the ignoregroup list

					}

				}
			
			}
#endif

		}
		//face (triangle or quad)
		else if (!strcmp(buffer, "f"))
		{
			int i=0;

			face_point_count = 0;
		
			delim = '/';
#ifdef DOPRINTFS
			printf("f ");
#endif

			//read we are reading until end of the line or we have 4 points
			while ((delim != '\r')&& (delim !='\n')  )
			{
				delim = '/';

				delim = gxi_read_to_delim(f, buffer, sizeof(buffer), " /\r\n\t");
				
				if (delim < 0) //error reading
					break;

				if (strlen(buffer)==0)
					continue;  //if we get empty vertex, forget about it (means we got multiple whitespace between vertices, probably)

				//If we don't have a temp combo, create one, otherwise clear what we do have
				if (!tmp_combo)
					tmp_combo = ram_alloc(sizeof(*tmp_combo), NULL);
				else
					ram_clear(tmp_combo, sizeof(*tmp_combo));
				
				//get vertex number from buffer
				tmp_combo->v = atoi(buffer);


				//texcoord number
				if (delim=='/')
				{
					delim = gxi_read_to_delim(f, buffer, sizeof(buffer), " /\r\n\t");

					tmp_combo->vt = atoi(buffer);
				}
				
				// normal number
				if (delim=='/')
				{
					delim = gxi_read_to_delim(f, buffer, sizeof(buffer), " /\r\n\t");
					tmp_combo->vn = atoi(buffer);
				}
				
				#ifdef DOPRINTFS
					printf("<%d/%d/%d>", tmp_combo->v, tmp_combo->vt, tmp_combo->vn);
				#endif

				//check this vertex for this particular combination;
				{
					v_t_n_combo_t* search_combo = NULL;
					zbool found = ZFALSE;
					coord3_t* vv;

					if (tmp_combo->v < 1)
					{
						printf(" invalid v number\n");

					}
					
					vv = zvec_get_at( &vertices, tmp_combo->v - 1 ); //-1 because OBJ files are 1-indexed

					if (vv)
						search_combo = vv->combos;

					while(search_combo)
					{
					
						if (
							  (search_combo->v == tmp_combo->v)
							&&(search_combo->vt == tmp_combo->vt)
							&&(search_combo->vn == tmp_combo->vn))
						{
							found = ZTRUE;
//							printf(" Repeat combination %d/%d/%d\n", search_combo->v,search_combo->vt,search_combo->vn);

							if (face_point_count >=FACE_POINT_LIMIT)
							{
								printf(".TOO MANY POINTS IN ONE FACE %d\n", face_point_count);
								break;
							}

							face_point[face_point_count++] = search_combo;  //we found an existing point for our face
							break;
						}

						search_combo = search_combo->next;
					
					}

					//otherwise we have a unique combination of v/vt/normal
					if (!found)
					{
//						printf(" New combination %d/%d/%d\n", tmp_combo->v,tmp_combo->vt,tmp_combo->vn);

						//add the new combo to this point's list of combos

						tmp_combo->next = vv->combos;
						vv->combos = tmp_combo;

						if (face_point_count >=FACE_POINT_LIMIT)
						{
							printf("/TOO MANY POINTS IN ONE FACE %d\n", face_point_count);
							tmp_combo = NULL;  //need to throw this away (the vertex owns the combo now(
							break;
						}

						face_point[face_point_count++] = tmp_combo;  //keep track of our face's combos

						tmp_combo = NULL;  //give up our pointer to it
					}

				


				}

			//	printf( " : ");

			}

			//put face
			{
				int new_points = 0;

				int j;
				
				//go through this face's points and see how many need to be added to the vbuffer
				for (j=0;j<face_point_count;j++)
				{
					if (face_point[j]->vbuffer != current_vbuffer)
					{
						new_points++;
						
					}
					//else 
					//	printf(" recycling point!\n");
				}

					

				if (    (gx_remaining_vertices(current_vbuffer) < new_points) 
					 || (gx_remaining_indices(current_vbuffer) < (3*(face_point_count-2)) ))
				{  //won't fit in vbuffer, so create a new one
					current_vbuffer = gx_vbuffer_mk(MESH_VERTEX_COUNT, MESH_INDEX_COUNT, GX_VBUFFER_TEXCOORD |GX_VBUFFER_NORMAL );

					//now that we are creating a new vbuffer, we also need to create a new mesh segment

	
					new_mesh = ram_alloc(sizeof(gx_mesh_t), mesh_free);
					new_mesh->style = ram_addref(ds);
					new_mesh->name = objname;
					printf(" set style to %s\n", ds->name);
					new_mesh->drawstart = 0;
					new_mesh->indexed = ZTRUE;
					new_mesh->data = current_vbuffer; //we are giving the only refernce of this new vbuffer to this mesh
					new_mesh->prim = gx_triangles;

					//new_mesh->next = current_mesh;
					//current_mesh = new_mesh;
					//new_mesh = NULL;
					if (!current_mesh)
					{
						mesh_start = new_mesh;
					}
					else
					{
						current_mesh->next = new_mesh;
					}
					current_mesh = new_mesh;
					new_mesh=NULL;

				}
			       
#if 1
				else if (
						(current_mesh->drawend != current_mesh->drawstart)

						&&  

						(
						  (ds && (ds != current_mesh->style)) 
						    ||
						  (objname && (objname != current_mesh->name)) 
						)	
					) {
					//if we fit, but changed style or name
					//we do the same, but with the existing vbuffer
					new_mesh = ram_alloc(sizeof(gx_mesh_t), mesh_free);
					new_mesh->name = objname;

					if (ds)
						new_mesh->style = ram_addref(ds);
					new_mesh->drawstart = current_mesh->drawend;  //START WHERE OTHER LEFT OFF
					new_mesh->drawend = current_mesh->drawend;  //START WHERE OTHER LEFT OFF
					new_mesh->indexed = ZTRUE;
					new_mesh->data = ram_addref(current_vbuffer);//adding new reference to vbuffer
					new_mesh->prim = gx_triangles;
									
					current_mesh->next = new_mesh;
					
					current_mesh = new_mesh;
					new_mesh=NULL;

				}
#endif

				//now lets make sure all points are in the current buffer
				for (j=0;j<face_point_count;j++)
				{

					if (face_point[j]->vbuffer != current_vbuffer)
					{

						

						coord3_t* vs = zvec_get_at(&vertices,  (face_point[j]->v -1));
						coord2_t* vt = zvec_get_at(&texcoords, (face_point[j]->vt -1) );
						coord3_t* vn = zvec_get_at(&normals,   (face_point[j]->vn -1));

						if (face_point[j]->vbuffer)
						{
//							printf("need to dupe a point into a new buffer\n");
						}


						face_point[j]->vbuffer = current_vbuffer;
						
						if (vt)
						{
							gx_vbuffer_tex2( current_vbuffer, 0, vt->s, vt->t);
							
						}

						if (vn)
						{
							gx_vbuffer_normal3( current_vbuffer, vn->x, vn->y, vn->z);
						}
						
						face_point[j]->vbuffer_vertex = gx_vbuffer_vertex3(current_vbuffer, vs->x, vs->y, vs->z);					


						
					}


				}

#if 0
				//triangle/quad only case

				//now put in all the indices
				gx_vbuffer_add_index(current_vbuffer, face_point[0]->vbuffer_vertex);
				gx_vbuffer_add_index(current_vbuffer, face_point[1]->vbuffer_vertex);
				gx_vbuffer_add_index(current_vbuffer, face_point[2]->vbuffer_vertex);
					current_mesh->drawend +=3;

				if (face_point_count >= 4)
				{

					gx_vbuffer_add_index(current_vbuffer, face_point[0]->vbuffer_vertex);
					gx_vbuffer_add_index(current_vbuffer, face_point[2]->vbuffer_vertex);
					gx_vbuffer_add_index(current_vbuffer, face_point[3]->vbuffer_vertex);
					current_mesh->drawend +=3;
				}
#endif

				//general case

			
 
				for (j=2;j<face_point_count;j++)
				{
					gx_vbuffer_index(current_vbuffer, face_point[0]->vbuffer_vertex);
					gx_vbuffer_index(current_vbuffer, face_point[j-1]->vbuffer_vertex);
					gx_vbuffer_index(current_vbuffer, face_point[j]->vbuffer_vertex);
					current_mesh->drawend +=3;
					triangles++;
				}


				
				
			
			}

//			printf(" endface\n");




			//face (triangle/quad)
		} 
		
		//read until end of line or file
		while (  (delim!='\n') && (delim!='\r') && (delim>0))
		{
			delim = gxi_read_to_delim(f, buffer, sizeof(buffer),  " \n\r\t" );
		}
		
			
	}

	#ifdef DOPRINTFS
				printf("done\n");
#endif
	//free left over tmp_combo
	if (tmp_combo)
		ram_free(tmp_combo);



	printf(" %d vertices, %d normals, %d texcoords, %d triangles\n",
		zvec_count(&vertices),	
		zvec_count(&normals),	
		zvec_count(&texcoords),
		triangles);	

	//delete all the intermin objects created and stored in these vectors:
	zvec_cleanup(&vertices);
	zvec_cleanup(&normals);
	zvec_cleanup(&texcoords);

	return mesh_start;
}

