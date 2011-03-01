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

	//draw the sector outline
	gx_sector_outline(sector);
	
	

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
			if (cosang > cos( 3.14159/4.5 ))
			{
				visible = ztrue;
			}
		}

		if (visible)
		{
			gx_portal_draw_test(p);
			draw_sector(p->target, camera_position, camera_look, camera_sector);
		}

		p=p->next_portal;
	}

	return new_camera_sector;
}

void graphtest_main()
{

	zfloat32 cx=0;
	zfloat32 cy=.5;
	zfloat32 cz=1;

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


	gx_mesh_t* objmesh = NULL;
	gx_image_t* objmeshtex = NULL;

	gx_light_t* lights[8];
	

	{
		vec3 pos;
		vec3 color;
		vec3 ambient;
		vec3set(pos, 1,.5,0);
		vec3set(color, 1, 1, 1);
		vec3set(ambient, 0,.1,0);

		lights[0] = gx_light_mk(gx_light_point, &pos, &color, &ambient);
		
		vec3set(pos, .5,0,0);
		vec3set(color, 0, 1, 0);
		lights[1] = gx_light_mk(gx_light_point, &pos, &color, &ambient);
	}

	printf("Init graphics\n");
	gx_init(1280, 1024 , "Test Graphics Window");

	gx_clear_color(0,0,0,1);
	gx_frame_clear(ztrue,ztrue);
	gx_frame_show();

	objmeshtex  = gx_image_load_tga( "E:\\mark\\projects\\projectZ\\meshtexture.tga");
	objmesh		= gx_mesh_load_obj(NULL, "E:\\mark\\projects\\projectZ\\mesh.obj", NULL);

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



	image1 = gx_image_load_tga( "rgbatarga.tga");
	image2 = gx_image_load_tga( "tex2.tga");
	
	
	//gx_image_enable(image1);  //don't need to enable because first use will

	vec3set(camera_pos,		0,	.5,	1);
	vec3set(camera_right,	1,	0,	0);
	vec3set(camera_up,		0,	1,	0);
	vec3set(camera_forward,	0,	0,	-1);

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

	heightmap = gx_image_load_tga("heightmap.tga");

	sharebuffer = gx_vbuffer_mk( 256*256*4, 3*256*256*4, ztrue, zfalse, 2);

	normbuffer = gx_vbuffer_mk( 256*256*4, 3*256*256*4, zfalse, ztrue, 2);


	heightbuffer = gx_vbuffer_from_image( normbuffer, heightmap, 0.0,0.0,-1.0,   //offset
		0,1,2,        //axis swizzle
		1.0,.1,2.0,  //scaling
		zfalse, 2, &start, &end);

//	

	mesh0 = gx_mesh_def( heightbuffer, &ds1, start, end, ztrue);
	
	

	heightmap2 = gx_image_load_tga("hmap.tga");

	heightbuffer2 = gx_vbuffer_from_image(sharebuffer, heightmap2, 0.0,0.0,-3.0,   //offset
		0,1,2,        //axis swizzle
		1.0,.1,2.0,  //scaling
		ztrue, 2, &start, &end);


	gx_vbuffer_update(heightbuffer);
	if (heightbuffer2 != heightbuffer)
		gx_vbuffer_update(heightbuffer2);


	mesh1 = gx_mesh_def( heightbuffer2, &ds2, start, end, ztrue);

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

	gx_mouse_capture(ztrue); //capture the mouse for relative motion



	while( 1)
	{
	

		zint32 mx, my;
		zbool rel;

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
	
		gx_setup_3d( 70.0, gx_frame_get_dimensions(NULL,NULL), .01, 1000);

		

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
			if (gx_key_state('e')) roll=.02;

			if (gx_key_state('4')) yaw=-.02;
			if (gx_key_state('6')) yaw=.02;

			if (gx_key_state('8')) pitch=-.02;
			if (gx_key_state('2')) pitch=.02;

			c = gx_getkey();
			if (c=='Q') 
				break;
			
			if (c=='m')
				gx_mouse_capture(zfalse);
				
			

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
			gx_spin(yaw, pitch, roll,&camera_right, &camera_up, &camera_forward);

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
			vec3set(vv, .5,.5,-1.5);

			ds1.blending = ztrue;
			ds1.numtextures = 1;
			ds1.textures = &objmeshtex;
			
			ds1.specular_color.array[0]=0;
			ds1.specular_color.array[1]=0;
			ds1.specular_color.array[2]=1;

			ds1.specular_exponent=100;

			//ds1.textures = &image1;

			gx_drawstyle_activate(&ds1);

			gx_set_active_lights(lights, 1);
			gx_set_active_textures( &objmeshtex  , 1);

			test_lighting_on();
			lights[0]->position.named.z -=.001;
			lights[0]->position.named.x -=.001;
			
			
		
				
			gx_mesh_draw_at(objmesh, &vv);
			
				
			gx_set_active_textures(NULL,0);
			gx_mesh_draw(mesh0);
			
			gx_set_active_lights(lights, 0); //no light
			
		
			
	

			//test_lighting_off();

			gx_set_active_textures(NULL, 0);
			gx_debug_show_light(lights[0], .1);


		}

		//draw camera's sector

		
		camera_sector = draw_sector(camera_sector, &camera_pos, &camera_forward, camera_sector );
		
		
		

		gx_frame_show();
	}

}


