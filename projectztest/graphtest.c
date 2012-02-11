#include <stdio.h>
#include <float.h>
#include <math.h>

#include "..\ztypes.h"
#include "..\vmath.h"
#include "..\memory\ram.h"
#include "..\structures\vector.h"
#include "..\graphics\gx_sys.h"
#include "..\graphics\gx_image.h"
#include "..\graphics\gx_sprite.h"
#include "..\graphics\gx_buffers.h"
#include "..\graphics\gx_drawstyle.h"
#include "..\graphics\gx_misc.h"
#include "..\graphics\gx_mesh.h"
#include "..\graphics\gx_light.h"

float aspect = 1.0;


//return is the new camera sector
gx_sector_t* draw_sector(gx_sector_t* sector, vec3* camera_position, vec3* camera_look, gx_sector_t* camera_sector)
{
	vec3 diff;
	zfloat32 dist=0;
	gx_portal_t * p;
	int i;

	

	zfloat32 cosang=0;

	zbool visible;
	
	
	
	gx_sector_t* new_camera_sector=camera_sector;


	sector->visiting = ztrue;


	
		

	for (i=0;i<sector->meshes.count; i++)
	{
		gx_mesh_draw( vec_get_at( &(sector->meshes) , i)); 	
	}

	
	p = sector->portals;

	while(p)
	{

		vec3mov(diff, p->pos);
		vec3sub(diff, *camera_position);

		dist = sqrt(vec3abs_sq(diff));

		visible= zfalse;

		if (dist < p->radius)
		{
			printf(" inside portal radius\n");
			//inside portal radius, so treat it as visible
			visible = ztrue;

			//if we are inside a portal's radius, we are in the camera's sector
			if (sector==camera_sector)
			{
				//and the camera is exiting this sector...

				if (
					( camera_position->vec3x  > sector->max.vec3x)||
					( camera_position->vec3y  > sector->max.vec3y)||
					( camera_position->vec3z  > sector->max.vec3z)||
					( camera_position->vec3x  < sector->min.vec3x)||
					( camera_position->vec3y  < sector->min.vec3y)||
					( camera_position->vec3z  < sector->min.vec3z)
					)
				{
					printf(" leaving sector\n");
					new_camera_sector = p->target; 
				}

			}
		}
		else
		{
			vec3scale(diff, 1.0/dist);
			//determine portal visibility with 'junk test'
			cosang = vec3dot(*camera_look, diff);
			if (cosang > cos( aspect*35.0   /180*3.14159    +atan(p->radius/dist) ))
			{
				visible = ztrue;
			}
		}

		if (visible)
		{
			
			if (p->target && !p->target->visiting)
				draw_sector(p->target, camera_position, camera_look, camera_sector);

			gx_portal_draw_test(p);
		}

		p=p->next_portal;
	}

	//draw the sector outline
	gx_sector_outline(sector);
	
	
	sector->visiting = zfalse;
	return new_camera_sector;
}

float ranf()
{

	return   ((rand()&0xFFFF) / (float)(0xFFFF));
}


void sector_random(gx_sector_t* sector, int limit)
{
	vec3 portal_pos ;
	vec3 diff;

	gx_sector_t* target = NULL;

	int axis = rand() % 3;
	int minmax = rand() % 2;
	
	float rx = ranf();
	float ry = ranf();
	float rz=  ranf();

	if (limit <=0) 
		return;

	vec3mov(diff, sector->max);
	vec3sub(diff, sector->min);
	
	vec3mov(portal_pos, sector->min);
	portal_pos.named.x += rx* diff.named.x;
	portal_pos.named.y += rx* diff.named.y;
	portal_pos.named.z += rx* diff.named.z;

	//stick portal to one of the walls
	portal_pos.array[axis] = minmax ? sector->min.array[axis] : sector->max.array[axis];
	
	//now we need a target sector
	{
		vec3 min;
		vec3 max;
		
		vec3mov(min, portal_pos);
		vec3mov(max, portal_pos);
		
		min.named.x -= ranf() * 2+1;
		min.named.y -= ranf() * 2+1;
		min.named.z -= ranf() * 2+1;

		max.named.x += ranf() * 2+1;
		max.named.y += ranf() * 2+1;
		max.named.z += ranf() * 2+1;

		if (!minmax)
			min.array[axis] = portal_pos.array[axis]+.01;
		else
			max.array[axis] = portal_pos.array[axis]-.01;

		target = gx_sector_mk(&min, &max);

	}



//	gx_sector_add_portal( sector, &portal_pos, 0.25, target); 
	//add reciprocal
//	gx_sector_add_portal( target,  &portal_pos, 0.25, sector);

	sector_random(target, limit-1);

}



