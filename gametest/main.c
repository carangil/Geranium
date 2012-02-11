// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include <math.h>
#include "../ztypes.h"
#include "../memory/ram.h"
#include "../vmath.h"
#include "../graphics/gx_sys.h"
#include "../graphics/gx_image.h"
#include "../graphics/gx_sprite.h"
#include "../graphics/gx_line.h"
#include "../graphics/gx_buffers.h"
#include "../graphics/gx_drawstyle.h"
#include "../structures/vector.c"
#include "../graphics/gx_misc.h"
#include "../graphics/gx_mesh.h"
#include "../graphics/gx_light.h"


zbool g_toggle0 = zfalse;

//math helper

//random float 0.0 to 1.0
float randf()
{
	return (rand()&255) / 255.0;
}


//random float -1.0 to 1.0
float randfs()
{
	return -1.0 + 2*(rand()&511) / 511.0;
}

//normalize a vector
void vec3norm( vec3* p)
{
	float d = sqrt( vec3abs_sq(*p)  );
	vec3scale( *p, (1/d) );
}

//camera stuff -todo move into library

typedef struct g_camera_s
{
	vec3 camera_pos;
	vec3 camera_forward;
	vec3 camera_right;
	vec3 camera_up;
} g_camera_t;

void g_camera_init(g_camera_t* cam)
{
	if (cam)
	{
		vec3set( cam->camera_pos,		0.0f, 0.0f, 0.0f);
		vec3set( cam->camera_right,		1.0f, 0.0f, 0.0f);
		vec3set( cam->camera_up,		0.0f, 1.0f, 0.0f);
		vec3set( cam->camera_forward,	0.0f, 0.0f, -1.0f);
	}
}

//SHEETS  ::todo: move to library if 'useful'
typedef struct sheet_edge_s
{
	vec_t indirect_vertices; //pointer to vertices on the edge of this sheet
} sheet_edge_t;

typedef struct sheet_s
{
	gx_vbuffer_t* vb;  //holds points for this sheet
	int numpoints;	  //how many points are in this sheet  (only for non-rectangulat sheets)
	int maxpoints;
	
	int width;			//w/h only for rectangulat sheets
	int height;
	
	int* points;	  //vertices within the vbuffer

	int num_edges;	  //3 or 4 edges (sheets can be triangular or rectangular)
	sheet_edge_t	edges[4]; 

} sheet_t;



#define SHEET_EDGE_TOP		0
#define SHEET_EDGE_BOTTOM	1
#define SHEET_EDGE_LEFT		2
#define SHEET_EDGE_RIGHT	3

//NOTE:  SHEET_POINT_AT is not safe!
#define SHEET_POINT_AT(SSSS,XXXX,YYYY)    ((SSSS)->points[   (SSSS)->width*(YYYY) + XXXX ] )

//create 2D sheet.  Does not have any points filled out yet
sheet_t * sheet_quad_mk(gx_vbuffer_t* vb, int width, int height)
{
	sheet_t* sheet = ram_alloc(sizeof(sheet_t), NULL);
	int a;

	if (!sheet)
		return NULL;

	sheet->numpoints = 0;
	sheet->width = width;
	sheet->height = height;
	sheet->maxpoints = width*height;
	sheet->points = ram_alloc(sizeof(zint32) * sheet->maxpoints, NULL);
	sheet->vb = vb;
	sheet->num_edges = 4;
	
	
	vec_mk( &sheet->edges[SHEET_EDGE_TOP].indirect_vertices, width);
	vec_mk( &sheet->edges[SHEET_EDGE_BOTTOM].indirect_vertices, width);
	for (a=0;a<width;a++)
	{
		vec_add(&(sheet->edges[SHEET_EDGE_TOP].indirect_vertices), & SHEET_POINT_AT(sheet, a, 0));
		vec_add(&(sheet->edges[SHEET_EDGE_BOTTOM].indirect_vertices), & SHEET_POINT_AT(sheet, a, height-1));
	}
	
	vec_mk( &sheet->edges[SHEET_EDGE_LEFT].indirect_vertices, height);
	vec_mk( &sheet->edges[SHEET_EDGE_RIGHT].indirect_vertices, height);

	for (a=0;a<height;a++)
	{
		vec_add(&sheet->edges[SHEET_EDGE_LEFT].indirect_vertices, & SHEET_POINT_AT(sheet, 0, a));
		vec_add(&sheet->edges[SHEET_EDGE_RIGHT].indirect_vertices, & SHEET_POINT_AT(sheet, width-1, a));
	}

	return sheet;
}

int sheet_set_at( sheet_t* s, int x, int y,  int vertex)
{
	if (!s)
		return GX_INDEX_INVALID;


	SHEET_POINT_AT(s, x, y) = vertex;

	return vertex;
}

void show_buffer(sheet_t* s)
{
	int a;
	for (a=0;a< s->width * s->height;a++)
	{
		if (a%s->width ==0) printf("\n");
		printf(" %02d", s->points[a]);
	}

}

//this function sets one edge of S to use vertices from T

