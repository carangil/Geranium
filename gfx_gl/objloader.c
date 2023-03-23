// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define GFXINTERNAL
#include "gfx_gl.h"
#include "zarray.h"


#include <stdio.h>


/*
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

*/

//simple mesh loader (obj files)

/*load obj file*/
/*OLD code*/

//try a simple mesh loader (obj files)


//reads from input until delimiter in 'delims' in  reached
//return value:  0  reached terminator
//               -1 buffer is full
//				 -2 end of file
//				 >0 the delimiter character
static zint32 read_to_delim(FILE* f, zchar* buffer, zuint32 buffer_len, zchar* delims)
{
	zuint32 i = 0;
	int c;
	int j;
	int retval = -1;
	int br = 0;

	if (!f)
		return -1;


	while (i < buffer_len)
	{
		c = fgetc(f);
		if (feof(f) || c < 0)
		{
			retval = -2;
			break;
		}

		if (c == 0)
		{
			retval = 0;
			break;
		}

		for (j = 0; delims[j]; j++)
		{
			if (c == delims[j])
			{
				retval = delims[j];
				br = 1;
				break;
			}
		}
		if (br)
			break;
		buffer[i++] = c;
	}

	buffer[i] = '\0';

	return retval;
}


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

	//track if already in the vbuffer
	gfx_vertex_bufferT* vb; //vertex buffer
	zuint32 p;	//point number

} v_t_n_combo_t;

//store 3-space number (positions or normals)
typedef struct coord3_s
{
	float x;
	float y;
	float z;
	v_t_n_combo_t* combos;  //used for positions to keep track of combinations used
} coord3_t;