//


typedef struct q_triangle_s
{
	struct q_triangle_s* parent;
	struct q_triangle_s* child[4];

	//gx_vbuffer_t* vb;
	struct tri_quadtree_buffer_s* tqbuffer;  //which buffer do we belong to?
	
	
	
	zint32 v[3]; //what vertex numbers in our vbuffer
	zint32 prim;  //what primitive number in our vbuffer
	
	vec3 middle;
	zfloat32 size;

	zbool enabled;
} q_triangle_t;



typedef struct tri_quadtree_buffer_s
{
	gx_vbuffer_t* vb; //vbuffer used for this set of triquadtrees
	vec_t triangles;  //keep a list of triangle that occupy this buffer
	zuint32 index_start;  //which indices of the buffer are actually drawn
	zuint32 index_end;

	struct tri_quadtree_buffer_s * next;  //many times need multiple buffers

	zbool dirty;

} tri_quadtree_buffer_t;


//how many vertex/index to keep in each
#define TQSIZE_VERTEX	65535
#define TQSIZE_INDEX	(65535*6)


static void _tqbkill(void* buf)
{
	//put cleanup code here
}

tri_quadtree_buffer_t* tqbuffer_mk()
{
	tri_quadtree_buffer_t* tqb = ram_alloc(sizeof(tri_quadtree_buffer_t), _tqbkill);
	if (tqb)
	{
		vec_mk( &tqb->triangles, 16);
		tqb->vb = gx_vbuffer_mk( TQSIZE_VERTEX, TQSIZE_INDEX, zfalse, zfalse, 1);
	}

	printf(" CREATING BUFFER\n");
	return tqb;
}

q_triangle_t* tqbuffer_add_triangle(tri_quadtree_buffer_t* tqbuffer, q_triangle_t* parent, int v0, int v1, int v2)
{

	q_triangle_t* tri = NULL;
	vec3 middle;

	tri = ram_alloc(sizeof(q_triangle_t), NULL);

	tri->enabled = ztrue; //make this drawable
	tri->tqbuffer = tqbuffer; //belongs to this buffer
	

	tri->parent = parent;

	//store indices in the triangle AND the vbuffer

	tri->v[0] = v0;
	tri->v[1] = v1; 
	tri->v[2] = v2;

	//gx_vbuffer_add_index( tqbuffer->vb, v0);
	//gx_vbuffer_add_index( tqbuffer->vb, v1);
	//gx_vbuffer_add_index( tqbuffer->vb, v2);


	tqbuffer->dirty = ztrue;

	vec_add( &tqbuffer->triangles, tri);

	{
		float s=0;
		vec3 a;

		vec3mov(a, *gx_vbuffer_v( tqbuffer->vb, v0) );  
		vec3sub(a, *gx_vbuffer_v( tqbuffer->vb, v0) );  
		s += sqrt( vec3abs_sq(a) );

		vec3mov(a, *gx_vbuffer_v( tqbuffer->vb, v0) );  
		vec3sub(a, *gx_vbuffer_v( tqbuffer->vb, v1) );  
		s += sqrt( vec3abs_sq(a) );


		vec3mov(a, *gx_vbuffer_v( tqbuffer->vb, v1) );  
		vec3sub(a, *gx_vbuffer_v( tqbuffer->vb, v2) );  
		s += sqrt( vec3abs_sq(a) );

		
 		tri->size  = s;//  start with unit size tris

	}


	//average 3 point to produce middle of triangle

 	vec3mov(middle, *gx_vbuffer_v( tqbuffer->vb, v0));
	vec3add(middle, *gx_vbuffer_v( tqbuffer->vb, v1));
	vec3add(middle, *gx_vbuffer_v( tqbuffer->vb, v2));
	vec3scale(middle, 1.0/3.0);
	vec3mov(tri->middle, middle);

	return tri;
}