// need basic 2d entity management

#define SHIP 0
#define ASTER 1
#define BULLET 2

typedef struct entity2_s
{
	zfloat32 x;
	zfloat32 y;

	zfloat32 ox;
	zfloat32 oy;

	zfloat32 xspeed;
	zfloat32 yspeed;

	zfloat32 rotation;	
	zfloat32 rotation_speed;	

	zfloat32 size;
	
	gx_sprite_t* sprite;
	zint32 type;

} entity2_t;


void entity_update( entity2_t* e,zfloat32  minx,zfloat32  miny,zfloat32  maxx, zfloat32 maxy)
{
	if (!e)
		return;



	e->x += e->xspeed;
	e->y += e->yspeed;

	if (e->x > maxx) e->x = minx ;
	if (e->y > maxy) e->y = miny ;
	
	if (e->x < minx) e->x = maxx ;
	if (e->y < miny) e->y = maxy ;
	
	e->rotation += e->rotation_speed;
}


#define MAX_ENT 100
#define START_ENTS 5

float ent_dist(entity2_t*  e, entity2_t*  f )
{
	return   sqrt(    (e->x - f->x)* (e->x - f->x) + (e->y - f->y)*(e->y - f->y)        );

}

void graphtest_main2()
{
	gx_image_t* image1 = NULL;
	
	zfloat32 a=0;
	
	//sprites
	gx_sprite_t* ship[3];
	gx_sprite_t* aster[2];
	gx_sprite_t* bullet=NULL;
	
	int start_count=8;

	int add_asteroids=0;
	float add_asteroids_size=0;
	float killed_x;
	float killed_y;
	float killed_xs;
	float killed_ys;

	int nodamping=0;

	int shipstate=0;
	int bullet_time = 0;

	//simple game data:
	entity2_t ents[MAX_ENT]; // up to MAX entities
	int  bullet_index=-1;
		
	int kill_bullet=0;
	int active_ents = 1;

	int i;

	printf("Init graphics\n");
	gx_init(640, 480 , "Test Graphics Window");


	ram_clear(ents, sizeof(ents));


	//set up some data
	image1 = gx_image_load_tga( "asteroids.tga");

	ship[0] = gx_sprite_mk(image1,0,64, 64, 64, .1, .1);
	ship[1] = gx_sprite_mk(image1,64,64, 64, 64, .1, .1);
	ship[2] = gx_sprite_mk(image1,128,64, 64, 64, .1, .1);

	aster[0] = gx_sprite_mk(image1,128+64,64, 64, 64, .3, .3);
	aster[1] = gx_sprite_mk(image1,0,0, 64, 64, .3, .3);

	bullet = gx_sprite_mk (image1, 32,32 ,3,3,.015,.015);


	ents[0].sprite = ship[0];
	ents[0].ox=.05;
	ents[0].oy=.05;
	ents[0].size = .05;

	for(i=1;i<start_count;i++)
	{
		ents[i].x=  ((rand()&15)-8)/8.0  ;
		ents[i].y=((rand()&15)-8)/8.0;
		ents[i].xspeed=((rand()&15)-8)/8.0 *.005 ;
		ents[i].yspeed=((rand()&15)-8)/8.0 *.005;
		ents[i].rotation=0;
		//ents[i].	
		ents[i].sprite=  aster[i & 1] ;
		ents[i].ox= .15;
		ents[i].oy= .15;
		ents[i].type = ASTER;
		ents[i].size = .15;
		
	}
	active_ents = start_count;

	gx_clear_color(0,0,0,1);
	
	gx_frame_clear(ztrue,ztrue);

	while( 1)
	{
		zchar c ;

		gx_window_event();

		c= gx_getkey();

		if (c=='Q') break;

		if (active_ents==1)
		{
			//only player remains

				add_asteroids=++start_count;
				add_asteroids_size= .15 ;


		}

		if (c==' ' && (bullet_index == -1 ))
		{

			ents[active_ents].x=ents[0].x;
			ents[active_ents].y=ents[0].y;

		
			ents[active_ents].xspeed = .03 * cos( ents[0].rotation / 180.0 *3.14159) + ents[0].xspeed*.1;
			ents[active_ents].yspeed = .03 * sin( ents[0].rotation / 180.0 *3.14159) + ents[0].yspeed*.1;

			ents[0].xspeed -= .005 * cos( ents[0].rotation / 180.0 *3.14159) ;
			ents[0].yspeed -= .005 * sin( ents[0].rotation / 180.0 *3.14159) ;

			ents[active_ents].sprite = bullet;
			ents[active_ents].rotation=0;
			ents[active_ents].rotation_speed=0;
			ents[active_ents].type=BULLET;
			ents[active_ents].ox=0;
			ents[active_ents].oy=0;
			ents[active_ents].size=0;

			bullet_index = active_ents;
			bullet_time = 60;
			active_ents++;

		}

		if (gx_key_state('8'))
		{
			ents[0].xspeed += .001 * cos( ents[0].rotation / 180.0 *3.14159);
			ents[0].yspeed += .001 * sin( ents[0].rotation / 180.0 *3.14159);
			shipstate = (shipstate+1) & 16;
			if (shipstate > 8)
				ents[0].sprite = ship[1];
			else
				ents[0].sprite = ship[2];
		}
		else ents[0].sprite=ship[0];
		

		if (gx_key_state('4'))
		{
			ents[0].rotation_speed = 5;
			nodamping=0;
		}
		else if (gx_key_state('6'))
		{
			ents[0].rotation_speed =-5;
			nodamping=0;
		}
		else if (!nodamping)
		{
			ents[0].rotation_speed =0;
		}


		gx_frame_clear(ztrue, ztrue);

		//setup projection matrix for 2d
		gx_setup_2d(-1,1,1,-1);


		//do stupid gravity
		{
			float gravx=0;
			float gravy=0;
			float d=0;
			for (i=1;i<active_ents;i++)
			{
				d= ent_dist(ents, ents+i);

				if (d< ents[i].size)
				{  //hit player  need to treat like a bullet hit

					ents[0].xspeed += ents[i].xspeed*.5;
					ents[0].yspeed += ents[i].yspeed*.5;
					ents[0].rotation_speed = - ents[i].rotation_speed*10;
					nodamping=1;


					
					add_asteroids=1;
					add_asteroids_size=  ents[i].size/2;
					killed_x  = ents[i].x;
					killed_y  = ents[i].y;
					killed_xs  = ents[i].xspeed;
					killed_ys  = ents[i].yspeed;

					ents[i] = ents[active_ents -1];

					if (bullet_index == active_ents-1)
						bullet_index = i;

					active_ents--;
					i--;

				}
				
				//d=d*d*d*d;

				if (d>.1)
				{
			
					gravx=0;
					gravy=0;

					gravx +=  (ents[i].x - ents[0].x) / d ;
					gravy +=  (ents[i].y - ents[0].y) / d ;

					//asteroids to ship
					ents[i].xspeed -= .000005*gravx;
					ents[i].yspeed -= .000005*gravy;

				}




			}
			//ship to asteroids
			//ents[0].xspeed += gravx*.000005;
			//ents[0].yspeed += gravy*.000005;

		}


		for (i=0;i<active_ents;i++)
		{
			entity_update(ents+i, -1.15, -1.15, 1.15, 1.15); 


			//check for collisions
			if (i>0 && (i!= bullet_index) && (bullet_index >=0) )
			{
				if ( ent_dist( ents+bullet_index, ents+i) < ents[i].size  *(1.4) )
				{
					

					add_asteroids=1;
					add_asteroids_size=  ents[i].size/2;
					killed_x  = ents[i].x;
					killed_y  = ents[i].y;
					killed_xs  = ents[i].xspeed;
					killed_ys  = ents[i].yspeed;

					ents[i] = ents[active_ents -1];

					if (bullet_index == active_ents-1)
						bullet_index = i;

					active_ents--;
					i--;

					kill_bullet=1;

					

					continue;

				}

			}


			gx_sprite_draw_rotozoom(ents[i].sprite, ents[i].x, ents[i].y, ztrue, ents[i].ox,ents[i].oy,
				i>0 && (i != bullet_index) ?  ents[i].size/.15 : 1.0 ,
				ents[i].rotation);
		}

		
		if ((bullet_index >=0 )&& bullet_time) 
		{
			bullet_time--;
			if (!bullet_time)
				kill_bullet=1;

		}

	

	


		if (kill_bullet && (bullet_index >=0))
		{
			kill_bullet = 0;
			ents[bullet_index] = ents[--active_ents];
			bullet_index=-1;
		}

		if (add_asteroids && (add_asteroids_size > (.15 / 4) ) )
		{
			add_asteroids=0;
			for (i=0;i<3;i++)
			{

				ents[active_ents].x= killed_x ;
				ents[active_ents].y= killed_y;
				ents[active_ents].xspeed=((rand()&15)-8)/8.0 *.005 + killed_xs ;
				ents[active_ents].yspeed=((rand()&15)-8)/8.0 *.005 + killed_ys;
				ents[active_ents].rotation=0;
				ents[active_ents].rotation_speed=((rand()&15)-8)/8.0 ;
				ents[active_ents].sprite=  aster[i & 1] ;
				ents[active_ents].ox= add_asteroids_size;
				ents[active_ents].oy= add_asteroids_size;
				ents[active_ents].type = ASTER;
				ents[active_ents].size = add_asteroids_size;
				active_ents++;

			}

		}


		gx_frame_show();
	}

	ram_free(ship[0]);
	ram_free(ship[1]);
	ram_free(ship[2]);
	ram_free(aster[0]);
	ram_free(aster[1]);
	
	ram_free(image1);

}