#define		ASSIGN_INDICES 1
#define		COPY_POSITION  2


//#define		COPY_TEXCOORD  4


void sew_sheets( sheet_t* s, int s_edge, sheet_t* t, int t_edge, int operation)
{
	int a;
	
	if (s->vb != t->vb)  //can't sew sheets that are in different vbuffers
		return;
	
	if (s->edges[s_edge].indirect_vertices.count != t->edges[t_edge].indirect_vertices.count)
		return;  //can't sew sheets that have different arity


	for (a=0;a< s->edges[s_edge].indirect_vertices.count;a++)
	{
		if (operation & ASSIGN_INDICES)
		{
			*(int*)(s->edges[s_edge].indirect_vertices.elements[a]) =  *(int*)(t->edges[t_edge].indirect_vertices.elements[a]);
		}

		if (operation & COPY_POSITION)
		{
			vec3* spos = gx_vbuffer_v(s->vb, *(int*)(s->edges[s_edge].indirect_vertices.elements[a]));
			vec3* tpos = gx_vbuffer_v(t->vb, *(int*)(t->edges[t_edge].indirect_vertices.elements[a]));;
			
			vec3mov (*spos, *tpos);
		}

	}

}

///#define interpolate(faaa, fbbb, fttt)   (((fbbb-faaa)*fttt)+faaa)
/*
        s---->
		2   3t
			 |
			 |
		0   1V


*/
void interpolate_tex(float* tx_out, float* ty_out, float s, float t, float tx0, float ty0, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3)
{
	float ns = 1.0-s;
	float nt = 1.0-t;

	float topx = tx0 * ns + tx1*s;
	float topy = ty0 * ns + ty1*s;

	float bottomx = tx2 * ns + tx3*s;
	float bottomy = ty2 * ns + ty3*s;

	*tx_out = nt * topx + t * bottomx;
	*ty_out = nt * topy + t * bottomy;

}