//true for success
zbool q_triangle_split(q_triangle_t* tri, int num)
{

	int v_0_1 = GX_INDEX_INVALID;
	int v_1_2 = GX_INDEX_INVALID;
	int v_2_0 = GX_INDEX_INVALID;
	

	int v0 = GX_INDEX_INVALID;
	int v1 = GX_INDEX_INVALID;
	int v2 = GX_INDEX_INVALID;
	


	tri_quadtree_buffer_t* insert_to= NULL;

	vec3 a;
	float s;  //texcoords
	float t;

	vec3 rnd;
	
	if (num<=0)
		return zfalse;

	//split triangle into 4
	if (tri->child[0])
	{
	//	return zfalse;  //already have children, ignore
	
		//fast case: already have children computed

		int i;
		for (i=0;i<4;i++)
		{
			//if (!q_triangle_split(tri->child[i], num-1))
			{
				vec_add( &tri->child[i]->tqbuffer->triangles, tri->child[i]);
				tri->child[i]->enabled=ztrue;
			}
			

		}
		

		return ztrue;
	}
	
	vec3set(rnd, tri->size * ranf(),  tri->size * ranf() , tri->size * ranf());
	vec3scale(rnd, .05);


	//check for free space
	//creating 3 vertices

	insert_to = tri->tqbuffer;

	while (
		((gx_remaining_vertices(insert_to->vb) < 3)||(gx_remaining_indices(insert_to->vb) < 4*3) )
		&& insert_to->next)
	{
		insert_to = insert_to->next;
	}

	if (
		(gx_remaining_vertices(insert_to->vb) < 3)||(gx_remaining_indices(insert_to->vb) < 4*3))
	{
		tri_quadtree_buffer_t* newbuf = tqbuffer_mk();
		newbuf->next = insert_to->next;
		insert_to->next = newbuf;
		insert_to = newbuf;
		
	}
		
	if (insert_to != tri->tqbuffer)
	{
		
		//copy this point to new vbuffer

		gx_vbuffer_add_tex(insert_to->vb, 0 , gx_vbuffer_s( tri->tqbuffer->vb, tri->v[0], 0), gx_vbuffer_t( tri->tqbuffer->vb, tri->v[0], 0));	
		v0 = gx_vbuffer_add_vertex(insert_to->vb, gx_vbuffer_x( tri->tqbuffer->vb, tri->v[0], 0), gx_vbuffer_y( tri->tqbuffer->vb, tri->v[0], 0), gx_vbuffer_z( tri->tqbuffer->vb, tri->v[0], 0));


		gx_vbuffer_add_tex(insert_to->vb, 0 , gx_vbuffer_s( tri->tqbuffer->vb, tri->v[1], 0), gx_vbuffer_t( tri->tqbuffer->vb, tri->v[1], 0));	
		v1 = gx_vbuffer_add_vertex(insert_to->vb, gx_vbuffer_x( tri->tqbuffer->vb, tri->v[1], 0), gx_vbuffer_y( tri->tqbuffer->vb, tri->v[1], 0), gx_vbuffer_z( tri->tqbuffer->vb, tri->v[1], 0));


		gx_vbuffer_add_tex(insert_to->vb, 0 , gx_vbuffer_s( tri->tqbuffer->vb, tri->v[2], 0), gx_vbuffer_t( tri->tqbuffer->vb, tri->v[2], 0));	
		v2 = gx_vbuffer_add_vertex(insert_to->vb, gx_vbuffer_x( tri->tqbuffer->vb, tri->v[2], 0), gx_vbuffer_y( tri->tqbuffer->vb, tri->v[2], 0), gx_vbuffer_z( tri->tqbuffer->vb, tri->v[2], 0));

	} 
	else
	{
		v0=tri->v[0];
		v1=tri->v[1];
		v2=tri->v[2];
	}
	
	insert_to->dirty = ztrue;

	//midpoint 0 1

	s = gx_vbuffer_s( tri->tqbuffer->vb, tri->v[0], 0);
	t = gx_vbuffer_t( tri->tqbuffer->vb, tri->v[0], 0);
	s += gx_vbuffer_s( tri->tqbuffer->vb, tri->v[1], 0);
	t += gx_vbuffer_t( tri->tqbuffer->vb, tri->v[1], 0);
	
	vec3mov(a, *gx_vbuffer_v(tri->tqbuffer->vb, tri->v[0]));
	vec3add(a, *gx_vbuffer_v(tri->tqbuffer->vb, tri->v[1]));
	vec3scale(a, 0.5);
	
	vec3add(a,rnd);

	gx_vbuffer_add_tex(insert_to->vb, 0, s/2, t/2);
	v_0_1 = gx_vbuffer_add_vertex(insert_to->vb, a.named.x, a.named.y, a.named.z);
	

	
	//midpoint 1 2

	s = gx_vbuffer_s( tri->tqbuffer->vb, tri->v[1], 0);
	t = gx_vbuffer_t( tri->tqbuffer->vb, tri->v[1], 0);
	s += gx_vbuffer_s( tri->tqbuffer->vb, tri->v[2], 0);
	t += gx_vbuffer_t( tri->tqbuffer->vb, tri->v[2], 0);
	
	vec3mov(a, *gx_vbuffer_v(tri->tqbuffer->vb, tri->v[1]));
	vec3add(a, *gx_vbuffer_v(tri->tqbuffer->vb, tri->v[2]));
	vec3scale(a, 0.5);

	vec3add(a,rnd);

	gx_vbuffer_add_tex(insert_to->vb, 0, s/2, t/2);
	v_1_2 = gx_vbuffer_add_vertex(insert_to->vb, a.named.x, a.named.y, a.named.z);

	//midpoint 2 0

	s = gx_vbuffer_s( tri->tqbuffer->vb, tri->v[2], 0);
	t = gx_vbuffer_t( tri->tqbuffer->vb, tri->v[2], 0);
	s += gx_vbuffer_s( tri->tqbuffer->vb, tri->v[0], 0);
	t += gx_vbuffer_t( tri->tqbuffer->vb, tri->v[0], 0);
	
	vec3mov(a, *gx_vbuffer_v(tri->tqbuffer->vb, tri->v[2]));
	vec3add(a, *gx_vbuffer_v(tri->tqbuffer->vb, tri->v[0]));
	vec3scale(a, 0.5);

	vec3add(a,rnd);

	gx_vbuffer_add_tex(insert_to->vb, 0, s/2, t/2);
	v_2_0 = gx_vbuffer_add_vertex(insert_to->vb, a.named.x, a.named.y, a.named.z);


	//create the 4 triangles

	
	tri->child[0] = tqbuffer_add_triangle(insert_to, tri, v0, v_0_1, v_2_0);
	tri->child[1] = tqbuffer_add_triangle(insert_to, tri, v2, v_2_0, v_1_2);
	tri->child[2] = tqbuffer_add_triangle(insert_to, tri, v1, v_1_2, v_0_1);
	tri->child[3] = tqbuffer_add_triangle(insert_to, tri, v_2_0, v_0_1, v_1_2);
	

	return ztrue;
}	