zbool test_point(vec3* in)
{
	int iter;
	int axis;
	
	zfloat32 magnitude=0;


	vec3 c;
	vec3 v;

	vec3set(v,0,0,0);
	vec3mov(c, *in);
	
	for (iter=0;iter < 3;iter++)
	{
		//early reject outside cube
		if ((in->array[iter] < -6) ||(in->array[iter] > 6))
			return zfalse;

	}




	/*
	for (each axis)
  if (v[axis]>1) v[axis] = 2-v[axis];
  else if (v[axis]<-1) v[axis] = -2-v[axis];
if (v.magnitude() < 0.5) v *= 4;
else if (v.magnitude() < 1) v /= square(v.magnitude());
v = scale*v + c;

*/

	for (iter=0;iter<12;iter++)
	{
		for (axis=0;axis<3;axis++)
		{
			float magnitude;

			if ( v.array[axis] > 1)
				v.array[axis] = 2-v.array[axis];
			else if (v.array[axis]<-1)
				v.array[axis]= -2-v.array[axis];

		}

		magnitude = sqrt(vec3abs_sq(v));

		if (magnitude >32)
			return zfalse;

		if (  magnitude < .5 )
		{
			vec3scale(v, 4);
		}
		else if (magnitude < 1)
		{
			vec3scale(v, 1/(magnitude*magnitude));
		}
			

		vec3scale(v, 2);

		vec3add(v, c);
	}

	return ztrue;
}