//generates an asteroid mesh
gx_mesh_t* gen_asteroid_mesh(gx_vbuffer_t* v, int isize)
{
	gx_mesh_t* m;
	
	
	int i;
	int j;
	int k;

	#define STEPS 10

	sheet_t*	top = NULL;
	sheet_t*	bottom = NULL;
	sheet_t*	left = NULL;
	sheet_t*	right = NULL;
	sheet_t*	front = NULL;
	sheet_t*	back = NULL;
	sheet_t*	sheets[6];

	
	vec3 zero;
	vec3 sizer;
	
	
	
	vec3* position = &zero;
	vec3* size = &sizer;

	vec3set(zero,0,0,0);

	vec3set( *size, isize, isize,isize);

	sheets[0] = top = sheet_quad_mk( v, STEPS, STEPS);
	sheets[1] = bottom = sheet_quad_mk( v, STEPS, STEPS);
	sheets[2] = left = sheet_quad_mk( v, STEPS, STEPS);
	sheets[3] = right = sheet_quad_mk( v, STEPS, STEPS);
	sheets[4] = front = sheet_quad_mk( v, STEPS, STEPS);
	sheets[5] = back = sheet_quad_mk( v, STEPS, STEPS);

#define RND .2


	//create the vertices

	for (i=0;i<STEPS;i++)
	{
		for (j=0;j<STEPS;j++)
		{

			vec3 p;

			float ls = i/(float)(STEPS-1);
			float lt = j/(float)(STEPS-1);
			
			float s;
			float t;
#if 1
			//BOTTOM

			s=0;
			t=0;
			
			interpolate_tex( &s, &t, ls, lt,  1, .5, .5,0  , 1.5,-00, 1, -.5);


			vec3set(p,	(i/(float)(STEPS-1))-.5,		-.5,						(-j/(float)(STEPS-1)) +.5 );
			vec3norm(&p);
			gx_vbuffer_add_normal(v, p.vec3x, p.vec3y, p.vec3z);
			vec3scale(p, 1 + randfs()*RND);
			p.vec3x *= size->vec3x; p.vec3y *= size->vec3y;p.vec3z *= size->vec3z;
			vec3add(p, *position);
			gx_vbuffer_add_tex(v, 0,  s, t);
			sheet_set_at( bottom, i, j, gx_vbuffer_add_vertexv(v, p));
#endif

			//TOP 
 			interpolate_tex( &s, &t, ls, lt,  .5 ,.33, .66, .50 , .33, .50, .5,.66 );
			printf("%f %f :  %f %f\    %d  %d  ", ls, lt , s, t, i, j);
			
			vec3set(p,  (i/(float)(STEPS-1))-.5,		+.5,						(-j/(float)(STEPS-1)) +.5 );
			vec3norm(&p);
			gx_vbuffer_add_normal(v, p.vec3x, p.vec3y, p.vec3z);
			vec3scale(p, 1 +randfs()*RND);
			vec3add(p, *position);
			vec3print( p);
			printf("\n");
			gx_vbuffer_add_tex(v, 0,  s, t);
			p.vec3x *= size->vec3x; p.vec3y *= size->vec3y;p.vec3z *= size->vec3z;
			sheet_set_at(top, i, j, gx_vbuffer_add_vertexv(v, p));
	s=0;
			t=0;
			
#if 1
			//LEFT

			interpolate_tex( &s, &t, ls, lt, .5,0, .5,.33, 0,.5, .33,.5 );

			vec3set(p,	-.5,							(i/(float)(STEPS-1))-.5,	(-j/(float)(STEPS-1)) +.5 );
			vec3norm(&p);
			gx_vbuffer_add_normal(v, p.vec3x, p.vec3y, p.vec3z);
			vec3scale(p, 1 +randfs()*RND);
			vec3add(p, *position);
			p.vec3x *= size->vec3x; p.vec3y *= size->vec3y;p.vec3z *= size->vec3z;
			gx_vbuffer_add_tex(v, 0,  s, t);
			sheet_set_at(left, i, j, gx_vbuffer_add_vertexv(v, p));
s=0;t=0;
			//RIGHT
			interpolate_tex( &s, &t, ls, lt, 1,.5, .66,.5, .5,1, .5,.66 );
			vec3set(p,	+.5,							(i/(float)(STEPS-1))-.5, 	(-j/(float)(STEPS-1)) +.5 );
			vec3norm(&p);
			gx_vbuffer_add_normal(v, p.vec3x, p.vec3y, p.vec3z);
			vec3scale(p, 1+randfs()*RND);
			vec3add(p, *position);
			p.vec3x *= size->vec3x; p.vec3y *= size->vec3y;p.vec3z *= size->vec3z;
			gx_vbuffer_add_tex(v, 0,  s, t);
			sheet_set_at(right, i, j, gx_vbuffer_add_vertexv(v, p));

			//FRONT
			interpolate_tex( &s, &t, ls, lt,.5,0, 1,.5, .5,.33, .66,.5 );
			vec3set(p,	(i/(float)(STEPS-1))-.5,		(j/(float)(STEPS-1)) -.5,	+.5);
			vec3norm(&p);
			gx_vbuffer_add_normal(v, p.vec3x, p.vec3y, p.vec3z);
			vec3scale(p, 1+randfs()*RND);
			vec3add(p, *position);
			p.vec3x *= size->vec3x; p.vec3y *= size->vec3y;p.vec3z *= size->vec3z;
			gx_vbuffer_add_tex(v, 0,  s, t);
			sheet_set_at(front, i, j, gx_vbuffer_add_vertexv(v, p));

			//BACK
			interpolate_tex( &s, &t, ls, lt,0,.5, .5,1, .33,.5, .5,.66);
			vec3set(p,	(i/(float)(STEPS-1))-.5,		(j/(float)(STEPS-1)) -.5,	-.5);		
			vec3norm(&p);
			gx_vbuffer_add_normal(v, p.vec3x, p.vec3y, p.vec3z);
			vec3scale(p, 1+randfs()*RND);
			vec3add(p, *position);
			p.vec3x *= size->vec3x; p.vec3y *= size->vec3y;p.vec3z *= size->vec3z;
			gx_vbuffer_add_tex(v, 0,  s, t);
			sheet_set_at(back , i, j,  gx_vbuffer_add_vertexv(v, p));
#endif
		}
	}

	//todo: sew the sheets here
	

	//connections to bottom
	sew_sheets( bottom, SHEET_EDGE_TOP, front, SHEET_EDGE_TOP, COPY_POSITION);
	sew_sheets( bottom, SHEET_EDGE_BOTTOM, back, SHEET_EDGE_TOP, COPY_POSITION);
	sew_sheets( bottom, SHEET_EDGE_LEFT, left, SHEET_EDGE_LEFT, COPY_POSITION);
	sew_sheets( bottom, SHEET_EDGE_RIGHT, right, SHEET_EDGE_LEFT, COPY_POSITION);


	//connections to top
	sew_sheets( top   , SHEET_EDGE_TOP,		front, SHEET_EDGE_BOTTOM, COPY_POSITION);
	sew_sheets( top   , SHEET_EDGE_BOTTOM,	back, SHEET_EDGE_BOTTOM, COPY_POSITION);
	sew_sheets( top   , SHEET_EDGE_LEFT,	left, SHEET_EDGE_RIGHT, COPY_POSITION);
	sew_sheets( top   , SHEET_EDGE_RIGHT,	right, SHEET_EDGE_RIGHT, COPY_POSITION);

	//sides
	
	sew_sheets( back   , SHEET_EDGE_LEFT,		left, SHEET_EDGE_BOTTOM, COPY_POSITION);
	sew_sheets( back   , SHEET_EDGE_RIGHT,		right, SHEET_EDGE_BOTTOM, COPY_POSITION);

	sew_sheets( front   , SHEET_EDGE_LEFT,		left, SHEET_EDGE_TOP, COPY_POSITION);
	sew_sheets( front   , SHEET_EDGE_RIGHT,		right, SHEET_EDGE_TOP, COPY_POSITION);



	//create triangles:

	for (i=1;i<STEPS;i++)
	{
		for (j=1;j<STEPS;j++)
		{

			for (k=0;k<6;k++)
			{
				gx_vbuffer_add_index( v, SHEET_POINT_AT(sheets[k], i-1, j-1));
				gx_vbuffer_add_index( v, SHEET_POINT_AT(sheets[k], i,   j-1));
				gx_vbuffer_add_index( v, SHEET_POINT_AT(sheets[k], i,   j  ));
				gx_vbuffer_add_index( v, SHEET_POINT_AT(sheets[k], i-1 , j ));
			}


		}
	}


	gx_vbuffer_update(v);

	
		
	//m = gx_mesh_def( v, NULL, NULL, v->vertex_count, zfalse);
	//m->prim = gx_points;
	
	
	m = gx_mesh_def( v, NULL, NULL, v->index_count, ztrue);
	m->prim = gx_quads;
	

	return m;
}