void tqbuffer_process(tri_quadtree_buffer_t* tqbuffer, vec3* camera_pos)
{
	int i;
	vec3 diff;
	q_triangle_t * tri = NULL;
	float dist2;
	float ssize;  //screen size
	float ssizeparent;  //screen size of parent
	zbool split = zfalse;


	if (tqbuffer->vb->index_count ==0)
		split=1;

	for (i=0;i< vec_count( &tqbuffer->triangles); i++)
	{
		tri = (q_triangle_t*) vec_get_at( &tqbuffer->triangles, i);
		
		if (!tri->enabled)
			continue; //skip disabled triangles

		vec3mov(diff, tri->middle );
		vec3sub(diff, *camera_pos);  //subtract camera position
		dist2 = vec3abs_sq(diff);  //dist is distance squared
		ssize = tri->size / dist2;  //size is apparent size over distance squared 
		

		//do same calculation but for parent
		if (tri->parent)
		{
			

			vec3mov(diff, tri->parent->middle );
			vec3sub(diff, *camera_pos);  //subtract camera position
			dist2 = vec3abs_sq(diff);  //dist is distance squared
			ssizeparent = tri->parent->size / dist2;  //size is apparent size over distance squared 
		}

		if (ssize > 1)
		{
			//need to split		
			if(q_triangle_split(tri,2))
			{
				tri->enabled = zfalse;
			//	vec_remove_unordered(&tqbuffer->triangles, i);  //remove this triangle

			}
			split = ztrue;
		}
		else
		if (tri->parent)
		{
			//need to recombine
			if (ssizeparent < .9 )
			{

				tri->parent->child[0]->enabled = zfalse;
				tri->parent->child[1]->enabled = zfalse;
				tri->parent->child[2]->enabled = zfalse;
				tri->parent->child[3]->enabled = zfalse;
				tri->parent->enabled= ztrue;
				vec_add(&tri->parent->tqbuffer->triangles, tri->parent);



				split=ztrue;  //need to reprocess
			}
		}
	}


	if (split || tqbuffer->dirty)  //if we did a split, rebuild the index buffer
	{
		tqbuffer->dirty = zfalse;
		gx_vbuffer_clear(tqbuffer->vb, ztrue, zfalse); //clear indices

		for (i=0;i< vec_count( &tqbuffer->triangles); i++)
		{
			tri = (q_triangle_t*) vec_get_at( &tqbuffer->triangles, i);
			
			if (tri->enabled)
			{
				gx_vbuffer_add_index(tqbuffer->vb,tri->v[0]);
				gx_vbuffer_add_index(tqbuffer->vb,tri->v[1]);
				gx_vbuffer_add_index(tqbuffer->vb,tri->v[2]);
			}
			else
			{
				vec_remove_unordered(&tqbuffer->triangles, i);
				i--;
				continue;
			}
		}

		gx_vbuffer_update( tqbuffer->vb);

		printf(" change %d \n",vec_count( &tqbuffer->triangles) );
	} 


	if (tqbuffer->next)
		tqbuffer_process(tqbuffer->next, camera_pos);
}