//even more crap
void graphtest3()
{

	int i;

	//camera vars
	vec3 camera_pos;
	vec3 camera_up;
	vec3 camera_right;
	vec3 camera_forward;


	gx_vbuffer_t* vb= NULL;

	//initialize graphics
	if (GX_OK != gx_init(640, 480 , "Test Graphics Window"))
	{
		printf("Init Fail\n");
		return;
	}
	


	//camera init
	vec3set(camera_pos,		0,	0,	10);
	vec3set(camera_right,	1,	0,	0);
	vec3set(camera_up,		0,	1,	0);
	vec3set(camera_forward,	0,	0,	-1);



	vb = gx_vbuffer_mk(100*100*100 *10, 100*100*100*6, ztrue,zfalse, 0);

//	for (i=0;i<5000;i++)
//	{
//		gx_vbuffer_add_color( vb, 1, 1, 1,1);
//		gx_vbuffer_add_vertex(vb, (rand()&255)/255.0,(rand()&255)/255.0,(rand()&255)/255.0);
//
//	}

	{
		int i;
		int j;
		int k;

	
		//float x;
		//float y;
		//float z;

		vec3 v;

		vec3 vx;
		vec3 vy;
		
		


#define SSS ( 50/8.0 )
		float ss= 1/SSS;

		for (i=0;i<100;i++)
		{
		

			v.named.x =  (i-50)/SSS;
			vx.named.x = (i+1-50)/SSS;
		
			printf("%d\n", i);
		
			for (j=0;j<100;j++)
			{
				v.named.y = (j-50)/SSS;
				vx.named.x = (i+1-50)/SSS;

				for (k=99;k>80;k--)
				{
					v.named.z = (k-50)/SSS;

					
					

					if   ( test_point (&v) )
					{
						int ver;

#define CC				gx_vbuffer_add_color(vb, cos(v.named.z) ,cos(v.named.z*10), .5*.25*cos(v.named.z*100),1);


						CC
						gx_vbuffer_add_vertex(vb, v.named.x,v.named.y,v.named.z);

						CC
						gx_vbuffer_add_vertex(vb, v.named.x+ss,v.named.y,v.named.z);


					
						CC

						gx_vbuffer_add_vertex(vb, v.named.x+ss,v.named.y+ss,v.named.z);
						
						CC
						ver = gx_vbuffer_add_vertex(vb, v.named.x,v.named.y+ss,v.named.z);

						

						gx_vbuffer_add_index(vb, ver-3);
						gx_vbuffer_add_index(vb, ver-2);
						gx_vbuffer_add_index(vb, ver-1);
						
						gx_vbuffer_add_index(vb, ver-3);
						gx_vbuffer_add_index(vb, ver-1);
						gx_vbuffer_add_index(vb, ver );



					}


				}
			}
		}

	}

	gx_vbuffer_update(vb);

	while(1)
	{

		zchar c = gx_getkey();


		if (c=='Q') 
			break;

		//clear screen
		gx_clear_color(0,0,0,1);
		gx_frame_clear(ztrue,ztrue);

		//setup perspective
		gx_setup_3d( 70.0, gx_frame_get_dimensions(NULL,NULL), .1, 1000);


		//adjust camera position
		{
						
			zfloat32 yaw	= 0.0;
			zfloat32 pitch	= 0.0;
			zfloat32 roll	= 0.0;
			
			zfloat32 czs=0;
			zfloat32 cys=0;
			zfloat32 cxs=0;

			if (gx_key_state('w')) czs=.02;
			if (gx_key_state('s')) czs=-.02;
			if (gx_key_state('a')) cxs=-.02;
			if (gx_key_state('d')) cxs=+.02;
			if (gx_key_state('r')) cys=+.02;
			if (gx_key_state('f')) cys=-.02;


			if (gx_key_state('q')) roll=-.02;
			if (gx_key_state('e')) roll=.02;

			if (gx_key_state('4')) yaw=-.02;
			if (gx_key_state('6')) yaw=.02;

			if (gx_key_state('8')) pitch=-.02;
			if (gx_key_state('2')) pitch=.02;

			
			//move camera
			vec3madd(camera_pos, cxs, camera_right);
			vec3madd(camera_pos, cys, camera_up);
			vec3madd(camera_pos, czs, camera_forward);
			
		
			//try some spin crap
			gx_spin(yaw, pitch, roll,&camera_right, &camera_up, &camera_forward);

		}

		//set camera position
		gx_camera_pos_rot( &camera_pos, &camera_right, &camera_up, &camera_forward);

		/* render start*/

		gx_vbuffer_draw(vb, 0, vb->index_count, gx_points, ztrue);

		/*render end*/

		gx_frame_show();

		gx_window_event();
	}

}


//even even more crap