//game structures

typedef struct entity_3d_s
{
	gx_mesh_t*    mesh;
	gx_sprite_t*  sprite;
	vec3 position;
	vec3 speed;

	vec3 x33;
	vec3 y33;
	vec3 z33;

	float xspin;
	float yspin;
	float zspin;

	float radius;  //radius for collision detection

} entity_3d_t;


//game constants
#define STARCOUNT 2000
#define INITACOUNT 200
#define PLAYER_RADIUS 1.0

//objects are restricted to +/- playzone coordinates
#define PLAYZONE  2000
#define SAFEZONE  50


//create 'count' asteroids at given size at position, each with random speed
entity_3d_t* gen_asteroid(float size,float speed,  vec3* position, gx_mesh_t* mesh , gx_image_t* spriteimage)
{
	entity_3d_t* asteroid = ram_alloc(sizeof(entity_3d_t), NULL);
	
	if (!asteroid) 
		return NULL;


	asteroid->mesh = mesh;



	vec3mov(asteroid->position, *position);

	vec3set( asteroid->speed , randfs()*speed ,randfs()*speed , randfs()*speed); 

	vec3set( asteroid->x33 , 1, 0,0);
	vec3set( asteroid->y33 , 0, 1,0);
	vec3set( asteroid->z33 , 0, 0,1);

	asteroid->xspin = randfs()*.005;
	asteroid->yspin = randfs()*.005;
	asteroid->zspin = randfs()*.005;
			
	asteroid->radius = size; 


	//now specify the sprite
	asteroid->sprite = gx_sprite_mk( spriteimage, 0, 0, spriteimage->width, spriteimage->height, size*2, size*2);

	return asteroid;

}

gx_mesh_t* load_spaceship_mesh()
{

	gx_image_t* tex = NULL;

	gx_image_t* heightmap = NULL;
	gx_vbuffer_t* heightbuffer = NULL;

	gx_image_t* heightmap2 = NULL;
	gx_vbuffer_t* heightbuffer2 = NULL;


	gx_vbuffer_t* normbuffer = NULL;

	gx_mesh_t *mesh0, *mesh1;

	zint32 start, end;



	normbuffer = gx_vbuffer_mk( 2*512*512*4, 2*3*512*512*4, zfalse, ztrue, 2);

	heightmap = gx_image_load_tga("shipheight_top.tga");


	heightbuffer = gx_vbuffer_from_image( normbuffer, heightmap, -0.5,0.0,-0.5,   //offset
		0,1,2,        //axis swizzle
		1.0,.2,1.0,  //scaling
		zfalse, 2, &start, &end, zfalse);


	mesh0 = gx_mesh_def( heightbuffer, NULL, start, end, ztrue);


	heightmap2 = gx_image_load_tga("shipheight_bottom.tga");

	heightbuffer2 = gx_vbuffer_from_image(normbuffer, heightmap2, -0.5,0.0,-0.5,   //offset
		0,1,2,        //axis swizzle
		1.0,-.1,1.0,  //scaling
		zfalse, 2, &start, &end, ztrue);


	gx_vbuffer_update(heightbuffer);
	if (heightbuffer2 != heightbuffer)
		gx_vbuffer_update(heightbuffer2);


	mesh1 = gx_mesh_def( heightbuffer2, NULL, start, end, ztrue);


	mesh0->next = mesh1;


	tex = gx_image_load_tga("spaceship_texture.tga");
	{
		gx_drawstyle_t* ds = ram_alloc(sizeof(gx_drawstyle_t), NULL);
		gx_drawstyle_t* dsbottom = ram_alloc(sizeof(gx_drawstyle_t), NULL);
		
		vec3set( ds->specular_color, 1, 1, 1);
		ds->specular_exponent = 10;

		memcpy(dsbottom, ds, sizeof(*ds));

		ds->numtextures = 1;
		ds->textures = ram_alloc(sizeof(gx_image_t**), NULL);				
		ds->textures[0] = tex;
		mesh0->style = ds;
		mesh1->style = dsbottom;
		
	}


	return mesh0;
}