//main
void graphtest_main()
{
	float f=0;
	zbool processtq = zfalse;

	zfloat32 cx=0;
	zfloat32 cy=.5;
	zfloat32 cz=1;
	float aaa=0;
	gx_sector_t* sector0 = NULL;
	gx_sector_t* sector1 = NULL;
	gx_sector_t* sector2 = NULL;

	gx_sector_t* camera_sector = NULL; //where the camera is currently located

	vec3 camera_pos;
	vec3 camera_up;
	vec3 camera_right;
	vec3 camera_forward;

	gx_mesh_t* mesh0 = NULL;
	gx_mesh_t* mesh1 = NULL;

	gx_drawstyle_t ds1;
	gx_drawstyle_t ds2;


	zuint32 start =0;
	zuint32 end=0;
	
	zfloat32 cxs=0;
	zfloat32 cys=0;
	zfloat32 czs=0;
	zfloat32 rolls=0;

	zfloat32 xxx=0;
	zfloat32 yyy=0;

	gx_vbuffer_t * sharebuffer = NULL;

	gx_vbuffer_t * normbuffer = NULL;


	gx_vbuffer_t * vbuf = NULL;
	gx_image_t *image1 = NULL;
	gx_image_t *image2 = NULL;
	gx_sprite_t* sprite = NULL;

	gx_image_t* heightmap = NULL;
	gx_vbuffer_t* heightbuffer = NULL;

	gx_image_t* heightmap2 = NULL;
	gx_vbuffer_t* heightbuffer2 = NULL;


	gx_mesh_t* objmesh2 = NULL;
	gx_mesh_t* objmesh = NULL;
	gx_image_t* objmeshtex = NULL;

	gx_light_t* lights[8];
	
	//try some buffer crap


	tri_quadtree_buffer_t * tqbuffer = NULL;


	




	{
		vec3 pos;
		vec3 color;
		vec3 ambient;
		vec3set(pos, 1,1,0);
		vec3set(color, 1, 0, 0);
		vec3set(ambient, 0,0,0);

		lights[0] = gx_light_mk(gx_light_directional, &pos, &color, &ambient);
		
	//	vec3set(pos, .5,0,0);
	//	vec3set(color, 1, 1, 0);
	//	lights[1] = gx_light_mk(gx_light_point, &pos, &color, &ambient);
	}

	printf("Init graphics\n");
	gx_init(1280, 1024 , "Test Graphics Window");

	gx_clear_color(0,0,0,1);
	gx_frame_clear(ztrue,ztrue);
	gx_frame_show();

//	objmeshtex  = gx_image_load_tga( "E:\\mark\\projects\\projectZ\\meshtexture.tga");
//	objmesh		= gx_mesh_load_obj(NULL, "E:\\mark\\projects\\projectZ\\mesh.obj", NULL);
//	objmesh2		= gx_mesh_load_obj(NULL, "E:\\mark\\projects\\projectZ\\bozo.obj", NULL);

	//update whole mesh
	{
		int piece=0;
		gx_mesh_t* z = objmesh;
		
		while(z)
		{
			printf("update piece %d\n", piece++);
			gx_vbuffer_update(z->data);
			z=z->next;
		}
	}

	if (objmesh2)
		 gx_vbuffer_update(objmesh2->data);


	image1 = gx_image_load_tga( "testship.tga");
	image2 = gx_image_load_tga( "tex2.tga");
	
	
	//gx_image_enable(image1);  //don't need to enable because first use will

	vec3set(camera_pos,			0,	.5,	1);
	vec3set(camera_right,	    1,	0,	0);
	vec3set(camera_up,		    0,	1,	0);
	vec3set(camera_forward,		0,	0,	-1);

	sprite = gx_sprite_mk(image1,100,100, image1->width, image1->height, .1, .1);

	vbuf = gx_vbuffer_mk(100, 12, ztrue,zfalse, 1);
	
	{
		zuint32 v0=GX_INDEX_INVALID;
		zuint32 v1=GX_INDEX_INVALID;
		zuint32 v2=GX_INDEX_INVALID;
		zuint32 v3=GX_INDEX_INVALID;
		zuint32 v4=GX_INDEX_INVALID;


		gx_vbuffer_add_tex(vbuf, 0, 0,0);
		gx_vbuffer_add_color(vbuf, 1, 1, 1, 1);
		v0=gx_vbuffer_add_vertex(vbuf, .1,.1,0);

		gx_vbuffer_add_tex(vbuf,0, 0,1);
		gx_vbuffer_add_color(vbuf, 0, 1, 0, 1);
		v1=gx_vbuffer_add_vertex(vbuf, .2, .1,0);

		gx_vbuffer_add_tex(vbuf,0, 1,1);
		gx_vbuffer_add_color(vbuf, 0, 0, 1, 1);
		v2=gx_vbuffer_add_vertex(vbuf, 0,.2,0);

		gx_vbuffer_add_tex(vbuf,0, 1,1);
		gx_vbuffer_add_color(vbuf, 0, 0, 1, 1);
		v3=gx_vbuffer_add_vertex(vbuf, .3,.2,0);


		gx_vbuffer_add_tex(vbuf,0, 1,1);
		gx_vbuffer_add_color(vbuf, 0, 0, 1, 1);
		v4=gx_vbuffer_add_vertex(vbuf, .15,.3,0);

		/*
        4

    2       3

      0   1
*/



		gx_vbuffer_add_index(vbuf, v0);
		gx_vbuffer_add_index(vbuf, v4);

		gx_vbuffer_add_index(vbuf, v4);
		gx_vbuffer_add_index(vbuf, v1);

		gx_vbuffer_add_index(vbuf, v1);
		gx_vbuffer_add_index(vbuf, v2);

		gx_vbuffer_add_index(vbuf, v2);
		gx_vbuffer_add_index(vbuf, v3);


		gx_vbuffer_add_index(vbuf, v3);
		gx_vbuffer_add_index(vbuf, v0);

	}


	gx_vbuffer_update(vbuf);  //make sure latest data is ready

	//heightmap = gx_image_load_tga("heightmap.tga");
	//heightmap = gx_image_load_tga("rockheight.tga");
	heightmap = gx_image_load_tga("shipheight_top.tga");

	sharebuffer = gx_vbuffer_mk( 2*512*512*4, 2*3*512*512*4, ztrue, zfalse, 2);

	normbuffer = gx_vbuffer_mk( 2*512*512*4, 2*3*512*512*4, zfalse, ztrue, 2);


	heightbuffer = gx_vbuffer_from_image( normbuffer, heightmap, 0.0,0.0,-1.0,   //offset
		0,1,2,        //axis swizzle
		1.0,.2,1.0,  //scaling
		zfalse, 2, &start, &end, zfalse);

//	

	mesh0 = gx_mesh_def( heightbuffer, NULL, start, end, ztrue);
	
	

	heightmap2 = gx_image_load_tga("shipheight_bottom.tga");

	heightbuffer2 = gx_vbuffer_from_image(normbuffer, heightmap2, 0.0,0.0,-1.0,   //offset
		0,1,2,        //axis swizzle
		1.0,-.1,1.0,  //scaling
		zfalse, 2, &start, &end, ztrue);


	gx_vbuffer_update(heightbuffer);
	if (heightbuffer2 != heightbuffer)
		gx_vbuffer_update(heightbuffer2);


	mesh1 = gx_mesh_def( heightbuffer2, NULL, start, end, ztrue);

	ram_free(heightmap);
	ram_free(heightmap2);
	heightmap =0;
	heightmap2=0;

	ram_clear(&ds1, sizeof(ds1));
	ram_clear(&ds2, sizeof(ds2));

	ds2.numtextures = 2;
	ds2.textures = ram_alloc( sizeof(gx_image_t*) *2 , NULL);
	ds2.textures[0] = image2;
	ds2.textures[1] = image1;

	//mesh0->next = mesh1;  //link both meshes

#if 0

	{

		vec3 mins;
		vec3 maxs;
		vec3 pos;

		vec3set(mins, 0,0, -1);
		vec3set(maxs, 1,1, 1);
		sector0 = gx_sector_mk(&mins, &maxs);


		vec3set(mins, 0,0, -3);
		vec3set(maxs, 1,1, -1);
		sector1 = gx_sector_mk(&mins, &maxs);

		vec3set(mins, 1,0, -4);
		vec3set(maxs, 2,2, -2);
		sector2 = gx_sector_mk(&mins, &maxs);

		vec3set(pos, .5,.5 ,-1);
		gx_sector_add_portal(sector0, &pos, .5, sector1);

		vec3set(pos, .5,.5 ,-3);
		gx_sector_add_portal(sector1, &pos, .5 , sector2);
		
		camera_sector = sector0;  //start here

	}


	vec_add(   & (sector0->meshes)  , mesh0);
	vec_add(   & (sector1->meshes)  , mesh1);
#else

{
	vec3 mins;
	vec3 maxs;
	
	vec3set(mins, -.5,0, -1);
	vec3set(maxs, 1,1, 1.5);
	sector0 = gx_sector_mk(&mins, &maxs);

	sector_random(sector0, 10);

	camera_sector = sector0;  //start here

}
#endif


	gx_mouse_capture(ztrue); //capture the mouse for relative motion




	tqbuffer = tqbuffer_mk();
	
	{
		int v0, v1, v2;

		gx_vbuffer_add_tex(tqbuffer->vb,0, 0.0,1.0); 
		v0 = gx_vbuffer_add_vertex(tqbuffer->vb, 0, -1, -1);
		
		gx_vbuffer_add_tex(tqbuffer->vb,0, 1.0,1.0); 
		v1 = gx_vbuffer_add_vertex(tqbuffer->vb, 1, -1, -1);

		gx_vbuffer_add_tex(tqbuffer->vb,0, 1.0,0.0); 
		v2 = gx_vbuffer_add_vertex(tqbuffer->vb, 1, -1, -2);

 		tqbuffer_add_triangle(tqbuffer, NULL, v0, v1, v2);
		tqbuffer_process(tqbuffer, &camera_pos);
		

	}
	//update the initial buffer:
	gx_vbuffer_update(tqbuffer->vb);  //update it



	while( 1)
	{
		

	

		zint32 mx, my;
		zbool rel;
	
		f+=.001;
		

		//if (processtq)
		//	tqbuffer_process(tqbuffer, &camera_pos);


		gx_window_event();


		gx_mouse_pos(&mx, &my, &rel);

		
		if (!rel)
		{
			//if not in relative mode, we don't want mouse movement numbers
			mx=0;
			my=0; 
		}

		//printf(" mouse position %d %d\n", mx, my);

		gx_frame_clear(ztrue,ztrue);

		//gx_setup_2d(-1,1,1,-1);
		//gx_vbuffer_draw(vbuf,2,4, gx_lines, ztrue);
		//gx_sprite_draw(sprite, xxx,yyy);

		xxx+=.01;
		yyy+=.03;

		if (xxx>1) xxx=-1;
		if (yyy>1) yyy=-1;
	
		gx_setup_3d( 70.0, aspect = gx_frame_get_dimensions(NULL,NULL), .01, 1000);

		

		{
			char c;
			
			zfloat32 yaw	= 0.0;
			zfloat32 pitch	= 0.0;
			zfloat32 roll	= rolls;
			
			czs=0;
			cys=0;
			cxs=0;



			if (gx_key_state('w')) czs=.01;
			if (gx_key_state('s')) czs=-.01;
			if (gx_key_state('a')) cxs=-.01;
			if (gx_key_state('d')) cxs=+.01;
			if (gx_key_state('r')) cys=+.01;
			if (gx_key_state('f')) cys=-.01;


			if (gx_key_state('q')) roll=-.02;
			if (gx_key_state('e')	) roll=.02;

			if (gx_key_state('4')) yaw=-.02;
			if (gx_key_state('6')) yaw=.02;

			if (gx_key_state('8')) pitch=-.02;
			if (gx_key_state('2')) pitch=.02;

			c = gx_getkey();
			if (c=='Q') 
				break;
			
			if (c=='m')
				gx_mouse_capture(zfalse);
				
			if (c=='p')
				processtq^=1;
			

			if (c=='M')
				gx_mouse_capture(ztrue);
			


			pitch += my*.001;
			yaw += mx*.001;
	

#if 0
	//restrict motion to current sector
			if ( camera_pos.named.x > camera_sector->max.named.x)
				camera_pos.named.x = camera_sector->max.named.x;

			if ( camera_pos.named.y > camera_sector->max.named.y)
				camera_pos.named.y = camera_sector->max.named.y;

			if ( camera_pos.named.z > camera_sector->max.named.z)
				camera_pos.named.z = camera_sector->max.named.z ;

			if ( camera_pos.named.x < camera_sector->min.named.x)
				camera_pos.named.x = camera_sector->min.named.x;

			if ( camera_pos.named.y < camera_sector->min.named.y)
				camera_pos.named.y = camera_sector->min.named.y;

			if ( camera_pos.named.z < camera_sector->min.named.z)
				camera_pos.named.z = camera_sector->min.named.z ;

#endif

			//move camera
			vec3madd(camera_pos, cxs, camera_right);
			vec3madd(camera_pos, cys, camera_up);
			vec3madd(camera_pos, czs, camera_forward);
			
			//try some spin crap
			gx_spin(ztrue, yaw, pitch, roll,&camera_right, &camera_up, &camera_forward);

		}
					
		gx_camera_pos_rot( &camera_pos, &camera_right, &camera_up, &camera_forward);
		
	gx_set_active_textures(NULL, 0);

	
	//draw sectors, and return the new camera sector
//	camera_sector = draw_sector(camera_sector, &camera_pos, &camera_forward, camera_sector );


	//gx_sector_outline(sector0);
	//gx_sector_outline(sector1);
	//gx_sector_outline(sector2);

#if 0
		{
			gx_image_t * txlist[2];
			txlist[0]=image1;
			txlist[1]=image2;
			gx_set_active_textures( txlist, 2);
		}

		gx_vbuffer_draw(heightbuffer,0,heightbuffer->index_count, gx_triangles, ztrue);

		gx_set_active_textures(NULL,0);
		gx_vbuffer_draw(heightbuffer2,0,heightbuffer2->index_count, gx_triangles, ztrue);

#endif

	



		mesh0->style = NULL;

		

	

		{

			gx_drawstyle_t ds1;
			int j;
			vec3 vv;
			vec3set(vv, .5,.5,-3.5);

			ds1.blending = ztrue;
			ds1.numtextures = 1;
			ds1.textures = &objmeshtex;
			
			
			ds1.specular_color.array[0]=0;
			ds1.specular_color.array[1]=0;
			ds1.specular_color.array[2]=1;

			ds1.specular_exponent=100;

			//ds1.textures = &image1;

			

			
			
			

		
		//	lights[0]->position.named.z -=.001;
		//	lights[0]->position.named.x -=.01;
		//	lights[0]->position.named.y+=.001;
			
		

			//gx_set_active_textures( NULL  , 0);	 

		

			//gx_mesh_draw_at(objmesh2, &vv);
		//	gx_set_active_textures( &objmeshtex  , 1);

			vv.named.z += 2;
			
			gx_drawstyle_activate(&ds1);

#if 0
			{
				vec3 x,y,z;
				vec3 s;
				
				vec3set(s, 0,.5,0);
				gx_move3d(&s);

				vec3set(s, .1,.1,.1);
				gx_scale3d(&s);


				vec3set(x,  cos(aaa), 0,sin(aaa));
				vec3set(y,  0, 1,0);
				vec3set(z,  -sin(aaa),0, cos(aaa));

				gx_rotate_3x3(&x, &y, &z);

				aaa+=.005;

			}
#endif
				
			
			
			gx_home();  //reset transformations 
			
			gx_set_active_textures(NULL, 0);
			gx_debug_show_light(lights[0], .1);
			
			gx_set_active_lights(lights, 1);

			



			ds1.blending = 0;
			gx_drawstyle_activate(&ds1);
			gx_set_active_textures(&image1,1);
			
			{
				vec3 v ;
				//vec3set(v, f,0,0);

				//gx_move3d(&v);

			}
			
			gx_set_active_textures(NULL, 0);
 			gx_mesh_draw(mesh0);
			gx_mesh_draw(mesh1);

		

		
		

			gx_set_active_lights(lights, 0); //no light
			
#if 0
			{
				tri_quadtree_buffer_t * b = tqbuffer;

				while(b)
				{

					gx_vbuffer_draw(b->vb, 0, b->vb->index_count, gx_triangles, ztrue);
					b=b->next;
				}

			}
#endif
			

			

			

		}

		//draw camera's sector

			
		
		//camera_sector = draw_sector(camera_sector, &camera_pos, &camera_forward, camera_sector );
		
		
		

		gx_frame_show();
	}

}