typedef struct tri_quadtree_s
{

	struct tri_quadtree_s* parent;
	struct tri_quadtree_s* child[4];

	gx_vbuffer_t* vb;
	zint32 ver[3]; //what vertex numbers in our vbuffer
	zint32 prim;  //what primitive number in our vbuffer
	
	vec3 middle;
	zfloat32 size;

} tri_quadtree_t;


tri_quadtree_t * tri_quadtree_mk(gx_vbuffer_t* preferred, zint32 v0, zint32 v1, zint32 v2, zfloat32 size)
{
	tri_quadtree_t* tri = ram_alloc( sizeof(tri_quadtree_t) , NULL);

	if (!tri)
		return NULL;
	
	tri->vb=preferred;;
	tri->ver[0] = v0;
	tri->ver[1] = v1;
	tri->ver[2] = v2;

	tri->prim =		gx_vbuffer_add_index(tri->vb, v0);
					gx_vbuffer_add_index(tri->vb, v1);
					gx_vbuffer_add_index(tri->vb, v2);

	tri->vb->index_notify[tri->prim] = &(tri->prim); //keep my prim up to date if indices move

	tri->size = size;

//	tri->size =  fabs( vbuffer_x( tri->vb, v0)    - vbuffer_x( tri->vb, v1) );
//	tri->size +=  fabs( vbuffer_x( tri->vb, v0)    - vbuffer_x( tri->vb, v2) );
//	tri->size +=  fabs( vbuffer_x( tri->vb, v1)    - vbuffer_x( tri->vb, v2) );

//	tri->size +=  fabs( vbuffer_y( tri->vb, v0)    - vbuffer_y( tri->vb, v1) );
//	tri->size +=  fabs( vbuffer_y( tri->vb, v0)    - vbuffer_y( tri->vb, v2) );
//	tri->size +=  fabs( vbuffer_y( tri->vb, v1)    - vbuffer_y( tri->vb, v2) );

//	tri->size +=  fabs( vbuffer_z( tri->vb, v0)    - vbuffer_z( tri->vb, v1) );
//	tri->size +=  fabs( vbuffer_z( tri->vb, v0)    - vbuffer_z( tri->vb, v2) );
//	tri->size +=  fabs( vbuffer_z( tri->vb, v1)    - vbuffer_z( tri->vb, v2) );

//	printf(" size %f\n", tri->size);

	
	vec3mov(tri->middle, *vbuffer_v(tri->vb, v0));
	vec3add(tri->middle, *vbuffer_v(tri->vb, v1));
	vec3add(tri->middle, *vbuffer_v(tri->vb, v2));
	vec3scale(tri->middle, 1.0f/3.0f);
	
//	vec3print(tri->middle);

	return tri;
}

zbool ok(gx_vbuffer_t* vb, int a, int b, int c)
{

	zfloat32 dab;
	zfloat32 dbc;
	zfloat32 dca;
	vec3 m;

	vec3mov(m, *vbuffer_v(vb, a));
	vec3sub(m, *vbuffer_v(vb, b));
	dab = sqrt(vec3abs_sq(m));

	vec3mov(m, *vbuffer_v(vb, b));
	vec3sub(m, *vbuffer_v(vb, c));
	dbc = sqrt(vec3abs_sq(m));

	vec3mov(m, *vbuffer_v(vb, c));
	vec3sub(m, *vbuffer_v(vb, a));
	dca = sqrt(vec3abs_sq(m));

	if ( dab > 3* dbc)
		return zfalse;

	if ( dab > 3* dca)
		return zfalse;


	if ( dbc > 3* dab)
		return zfalse;

	if ( dbc > 3* dca)
		return zfalse;


	if ( dca > 3* dab)
		return zfalse;

	if ( dca > 3* dbc)
		return zfalse;


	return ztrue;

}