int main(int argc, char** argv)
{
	int bullet_i = 0;
	int bullet_timer =0;

	// IO variables
	zchar keypress=0;
	zint32 mouse_x=0;
	zint32 mouse_y=0;
	zbool  mouse_relative=zfalse;

	//game/graphics variables
	g_camera_t	player_camera;
	vec3		camera_inertia;

	zfloat32 delta_yaw		= 0.0;
	zfloat32 delta_pitch	= 0.0;
	zfloat32 delta_roll		= 0.0;


	//need a light source
	gx_light_t*	sunlight = NULL;
	gx_light_t* lights[4];  //4 lights
	int			num_lights=0;

	//need a starfield (we ARE in space)
	gx_vbuffer_t*	starfield = NULL;

	vec_t			asteroids; //hold entities

	gx_image_t		*spacerock = NULL;  //holds rock texture for asteroids
	gx_drawstyle_t	spacerock_ds;   //drawstyle for the asteroid

	gx_image_t		*bullet_image = NULL;  //holds bullet texture

	gx_image_t		*asteroid_imposter_img = NULL;
		
	gx_vbuffer_t*	asteroid_vb = NULL;  //vbuffer to hold the asteroid mesh
	gx_mesh_t * asteroid_mesh = NULL;

	
	//need an enemy spaceship
	gx_mesh_t * spaceship_mesh = NULL;
	//vec3		spaceship_pos;
	g_camera_t	spaceship_camera;

	//need a bullet
	vec3	bullet_position[100];
	vec3    old_bullet_position[100];
	vec3	bullet_speed[100];
	int		num_bullets=0;

	float bullet_ang=0;
	//zbool	bullet_valid = zfalse;
	gx_sprite_t* bullet_sprite = NULL;

	//the sun
	gx_image_t* sun_image = NULL;
	gx_sprite_t* sun_sprite = NULL;

	//Initialize graphics
	gx_init(1270, 1000 , "Test");
	//gx_init(640, 480 , "Test");
	gx_clear_color(0,0,0,1);
	gx_mouse_capture(ztrue);  //mouse input will be relative 
	
	//Load assets
	spacerock  = gx_image_load_tga( "spacerock.tga");
	//spacerock  = gx_image_load_tga( "unwrapped_cube.tga");
	spacerock_ds.textures = &spacerock;
	spacerock_ds.numtextures=1;
	vec3set( spacerock_ds.specular_color , 0, 0, 0);
	spacerock_ds.specular_exponent = 10;


	bullet_image = gx_image_load_tga( "shot.tga");

	asteroid_imposter_img  = gx_image_load_tga( "asteroid_imposter.tga");

	bullet_sprite = gx_sprite_mk(bullet_image, 0.0,0.0, bullet_image->width ,bullet_image->height,50,50);


	sun_image  = gx_image_load_tga( "sun.tga");

	sun_sprite = gx_sprite_mk(sun_image, 0.0,0.0, sun_image->width ,sun_image->height,50,50);


	//get spaceship mesh
	spaceship_mesh = load_spaceship_mesh();
	//vec3set(spaceship_pos, 10,0,-50);
	g_camera_init( & spaceship_camera);
	//spaceship_camera.camera_forward.named.z=1;
	vec3set( spaceship_camera.camera_pos, 10,0,-60);


	//initialize game data
	g_camera_init(&player_camera);
	vec3set(camera_inertia, 0,0,0);
	
	//make a vbuffer to hold the asteroid mesh
	asteroid_vb = gx_vbuffer_mk(65535,  65535,zfalse, ztrue, 1);

	//create an asteroid mesh
	asteroid_mesh = gen_asteroid_mesh( asteroid_vb, 1);	
	asteroid_mesh->style = &spacerock_ds;

	vec_mk(&asteroids, INITACOUNT);
	//create asteroids
	{
		int i;
		vec3 p;
		
		entity_3d_t * asteroid = NULL;

		vec3set(p, 0, 1, 0);

		for (i=0;i<INITACOUNT;i++)
		{
			vec3set(p,  randfs()*PLAYZONE, randfs()*PLAYZONE, randfs()*PLAYZONE);
			asteroid = gen_asteroid( 50 ,1,  &p, asteroid_mesh, asteroid_imposter_img);
			
			vec_add(&asteroids, asteroid)			;
		}


		
	}


	//create light source
	{	
		vec3 lightpos;
		vec3 lightcolor;
		vec3 lightambient;
		vec3set (lightpos, 1, 0, 0 );
		vec3set (lightcolor, .8, .8, .8);
		vec3set (lightambient, 0, .1, .1);
		lights[0] = sunlight = gx_light_mk( gx_light_directional, &lightpos, &lightcolor, &lightambient);
		

		//have 3 other lights on 'standby'
		vec3set (lightcolor, .2, .2, .05);
		vec3set (lightambient, 0, 0, 0);
		lights[1] = gx_light_mk(gx_light_point, &lightpos, &lightcolor, &lightambient);
		lights[2] = gx_light_mk(gx_light_point, &lightpos, &lightcolor, &lightambient);
		lights[3] = gx_light_mk(gx_light_point, &lightpos, &lightcolor, &lightambient);
	}


	//create stars

	starfield = gx_vbuffer_mk(STARCOUNT,0,ztrue, zfalse, 0);		
	{
		int i;  
		for (i=0;i<STARCOUNT;i++)
		{
			vec3 p;
			float s;

			vec3set(p, randf()-.5,randf()-.5,randf()-.5);
			
			//normalize
			
			s=sqrt(vec3abs_sq(p));
			vec3scale( p, ( 1.0/s )  );

			gx_vbuffer_add_color(starfield, .7+.3*randf(),.7+.3*randf(),.7+.3*randf(),1);
			gx_vbuffer_add_vertex(starfield, p.vec3x, p.vec3y, p.vec3z);

		}
	}
	gx_vbuffer_update(starfield);



	//The game loop
	for(;;)  
	{
		gx_window_event();  //handles any window events (I/O)

		//set up projection matrix for this frame
		gx_setup_3d( 70.0f,  gx_frame_get_dimensions(NULL,NULL), .1f, 50000.0f);
		
		//Read mouse input
		gx_mouse_pos(&mouse_x, &mouse_y, &mouse_relative);
		if (!mouse_relative)
		{
			//if some some reason we get an absolute mouse position, clear it out
			mouse_x=0;
			mouse_y=0;
		}
	
	
		bullet_timer--;

		if (gx_mouse_state(GX_MOUSE_LEFT) && bullet_timer < 0 )
		{
			int bullet_i = num_bullets;
			if (num_bullets ==100)
			{		
				bullet_i = rand() % 100;
			}


			bullet_timer = 5; 
			//printf("FIRE!\n");
			vec3mov( bullet_position[bullet_i] , player_camera.camera_pos);

			

			//vec3mov( bullet_speed, camera_inertia);
			//vec3set(bullet_speed, 0,0,0);
			
			vec3mov (bullet_speed[bullet_i],player_camera.camera_forward);

			//vec3print(bullet_speed); printf( " bullet speed\n");
			vec3madd(bullet_speed[bullet_i],  sqrt( vec3abs_sq(camera_inertia))  +3, player_camera.camera_forward);

			if (num_bullets < 100)
				num_bullets++;
		}


		//read keyboard input
		keypress = gx_getkey();  //reads decoded ascii chars
		
		if (keypress=='m')
		{
			//toggle the mouse capture
			gx_mouse_capture( mouse_relative ^ 1);
		}
		else if (keypress=='~')
		{
			break;
		}
		else if (keypress=='(')
		{
			g_toggle0 ^= ztrue;
		}


		//camera control
		{
			//zfloat32 delta_yaw		= 0.0;
			//zfloat32 delta_pitch	= 0.0;
			//zfloat32 delta_roll		= 0.0;
		
			vec3	 delta_pos;
			vec3set	 (delta_pos, 0,0,0);
	
			if (gx_key_state(' ')) {
				vec3set(camera_inertia, 0,0,0);
				delta_pitch=0;
				delta_roll=0;
				delta_yaw=0;
			}

			if (gx_key_state('w')) delta_pos.vec3z+=.9;
			if (gx_key_state('s')) delta_pos.vec3z=-.9;
			if (gx_key_state('a')) delta_pos.vec3x=-.9;
			if (gx_key_state('d')) delta_pos.vec3x=+.9;
			if (gx_key_state('r')) delta_pos.vec3y=+.9;
			if (gx_key_state('f')) delta_pos.vec3y=-.9;

			if (gx_key_state('q')) delta_roll-=.002;
			if (gx_key_state('e')) delta_roll+=.002;

		//	if (gx_key_state('4')) delta_yaw=-.02;
		//	if (gx_key_state('6')) delta_yaw=.02;

		//	if (gx_key_state('8')) delta_pitch=-.02;
		//	if (gx_key_state('2')) delta_pitch=.02;

			//move camera using camera's basis
			vec3madd( camera_inertia, delta_pos.vec3x, player_camera.camera_right);
			vec3madd( camera_inertia, delta_pos.vec3y, player_camera.camera_up);
			vec3madd( camera_inertia, delta_pos.vec3z, player_camera.camera_forward);

			vec3madd( player_camera.camera_pos, .1 , camera_inertia);
			
			printf("---\n");
			vec3print( player_camera.camera_right); printf("\n");
			vec3print( player_camera.camera_up); printf("\n");
			vec3print( player_camera.camera_forward); printf("\n");
			


			//lets spin camera  (relative to its own coord system)

			delta_pitch += mouse_y*.0003;
			delta_yaw += mouse_x*.0003;

			gx_spin(ztrue, delta_yaw, delta_pitch, delta_roll,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);

		}

		//clear screen		
		gx_frame_clear(ztrue,ztrue);
		gx_set_active_lights(NULL, 0);  //no lighting activated
		
		//draw starfield
		gx_camera_home();
		gx_camera_pos_rot(NULL,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward); 
		
		gx_zbuffer(zfalse);
		
		gx_drawstyle_activate(NULL) ;
		gx_vbuffer_draw(starfield, 0, starfield->vertex_count, gx_points, zfalse);

		//draw sun
		{

			vec3 sun_right;
			vec3 sun_up;
			vec3 sunpos;
			vec3set(sunpos, 100, 00, 0);
			vec3set(sun_right, 0,0,1);
			vec3set(sun_up, 0,1,0);


			gx_sprite_draw_3d(sun_sprite, &sunpos, &sun_up, &sun_right, ztrue, ztrue);

		}


		gx_zbuffer(ztrue);
		//render here

		gx_camera_home();
		gx_camera_pos_rot( &player_camera.camera_pos,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);
		
		gx_home();
		
		
	
		
		//gx_debug_show_light( light, 20.0);

		num_lights=1;
		{
			int i = num_bullets-1;
			while ( i >= 0 && num_lights < 4)
			{
				vec3mov(lights[1]->position, bullet_position[num_bullets-1]);
				num_lights++;
				i--;
			}
		}
		gx_set_active_lights(lights, num_lights);		

	

		//control spaceship movement// try to keep a certain distance from player
		if (1) {
			

			vec3 ship_to_player;

			float d;

			vec3mov(ship_to_player, player_camera.camera_pos);
			
			vec3sub(ship_to_player, spaceship_camera.camera_pos);
			
			d = sqrt(vec3abs_sq( ship_to_player));

			//ok normalize as well
			vec3norm(&ship_to_player);

			if (d> 400)
			{
				vec3madd(spaceship_camera.camera_pos, d/200, ship_to_player);
			}
			else 
			{
				vec3madd(spaceship_camera.camera_pos, -d/200, ship_to_player);
			}

			//spaceship should turn to face player

			{
				float dot_x;
				float dot_y;

				dot_x = vec3dot( spaceship_camera.camera_right, ship_to_player);
				dot_y = vec3dot( spaceship_camera.camera_up, ship_to_player);
				

				
									
				gx_spin( ztrue, dot_x*.01 , dot_y*.01 , 0, &spaceship_camera.camera_right, &spaceship_camera.camera_up, &spaceship_camera.camera_forward);
		
			}
			//keep roll relative to player the same (aestetic reasons)

			{
				gx_spin( ztrue, 0 , 0 ,  .01 *  (  vec3dot(spaceship_camera.camera_up,  player_camera.camera_right) )  , &spaceship_camera.camera_right, &spaceship_camera.camera_up, &spaceship_camera.camera_forward);
			}

			//try to evade player
			{

				float dot_px, dot_py;
				int s;

				dot_px = vec3dot(player_camera.camera_right, ship_to_player);
				
				if (dot_px> 0)
					s = -2;
				else s=2;
				
				vec3madd( spaceship_camera.camera_pos, s , player_camera.camera_right);



				dot_py = vec3dot(player_camera.camera_up, ship_to_player);
				
				if (dot_py> 0)
					s = -2;
				else s=2;
				
				vec3madd( spaceship_camera.camera_pos, s , player_camera.camera_up);

			
				

			}
			
			//ok i guess we need to shoot at the player
			if (rand() % 1000 < 10)
			{
			int bullet_i = num_bullets;
			if (num_bullets ==100)
			{		
				bullet_i = rand() % 100;
			}


			
			
			vec3mov( bullet_position[bullet_i] , spaceship_camera.camera_pos);

			
	
			
			vec3mov (bullet_speed[bullet_i],spaceship_camera.camera_forward);

			
			vec3madd(bullet_speed[bullet_i],  sqrt( vec3abs_sq(camera_inertia))  , spaceship_camera.camera_forward);

			if (num_bullets < 100)
				num_bullets++;
			

			}

		}


		//draw spaceship
		
		gx_move3d(&spaceship_camera.camera_pos);
		gx_rotate_3x3( &spaceship_camera.camera_right, &spaceship_camera.camera_up, &spaceship_camera.camera_forward);
		
		
		gx_scale(200.0);
		
		gx_mesh_draw(spaceship_mesh);
		gx_home();

		//draw asteroids

		{
			int i;
			for (i=0;i<vec_count(&asteroids);i++)
			{	
				float xx;
				float yy;
				float zz;
				vec3 pp;

				float xs=0 ;
				float ys= 0;
				float zs= 0;

				float xe=0;
				float ye=0;
				float ze=0;
	
				entity_3d_t* aster = vec_get_at(&asteroids, i);

				

				//setup transform and draw


				
				

				vec3mov (pp, player_camera.camera_pos);
				vec3sub (pp, aster->position);
			//	if (vec3abs_sq(pp) > (3000*3000))
				{
				//	vec3 zero;
				//	vec3set(zero,0,0,0);
					//gx_sprite_draw_3d( aster->sprite, &aster->position, &player_camera.camera_up, &player_camera.camera_right, ztrue, ztrue);
				}
			//	else
				{
					
					gx_home();
					gx_move3d( &aster->position);
					gx_scale( aster->radius);
					gx_rotate_3x3( &aster->x33, &aster->y33, &aster->z33);
					gx_mesh_draw(aster->mesh);
					gx_home();
				}

				//draw all 'copies' of asteroid
				{
					int i;
					int j;
					int k;
						vec3 p;
					for (i=-2;i<=2;i++)
					{

						for (j=-2;j<=2;j++)
						{

							for (k=-2;k<=2;k++)
							{

								if (!i && !j && ! k)
									continue;

								vec3mov(p, aster->position);
								p.vec3x += i * PLAYZONE*2;
								p.vec3y += j * PLAYZONE*2;
								p.vec3z += k * PLAYZONE*2;


							//	gx_sprite_draw_3d( aster->sprite, &p, &player_camera.camera_up, &player_camera.camera_right, ztrue, ztrue);
							}
						}
					}

				}
			

				
				
				
				//move asteroid
				vec3add(aster->position, aster->speed);


				//restriction motion 
				if (aster->position.vec3x < -PLAYZONE + player_camera.camera_pos.vec3x) aster->position.vec3x+= PLAYZONE*2;
				if (aster->position.vec3y < -PLAYZONE + player_camera.camera_pos.vec3y) aster->position.vec3y+= PLAYZONE*2;
				if (aster->position.vec3z < -PLAYZONE + player_camera.camera_pos.vec3z) aster->position.vec3z+= PLAYZONE*2;
				if (aster->position.vec3x >  PLAYZONE+ player_camera.camera_pos.vec3x) aster->position.vec3x-= PLAYZONE*2;
				if (aster->position.vec3y >  PLAYZONE+ player_camera.camera_pos.vec3y) aster->position.vec3y-= PLAYZONE*2;
				if (aster->position.vec3z >  PLAYZONE+ player_camera.camera_pos.vec3z) aster->position.vec3z-= PLAYZONE*2;



				//spin asteroid
			//	if (g_toggle0)
			//	{
					gx_spin(zfalse, aster->zspin, aster->xspin, aster->yspin, &aster->x33, &aster->y33, &aster->z33);
					//gx_spin(zfalse, 0.00, 0.0, 0.01, &aster->x33, &aster->y33, &aster->z33);
			//	}

				
				//do collision detect against player

				{
					vec3 diff;
					float dist=0;

					vec3mov(diff, aster->position);
					vec3sub(diff, player_camera.camera_pos);
					dist = sqrt(vec3abs_sq(diff));

					if (dist < (aster->radius+ PLAYER_RADIUS) )
					{
						vec3scale(camera_inertia, -1);  //reverse player camera
						

					//	camera_inertia.named.x
					}
				
				}

			}
		}

		//bullets

		gx_set_active_lights(NULL, 0);

		bullet_ang +=  .2;

		for (bullet_i=0;bullet_i < num_bullets; bullet_i++)
		{
			int i;

			gx_home();
			vec3mov(old_bullet_position[bullet_i], bullet_position[bullet_i]);
			vec3add(bullet_position[bullet_i], bullet_speed[bullet_i]);
			
			//gx_zbuffer(zfalse);
			
			//for (i=0;i<10;i++)
			{

				vec3 cu;
				vec3 cr;

				vec3set(cr, 0,0,0);
				vec3madd(cr, cos(bullet_ang), player_camera.camera_right);
				vec3madd(cr, sin(bullet_ang), player_camera.camera_up);


				vec3set(cu, 0,0,0);
				vec3madd(cu, cos(bullet_ang), player_camera.camera_up);
				vec3madd(cu, -sin(bullet_ang), player_camera.camera_right);



				gx_sprite_draw_3d( bullet_sprite, &bullet_position[bullet_i], &cu, &cr, ztrue, ztrue);

			}
			//gx_zbuffer(ztrue);

			//vec3print(bullet_position); printf( " bullet position\n");

			
			//ok lets kill asteroids now


			for (i=0;i<vec_count(&asteroids);i++)
			{
				
				float dist;
				float t;
				vec3 p;
				
				entity_3d_t* e = vec_get_at(&asteroids, i);
				
				for (t = 0;t<1; t+=.1)
				{
					vec3set(p, 0,0,0);
					vec3madd( p, t, bullet_position[bullet_i]);
					vec3madd( p, 1-t, bullet_position[bullet_i]);

					vec3sub( p, e->position);



					if (  (e->radius+2.0) >=  sqrt( vec3abs_sq(p) ))
					{
						float s = e->radius;
						
						int newsize = e->radius /2;
						float newspeed = 10-newsize;
						newspeed = newspeed * .1;

						if (newsize >= 2)
						{

							int k;
							for (k=0;k<2;k++)
							{
								vec_add( &asteroids, gen_asteroid( newsize,newspeed, &e->position, asteroid_mesh, asteroid_imposter_img));
							}

						}

 						ram_free(vec_remove_unordered(&asteroids, i));
						
						bullet_position[bullet_i] = bullet_position[num_bullets-1];
						old_bullet_position[bullet_i] = old_bullet_position[num_bullets-1];
						bullet_speed[bullet_i] = bullet_speed[num_bullets-1];
						

						num_bullets--;

						break;
					}

				}




			}
		}

		
		gx_frame_show();  //show the frame
	}

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 