//cleans up a coord3_t; pass into the allocation for a coord3_t
zbool _coord3_s_cleanup(void* x)
{
	coord3_t* s = x;
	if (s)
	{
		v_t_n_combo_t* combo = s->combos;
		v_t_n_combo_t* combo_next = NULL;
		while (combo)
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


//face point limit: 3 - triangle 4-quad 6-hexagon etc
#define FACE_POINT_LIMIT 8

typedef struct face_s
{
	v_t_n_combo_t* point[FACE_POINT_LIMIT];
} poly_t;

//#define DOPRINTFS

//loads an OBJ file as a mesh
//todo: incomplete: it just parses the obj file and throws it away :(
//vbuf is the perferred vbuffer to put vertices/indices into
//behavior:

#define MESH_VERTEX_COUNT 65535
#define MESH_INDEX_COUNT  4*64436


gfx_meshT* gfx_mesh_load_obj(zchar* filename, float scale)
{
	//	gx_mesh_t* m = NULL;
	FILE* f = fopen(filename, "rb");

	char buffer[100];
	int delim;


	zvecT vertices;
	zvecT normals;
	zvecT texcoords;
	zvecT polys;
	zuint32 tricount = 0;

	v_t_n_combo_t* tmp_combo = NULL;
	zuint32 unique_combos = 0;
	int face_point_count = 0;

	if (!f) {
		printf(" Can't open %s\n", filename);
		return NULL;
	}


	
	//obj file data
	//TODO:  these vectors are pointer arrays. allocating an object per vertex, texcoord, etc is a bit wasteful
	//this can be refactore into using the newer 'zarrays', which are resizable arrsys, and using array indices instead of pointers
	//but for now, keeping this is OK.. this worked on much older computers just fine, and is 'transient' memory usage
	zvec_mk(&vertices, 100);
	zvec_mk(&normals, 100);
	zvec_mk(&texcoords, 100);
	zvec_mk(&polys, 100);


	while (1)
	{
		coord3_t* v3 = NULL;
		coord2_t* v2 = NULL;

		delim = read_to_delim(f, buffer, sizeof(buffer), " \n\r\t");
		if (delim < 0) //negative number is read error or end of file
			break;

		//vertex
		if (!strcmp(buffer, "v"))
		{
			if (v3 = zvec_add_or_free(&vertices, ram_alloc(sizeof(*v3), _coord3_s_cleanup)))
			{
				fscanf(f, "%f %f %f", &v3->x, &v3->y, &v3->z);

		
				v3->x *=scale;
				v3->y *=scale;
				v3->z *=scale;
#ifdef DOPRINTFS
				printf("v(%d)", vertices.count);
#endif

			}
		}//end 'v'
		//texcoord
		else if (!strcmp(buffer, "vt"))
		{


			if (v2 = zvec_add_or_free(&texcoords, ram_alloc(sizeof(*v2), NULL)))
			{
				fscanf(f, "%f %f", &v2->s, &v2->t);
			}

#ifdef DOPRINTFS
			printf("vt(%d)", texcoords.count);
#endif

		}//end vt
		//surface normal
		else if (!strcmp(buffer, "vn"))
		{

			if (v3 = zvec_add_or_free(&normals, ram_alloc(sizeof(*v3), NULL)))
			{
				fscanf(f, "%f %f %f", &v3->x, &v3->y, &v3->z);

			}

#ifdef DOPRINTFS
			printf("v(%d)", normals.count);
#endif

		} //end vt
		//group command
		else if (!strcmp(buffer, "g"))
		{
			int igs;
			//the only support we have for groups is to exclude certain groups from the mesh
#if 0
			fscanf("%99s", buffer);

			if (ignoregroups)
			{

				while (ignoregroups[igs])
				{
					if (!strcmp(ignoregroups[igs], buffer))
					{
						//we found something on the ignoregroup list

					}

				}

			}
#endif

		} //end g
		//face (triangle or quad)
		else if (!strcmp(buffer, "f"))
		{
			poly_t* poly = ram_alloc(sizeof(poly_t), NULL);

			int i = 0;

			face_point_count = 0;

			delim = '/';
#ifdef DOPRINTFS
			printf("f ");
#endif

			
			//read we are reading until end of the line 
			while ((delim != '\r') && (delim != '\n'))
			{
				delim = '/';

				delim = read_to_delim(f, buffer, sizeof(buffer), " /\r\n\t");

				if (delim < 0) //error reading
					break;

				if (strlen(buffer) == 0)
					continue;  //if we get empty vertex, forget about it (means we got multiple whitespace between vertices, probably)

				//If we don't have a temp combo, create one, otherwise clear what we do have
				if (!tmp_combo)
					tmp_combo = ram_alloc(sizeof(*tmp_combo), NULL);
				else
					ram_clear(tmp_combo, sizeof(*tmp_combo));

				//get vertex number from buffer
				tmp_combo->v = atoi(buffer);


				//texcoord number
				if (delim == '/')
				{
					delim = read_to_delim(f, buffer, sizeof(buffer), " /\r\n\t");

					tmp_combo->vt = atoi(buffer);
				}

				// normal number
				if (delim == '/')
				{
					delim = read_to_delim(f, buffer, sizeof(buffer), " /\r\n\t");
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

					vv = zvec_get_at(&vertices, tmp_combo->v - 1); //-1 because OBJ files are 1-indexed

					if (vv)
						search_combo = vv->combos;

					while (search_combo)
					{

						if (
							(search_combo->v == tmp_combo->v)
							&& (search_combo->vt == tmp_combo->vt)
							&& (search_combo->vn == tmp_combo->vn))
						{
							found = ZTRUE;
							//							printf(" Repeat combination %d/%d/%d\n", search_combo->v,search_combo->vt,search_combo->vn);

							if (face_point_count >= FACE_POINT_LIMIT)
							{
								printf(".TOO MANY POINTS IN ONE FACE %d\n", face_point_count);
								break;
							}

							poly->point[face_point_count++] = search_combo;  //we found an existing point for our face
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
						unique_combos++;
						if (face_point_count >= FACE_POINT_LIMIT)
						{
							printf("/TOO MANY POINTS IN ONE FACE %d\n", face_point_count);
							tmp_combo = NULL;  //need to throw this away (the vertex owns the combo now(
							break;
						}

						poly->point[face_point_count++] = tmp_combo;  //keep track of our face's combos

						tmp_combo = NULL;  //give up our pointer to it
					}




				}

				
				
			}


			//read until end of line or file
			while ((delim != '\n') && (delim != '\r') && (delim > 0))
			{
				delim = read_to_delim(f, buffer, sizeof(buffer), " \n\r\t");
			}

			
			
			tricount += face_point_count - 2;  //triangle: 1,  quad: 2 triangles
			zvec_add_or_free(&polys, poly);
		}//end f

		

	} //end loop

	printf(" PARSED %d vertices  %d normals  %d texcoords   %d polygons   (%d unique vertices %d triangles) \n",
		zvec_count(&vertices), zvec_count(&normals), zvec_count(&texcoords), zvec_count(&polys), unique_combos, tricount);


#ifdef DOPRINTFS
		printf("done\n");
#endif
		//free left over tmp_combo
		if (tmp_combo)
			ram_free(tmp_combo);


		/* Make mesh from the data */
		gfx_meshT* start_mesh = NULL;
		gfx_meshT* mesh = start_mesh = ram_alloc(sizeof(gfx_meshT), gfx_free_mesh);
		
		zuint32 va = unique_combos;
		if (va > MESH_VERTEX_COUNT)
			va = MESH_VERTEX_COUNT;

		zuint32 vi = tricount*3+3;
		if (vi > MESH_INDEX_COUNT)
			vi = MESH_INDEX_COUNT;

		mesh->vb = gfx_vertex_buffer_mk(va, "position:3|normal:3|texcoord:2"); //this sets position as 0, normal as 1, and texcoord as 2
		gfx_vertex_buffer_add_index(mesh->vb, vi);

		zuint32 i;
		zuint32 iu=0;  //index used
		zuint32 vu = 0; //vertex used

		for (i = 0; i < zvec_count(&polys); i++) {

 			poly_t* poly = zvec_get_at(&polys, i);

			zuint32 new_points = 0;
			zuint32 new_index = 0;

			int j;
			for (j = 0; poly->point[j] && ( j<FACE_POINT_LIMIT) ; j++) {
				if (poly->point[j]->vb != mesh->vb)
					new_points++;
	
				if (j >= 2)
					new_index+=3;  //triangle will be 1 index, quads 2, etc

			}

			if (j >= FACE_POINT_LIMIT) {

				printf(" too many points\n");
				continue;
			}


			//todo: fix limits

			if ((vu + new_points >= va) || (iu + new_index >= vi)) {


				mesh->next_piece = ram_alloc(sizeof(gfx_meshT), gfx_free_mesh);

				mesh = mesh->next_piece;

				vu = 0;
				iu = 0;
			//	va = MESH_VERTEX_COUNT;
				//vi = MESH_INDEX_COUNT;

				mesh->vb = gfx_vertex_buffer_mk(va, "position:3|normal:3|texcoord:2"); //this sets position as 0, normal as 1, and texcoord as 2
				gfx_vertex_buffer_add_index(mesh->vb, vi);


			}
			

			for (j = 0; poly->point[j] && (j < FACE_POINT_LIMIT); j++) {

				if (poly->point[j]->vb != mesh->vb) {
					//need to add the point;
					poly->point[j]->vb = mesh->vb;  //use this buffer 
					
					coord3_t* vp = zvec_get_at(&vertices, poly->point[j]->v-1);
					coord3_t* vnp = zvec_get_at(&normals, poly->point[j]->vn-1);
					coord2_t* vtp = zvec_get_at(&texcoords, poly->point[j]->vt-1);


					if (vnp)
						gfx_vertex_data(mesh->vb, 1, vnp->x, vnp->y, vnp->z, 0.0f); //normal

					if (vtp)
						gfx_vertex_data(mesh->vb, 2, vtp->s, vtp->t, 0.0f, 0.0f);

					vu++;
					poly->point[j]->p= gfx_vertex_done(mesh->vb, 0, vp->x, vp->y, vp->z, 0.0f); //position
					//printf(" Made new point %d for vtn %d %d %d\n", poly->point[j]->p, poly->point[j]->v, poly->point[j]->vt, poly->point[j]->vn);

				}
				else {
				//	printf(" Reuse point %d for vtn %d %d %d\n", poly->point[j]->p, poly->point[j]->v, poly->point[j]->vt, poly->point[j]->vn);
				}

				if (j >= 2) {
					iu+=3;
					gfx_index_triangle(mesh->vb, poly->point[0]->p, poly->point[j-1]->p, poly->point[j]->p);
				}
				

			}
			


		}

		/*mesh is finished */

		//delete all the intermin objects created and stored in these vectors:
		zvec_cleanup(&vertices);
		zvec_cleanup(&normals);
		zvec_cleanup(&texcoords);
		zvec_cleanup(&polys);

		return start_mesh;
		//end function//end loop



	}


/*end obj file*/

void lcase(char* s) {

	for (; *s; s++)
		if (((*s) >= 'A') && ((*s) <= 'Z'))
			*s = *s + 'a' - 'A';
		
}

int next(FILE* f, char* buf) {
	int e = fscanf(f, "%99s", buf);

	lcase(buf);

	 if (e == EOF)
		return 0;

	return 1;

	
}
#define NEXT next(f, buf)
#define EQ(AA)   (!strcmp(buf, AA))

#define XROT 0
#define YROT 1
#define ZROT 2
#define XPOS 3
#define YPOS 4
#define ZPOS 5

#define Z_Y_SWAP 


gfx_jointT* tryparsejoint(FILE* f, char* buf, float scale) {

	gfx_jointT* child = NULL;


	if (EQ("joint") || EQ("root") ||EQ("end" )) {
		printf(" parsing joint\n");

		NEXT;	//name (or "site" in end site
		printf("name is %s\n", buf);

		gfx_jointT* joint = ram_alloc(sizeof(gfx_jointT), NULL);
		
		joint->name = ram_strdup(buf);

		NEXT;
		if (!EQ("{")) {
			printf(" expected{");
			
			exit(1);
		}
				
		while (NEXT) {

			if (EQ("}"))
				return joint; //bubble up

			child = tryparsejoint(f, buf, scale); //see if there's a joint inside
			if (child) {
				zvec_add_or_free(&joint->children, child);
				child = NULL;
			}

			if (EQ("offset")) {
				
			#ifdef Z_Y_SWAP

				fscanf(f, "%f %f %f", &joint->offset.VX, &joint->offset.VZ, &joint->offset.VY);
				joint->offset.VZ *= -1;
				vec3scale(joint->offset, scale);
#else
				fscanf(f, "%f %f %f", &joint->offset.VX, &joint->offset.VY, &joint->offset.VZ);
				
				
			#endif

			printf(" the offset is %f %f %f\n", joint->offset.VX, joint->offset.VY, joint->offset.VZ);

			}

			if (EQ("channels")) {
				int chnum = 0;
				int ch;
				int chname;
				
				fscanf(f, "%d", &chnum);
				for (ch = 0; ch < chnum; ch++) {
					NEXT;

					chname = -1;

				#ifdef Z_Y_SWAP

					if (!strcmp(buf, "xrotation")) chname = XROT;
					else if (!strcmp(buf, "yrotation")) chname = ZROT;
					else if (!strcmp(buf, "zrotation")) chname = YROT;
					else if (!strcmp(buf, "xposition")) chname = XPOS;
					else if (!strcmp(buf, "yposition")) chname = ZPOS;
					else if (!strcmp(buf, "zposition")) chname = YPOS;


				#else

					if      (!strcmp(buf, "xrotation")) chname = XROT;
					else if (!strcmp(buf, "yrotation")) chname = YROT;
					else if (!strcmp(buf, "zrotation")) chname = ZROT;
					else if (!strcmp(buf, "xposition")) chname = XPOS;
					else if (!strcmp(buf, "yposition")) chname = YPOS;
					else if (!strcmp(buf, "zposition")) chname = ZPOS;

			#endif
					printf(" channel %d name is %s and name constant is %d\n", ch, buf, chname);
					joint->channels[ch] = chname;
				
				}
				joint->numchannels = chnum;
			}

		}
	
	}
	return NULL;

}


#define INDENT {
void debug_print_skeleton(gfx_jointT* joint, int indent) {
	gfx_jointT* child;
	int i;

	printf("%*s  numchannels  %d\n", indent, "", joint->numchannels);

	for (i = 0; i < zvec_count(&joint->children); i++) {
		child = zvec_get_at(&joint->children, i);
		debug_print_skeleton(child, indent + 4);
	}

}

float aaa = 0;
int frame = 100;

gfx_transformT test_trans;

void debug_draw_skeleton(gfx_jointT* joint, vec3* origin) {
	gfx_jointT* child;
	int i;

	vec3 z;
	vec3set(z, 0, 0, 0);

	if (origin != NULL)  //origin tracks the rest post offet
		z = *origin;
	
	vec3add(z, joint->offset); //track accumulated rest pose offset
	


	gfx_transformT tr;
	
	if ((joint->numchannels == 3) || frame ==-1)
		gfx_translate(&joint->offset);				//use resf post offset translation IF we are in rest pose OR this joint doesn't have per-frame translation data

	int h;


	if (frame >= 0) {


		for (h = 0; h < joint->numchannels; h++) {

			if (joint->channels[h] == XROT)			gfx_rotate_x(joint->framedata[joint->numchannels * frame + h] * DEGREE);
			if (joint->channels[h] == YROT)			gfx_rotate_y(joint->framedata[joint->numchannels * frame + h] * DEGREE);
			if (joint->channels[h] == ZROT)			gfx_rotate_z(-joint->framedata[joint->numchannels * frame + h] * DEGREE);

#ifdef Z_Y_SWAP

			if (joint->channels[h] == XPOS)			gfx_translate3(joint->framedata[joint->numchannels * frame + h], 0, 0);
			if (joint->channels[h] == YPOS)			gfx_translate3(0, joint->framedata[joint->numchannels * frame + h], 0);
			if (joint->channels[h] == ZPOS)			gfx_translate3(0, 0, -joint->framedata[joint->numchannels * frame + h]);
#else
			if (joint->channels[h] == XPOS)			gfx_translate3(joint->framedata[joint->numchannels * frame + h], 0, 0);
			if (joint->channels[h] == YPOS)			gfx_translate3(0, joint->framedata[joint->numchannels * frame + h], 0);
			if (joint->channels[h] == ZPOS)			gfx_translate3(0, 0, joint->framedata[joint->numchannels * frame + h]);

#endif

		}

	}
	


	gfx_save_transform(&tr);

	if (joint->name && !strcmp(joint->name, "lower.arm.l")) {
		gfx_translate3(-z.VX, -z.VY, -z.VZ);  //undo rest pose offset
		gfx_save_transform(&test_trans);  //save this as model transformation

		gfx_load_transform(&tr); //put it back for skel drawing


	}



	for (i = 0; i < zvec_count(&joint->children); i++) {
		vec3 p;

		
		//p=*origin;

		child = zvec_get_at(&joint->children, i);
		
	//	vec3add(p, child->offset);

//		gfx_rotate_z(aaa * DEGREE);
	//	aaa += .0001;


		//gfx_rotate_z(aaa * DEGREE);
		if (origin)
			gfx_arrow(NULL, &child->offset);
					
		debug_draw_skeleton(child, &z);
		gfx_load_transform(&tr);
	}

}



void read_pose_frame(FILE* f, gfx_jointT* joint, int frame, int framecount, float scale) {
	gfx_jointT* child;
	int i;

	

	if (!joint->framedata) {
		joint->framedata = ram_alloc(sizeof(float)* joint->numchannels * framecount, NULL);
	}
	int pos = frame * joint->numchannels;
	for (i = 0; i < joint->numchannels; i++) {
		float val;
		fscanf(f, "%f", &val);

		if ((joint->channels[i] == XPOS)||
			(joint->channels[i] == YPOS)||
			(joint->channels[i] == ZPOS))
			val*= scale;

		joint->framedata[pos++] = val;

	}

	for (i = 0; i < zvec_count(&joint->children); i++) {
		child = zvec_get_at(&joint->children, i);
		read_pose_frame(f, child, frame, framecount, scale);
	}

}


gfx_jointT* load_bvh(char* filename, float scale) {

	FILE* f = fopen(filename, "rb");
	
	//FILE* f = fopen("H:/projects/Zcore-data/web/realistickoreanwoman/skeleton.bvh", "rb");
	gfx_jointT* rootJoint = NULL;

	char buf[100];
	while (NEXT) {

 		if (EQ("hierarchy")) {
			NEXT;
			rootJoint = tryparsejoint(f, buf, scale);
		}
		if (EQ("frames:")) { //not checking for 'motion' keyword, just ignore until frames
			NEXT;
			int framecount = atoi(buf);
			printf("%d frames\n", framecount);

			NEXT;


			while (!isdigit(buf[0]) && (buf[0] != '.')) {
				printf(" ignore string %s\n", buf);
				if (EQ("time:")) {
					NEXT;	//jump over frame time
					break;
				}
				if (!NEXT)
					break;
			}

			int i;
			for (i = 0; i < framecount; i++) {
				read_pose_frame(f, rootJoint, i, framecount, scale);
				

			}


		}
				
	}


	//getc(stdin);

	//print root joint
	debug_print_skeleton(rootJoint, 0);



	fclose(f);
	//exit(0);
	

	return rootJoint;
}