void split_triangle(vec_t* triangles, tri_quadtree_t* tri, vec3* camera_pos)
{
	

	vec3 a;
	vec3 d;
	int i;

	int v_0_1=0;
	int v_1_2=0;
	int v_2_0=0;
	int v0;
	int v1;
	int v2;

//	printf(" split %p\n", tri); 
//	if ( ! (tri->child[0]    || tri->child[1] || tri->child[2] || tri->child[3]       ))
	{
		//if there are no children, create them

		v0=tri->ver[0];
		v1=tri->ver[1];
		v2=tri->ver[2];

	#define CCC  	gx_vbuffer_add_color(tri->vb, (a.named.z-5),(a.named.z-5),     (a.named.z-5)       ,1);


		//midpoint between vertex 0 and 1
		vec3mov(a, *vbuffer_v(tri->vb, v0));
		vec3add(a, *vbuffer_v(tri->vb, v1));
		vec3scale(a, 0.5f);
	//	a.named.z+=.05;



		//set up ray from camera to point
		vec3mov(d, a);  
		vec3sub(d, *camera_pos);
		vec3scale( d,   1/sqrt(vec3abs_sq(d)));
//		vec3print(d);
		vec3scale(d, .001);
		//for (a.named.z=6.0;a.named.z>2.5;a.named.z-=.001)
		if (test_point(&a))
			vec3sub(a,d);
		

		for (i=0;i<1000;i++)
		{
			if (test_point(&a))
			{
				CCC
				v_0_1 = gx_vbuffer_add_vertex(tri->vb, a.named.x, a.named.y, a.named.z);

				break;
			}
			vec3add(a, d);
		}
	





		//midpoint between vertex 1 and 2
		vec3mov(a, *vbuffer_v(tri->vb, v1));
		vec3add(a, *vbuffer_v(tri->vb, v2));
		vec3scale(a, 0.5f);
	//	a.named.z+=.05;



//		for (a.named.z=6.0;a.named.z>2.5;a.named.z-=.001)
//		{
//			if (test_point(&a))
//				break;
//		}

		//set up ray from camera to point
		vec3mov(d, a);  
		vec3sub(d, *camera_pos);
		vec3scale( d,   1/sqrt(vec3abs_sq(d)));
//		vec3print(d);
		vec3scale(d, .001);
		//for (a.named.z=6.0;a.named.z>2.5;a.named.z-=.001)
		if (test_point(&a))
			vec3sub(a,d);
		

		for (i=0;i<1000;i++)
		{
			if (test_point(&a))
			{
				CCC
				v_1_2 = gx_vbuffer_add_vertex(tri->vb, a.named.x, a.named.y, a.named.z);

				break;
			}
			vec3add(a, d);
		}



	

		//midpoint between vertex 2 and 0
		vec3mov(a, *vbuffer_v(tri->vb, v2));
		vec3add(a, *vbuffer_v(tri->vb, v0));
		vec3scale(a, 0.5f);
		//a.named.z+=.05;
		
//		for (a.named.z=6.0;a.named.z>2.5;a.named.z-=.001)
//		{
//			if (test_point(&a))
//				break;
//		}

		//set up ray from camera to point
		vec3mov(d, a);  
		vec3sub(d, *camera_pos);
		
		vec3scale( d,   1/sqrt(vec3abs_sq(d)));
//		vec3print(d);
		vec3scale(d, .001);
		//for (a.named.z=6.0;a.named.z>2.5;a.named.z-=.001)
		if (test_point(&a))
			vec3sub(a,d);
			

		for (i=0;i<1000;i++)
		{
			if (test_point(&a))
			{
				CCC
				v_2_0 = gx_vbuffer_add_vertex(tri->vb, a.named.x, a.named.y, a.named.z);
				break;
			}
			vec3add(a, d);
		}





		//create triangles

		if ((v_0_1 && v_2_0) &&ok( tri->vb, v0, v_0_1, v_2_0))
		{
			 tri->child[0]= tri_quadtree_mk( tri->vb, v0, v_0_1, v_2_0, tri->size/4);
			 tri->child[0]->parent =tri;
		}
		
		if ((v_0_1 && v_1_2)&&ok(tri->vb,v_0_1, v1, v_1_2))
		{
			tri->child[1]= tri_quadtree_mk( tri->vb, v_0_1, v1, v_1_2, tri->size/4);
			tri->child[1]->parent =tri;
		}
		
		if ((v_2_0 && v_1_2)&&ok(tri->vb, v_2_0, v_1_2, v2))
		{
			tri->child[2]=tri_quadtree_mk( tri->vb, v_2_0, v_1_2, v2, tri->size/4);
			tri->child[2]->parent =tri;
		}
		
		if ((v_0_1 && v_1_2 && v_2_0)&&ok(tri->vb,v_0_1, v_1_2, v_2_0))
		{
			tri->child[3]=tri_quadtree_mk( tri->vb, v_0_1, v_1_2, v_2_0, tri->size/4);
			tri->child[3]->parent =tri;
		}
		

	}
	
	{
		int i;
		for (i=0;i<4;i++)
			if (tri->child[i])
				vec_add(triangles, tri->child[i]);
	}


}


void remove_prim_from_vbuffer(gx_vbuffer_t* v, int prim)
{
	
	v->index_count -=3;

	v->index_data[prim] = v->index_data[ v->index_count ];
	v->index_data[prim+1] = v->index_data[ v->index_count +1];
	v->index_data[prim+2] = v->index_data[ v->index_count +2];


	v->index_notify[prim] = v->index_notify[v->index_count];

	if (v->index_notify[prim])
	{
		*(v->index_notify[prim]) = prim;
	}

}

void process_triangles(vec_t* triangles, vec3* camera_pos)
{
	int i;
	int oc = triangles->count;
	//printf(" %d triangles\n", triangles->count);
	for (i=0;i<oc;i++)
	{
		tri_quadtree_t* tri = vec_get_at(triangles, i);

		zfloat32 screensize;
		vec3 m;

		vec3mov(m, tri->middle); 
		vec3sub(m, *camera_pos);

		screensize = tri->size / (vec3abs_sq(m));
	//	printf(" %f\n", screensize);

		
		if (screensize >.01)
		{
			vec_remove_unordered(triangles, i); //remove the current triangle from the list
			remove_prim_from_vbuffer(tri->vb, tri->prim);
			tri->prim=0;

			split_triangle(triangles, tri, camera_pos);				
		}
		if (screensize< .01/5)
		{
			int j;
			
			if (tri->parent)
			{
				vec_remove_unordered(triangles, i); //remove triangle
				remove_prim_from_vbuffer(tri->vb, tri->prim);
				tri->prim=0;
				
				if (!tri->parent->prim)
				{
					vec_add(triangles, tri->parent);

					tri->parent->prim = gx_vbuffer_add_index( tri->vb, tri->parent->ver[0]);
					gx_vbuffer_add_index( tri->vb, tri->parent->ver[1]);
					gx_vbuffer_add_index( tri->vb, tri->parent->ver[2]);
				}

				
			}

		}


		
		if (triangles->count < oc)
			oc = triangles->count;
	}
	

}

typedef struct voxel
{
	int index;
} voxel_t;





int points[400][300];
	int tris[400][300];


void graphtest4()
{

	int i;
	int j;

	int ci=0;
	int cj=0;

	
	



	vec_t* triangles = NULL;

	//camera vars
	vec3 camera_pos;
	vec3 camera_up;
	vec3 camera_right;
	vec3 camera_forward;

	
	gx_vbuffer_t* vb= NULL;

	for (i=0;i<400;i++) for (j=0;j<300;j++)
	{

		points[i][j]=-1;
		tris[i][j]=-1;

	}

	//initialize graphics
	if (GX_OK != gx_init(640, 480 , "Test Graphics Window"))
	{
		printf("Init Fail\n");
		return;
	}
	
//	triangles = vec_mk(NULL, 100);

	//camera init
	vec3set(camera_pos,		0,	0,	9);
	vec3set(camera_right,	1,	0,	0);
	vec3set(camera_up,		0,	1,	0);
	vec3set(camera_forward,	0,	0,	-1);



	vb = gx_vbuffer_mk(100*100*100 *10, 100*100*100*6, ztrue,zfalse, 0);

//	gx_vbuffer_add_color(vb,1,1,1,1);
//	gx_vbuffer_add_vertex(vb,-6,-6,6);

//	gx_vbuffer_add_color(vb,1,0,1,1);
//	gx_vbuffer_add_vertex(vb,6,-6,6);

//	gx_vbuffer_add_color(vb,1,1,0,1);
//	gx_vbuffer_add_vertex(vb,-6,6,6);

//	gx_vbuffer_add_color(vb,1,1,0,1);
//	gx_vbuffer_add_vertex(vb,6,6,6);


	//add 
	//vec_add(triangles,  tri_quadtree_mk(vb, 0, 1, 2, 500));
	//vec_add(triangles,  tri_quadtree_mk(vb, 1,3,2, 500));


	{

		vec3 film;
		zfloat32 t;
		vec3 space;
		vec3 spacex;
		vec3 spacey;
		
		vec3 filmx;
		vec3 filmy;


		float d=0;
		float dx=0;
		float dy=0;


		for (i=0;i<400;i+=5)
		{
		//	printf("trace row %d\n", i);
			for (j=0;j<300;j+=5)
			{

				int hit;

				vec3mov(film, camera_forward);
				vec3madd(film, -(i-200)/200.0*.8, camera_right);
				vec3madd(film, -(j-150)/150.0*.8, camera_up);
				vec3scale(film, 1/sqrt(vec3abs_sq(film)));

				vec3mov(filmx, camera_forward);
				vec3madd(filmx, -(i+1-200)/200.0*.8, camera_right);
				vec3madd(filmx, -(j-150)/150.0*.8, camera_up);
				vec3scale(filmx, 1/sqrt(vec3abs_sq(film)));

				vec3mov(filmy, camera_forward);
				vec3madd(filmy, -(i-200)/200.0*.8, camera_right);
				vec3madd(filmy, -(j+1-150)/150.0*.8, camera_up);
				vec3scale(filmy, 1/sqrt(vec3abs_sq(film)));


				//film now contains the vector from lens to a particular point on the 'film'


				hit=0;


				for (t=0;t<20;t+=.01)
				{
					vec3mov(spacex, camera_pos);
					vec3madd(spacex, t, filmx);
					
					if ( test_point(&spacex))
					{
						hit++;
						dx=t;
						break;
					}
				}

				for (t=0;t<20;t+=.01)
				{
					vec3mov(spacey, camera_pos);
					vec3madd(spacey, t, filmy);
					
					if ( test_point(&spacey))
					{
						hit++;
						dy=t;
						break;
					}
				}





				for (t=0;t<20;t+=.01)
				{
					vec3mov(space, camera_pos);
					vec3madd(space, t, film);
					
					if ( test_point(&space) && (hit==2) )
					{
						hit++;
						gx_vbuffer_add_color(vb, 1, 1 , 1 ,1);
						//points[i][j] = gx_vbuffer_add_vertex(vb, space.named.x, space.named.y, space.named.z);
					//	printf("{%f}", t);
						break;
					}
				}








			}
		}


	}

	gx_vbuffer_update(vb);
ci=0;
cj=0;
	while(1)
	{

		zchar c = gx_getkey();

		/*update random points*/
		{
			int i =0;
			int j=0;
			vec3 film;
			vec3 space;


			vec3 filmx;
			vec3 spacex;

			vec3 filmy;
			vec3 spacey;

			float t;
			int k;
			float tts;

			int hit;

			float dx=0;
			float dy=0;

			for (k=0;k<50;k++){
			//calculate random pixel
			ci=ci+1;
			if (ci >=400)
			{
				
				ci=0;
				cj++;
			}

			if (cj >=300)
			{
				ci=0;
				cj=0;
			}
			cj = rand() % 300;

			//calculate intersection

			vec3mov(film, camera_forward);
			vec3madd(film, -(ci-200)/200.0*.8, camera_right);
			vec3madd(film, -(cj-150)/150.0*.8, camera_up);
			vec3scale(film, 1/sqrt(vec3abs_sq(film)));


			vec3mov(filmy, camera_forward);
			vec3madd(filmy, -(ci-200)/200.0*.8, camera_right);
			vec3madd(filmy, -(cj+1-150)/150.0*.8, camera_up);
			vec3scale(filmy, 1/sqrt(vec3abs_sq(filmy)));


			vec3mov(filmx, camera_forward);
			vec3madd(filmx, -(ci+1-200)/200.0*.8, camera_right);
			vec3madd(filmx, -(cj-150)/150.0*.8, camera_up);
			vec3scale(filmx, 1/sqrt(vec3abs_sq(filmx)));





			//film now contains the vector from lens to a particular point on the 'film'
			//vec3print(film);
			


			hit=0;
			
			tts=.001;
			for (t=0;t<100;t+=tts)
			{
				vec3mov(spacex, camera_pos);
				vec3madd(spacex, t, filmx);
				tts+=.0001;
				if ( test_point(&spacex))
				{
					dx=t;
					hit++;
					break;
				}
			}


			tts=.001;
			for (t=0;t<100;t+=tts)
			{
				vec3mov(spacey, camera_pos);
				vec3madd(spacey, t, filmy);
				tts+=.0001;
				if ( test_point(&spacey))
				{
					dy=t;
					hit++;
					break;
				}
			}






			tts=0;
			for (t=0;t<100;t+=tts)
			{
				vec3mov(space, camera_pos);
				vec3madd(space, t, film);
				tts+=.0001;
				
				

				if ( test_point(&space) && (hit==2) )
				{
					if (points[ci][cj]==-1)
					{
						points[ci][cj] = gx_vbuffer_add_vertex(vb, space.named.x, space.named.y, space.named.z);
					}

					vb->color_data[COLOR_COMPONENTS * points[ci][cj] ] = (t-dx)*1000;
					vb->color_data[COLOR_COMPONENTS * points[ci][cj] +1] = (t-dy)*1000;
					vb->color_data[COLOR_COMPONENTS * points[ci][cj] +2] = (dy-dx)*1000;
					vb->color_data[COLOR_COMPONENTS * points[ci][cj] +3] = 0;

					vb->vertex_data[VERTEX_COMPONENTS * points[ci][cj] ] = space.named.x;
					vb->vertex_data[VERTEX_COMPONENTS * points[ci][cj] +1] = space.named.y;
					vb->vertex_data[VERTEX_COMPONENTS * points[ci][cj] +2] = space.named.z;
					

					//add a triangle
					if (tris[ci][cj] == -1)
					{
						if ((ci <398) && (cj <298))
						{
							if ( ((points[ci+1][cj])!=-1) && ((points[ci][cj+1]!=-1)))
							{
								
								gx_vbuffer_add_index(vb, points[ci][cj]);
								gx_vbuffer_add_index(vb, points[ci][cj+1]);


								gx_vbuffer_add_index(vb, points[ci+1][cj]);

								//gx_vbuffer_add_index(vb, points[ci+1][cj]);
								//_vbuffer_add_index(vb, points[ci][cj+1]);

							}
						}
					}


					//	printf("{%f}", t);
					break;
				}
			}

		}

		}

	//	process_triangles(triangles, &camera_pos);
		gx_vbuffer_update(vb);

		if (c=='Q') 
			break;

		//clear screen
		gx_clear_color(0,0,0,1);
		gx_frame_clear(ztrue,ztrue);

		//setup perspective
		gx_setup_3d( 70.0, gx_frame_get_dimensions(NULL,NULL), .0001, 1000);


		//adjust camera position
		{
						
			zfloat32 yaw	= 0.0;
			zfloat32 pitch	= 0.0;
			zfloat32 roll	= 0.0;
			
			zfloat32 czs=0;
			zfloat32 cys=0;
			zfloat32 cxs=0;

			if (gx_key_state('w')) czs=.002;
			if (gx_key_state('s')) czs=-.002;
			if (gx_key_state('a')) cxs=-.002;
			if (gx_key_state('d')) cxs=+.002;
			if (gx_key_state('r')) cys=+.002;
			if (gx_key_state('f')) cys=-.002;


			if (gx_key_state('W')) czs=.02;
			if (gx_key_state('S')) czs=-.02;
			if (gx_key_state('A')) cxs=-.02;
			if (gx_key_state('D')) cxs=+.02;
			if (gx_key_state('R')) cys=+.02;
			if (gx_key_state('F')) cys=-.02;




			if (gx_key_state('q')) roll=-.02;
			if (gx_key_state('e')) roll=.02;

			if (gx_key_state('4')) yaw=-.02;
			if (gx_key_state('6')) yaw=.02;

			if (gx_key_state('8')) pitch=-.02;
			if (gx_key_state('2')) pitch=.02;

			
			//move camera
			vec3madd(camera_pos, cxs, camera_right);
			vec3madd(camera_pos, cys, camera_up);
			vec3madd(camera_pos, czs, camera_forward);
			
		
			//try some spin crap
			gx_spin(yaw, pitch, roll,&camera_right, &camera_up, &camera_forward);

		}

		//set camera position
		gx_camera_pos_rot( &camera_pos, &camera_right, &camera_up, &camera_forward);

		/* render start*/

	//	gx_vbuffer_draw(vb, 0, vb->index_count, gx_triangles, ztrue);

		gx_vbuffer_draw(vb, 0, vb->vertex_count, gx_points, zfalse);

		

		/*render end*/

		gx_frame_show();

		gx_window_event();
	}

}