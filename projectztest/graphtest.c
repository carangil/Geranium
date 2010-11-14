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



//return is the new camera sector
gx_sector_t* draw_sector(gx_sector_t* sector, vec3* camera_position, vec3* camera_look, gx_sector_t* camera_sector)
{
	vec3 diff;
	zfloat32 dist=0;
	gx_portal_t * p;

	zfloat32 cosang=0;

	zbool visible;
	gx_sector_t* new_camera_sector=camera_sector;

	//draw the sector outline
	gx_sector_outline(sector);
	
	
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


	gx_vbuffer_t * vbuf = NULL;
	gx_image_t *image1 = NULL;
	gx_image_t *image2 = NULL;
	gx_sprite_t* sprite = NULL;

	gx_image_t* heightmap = NULL;
	gx_vbuffer_t* heightbuffer = NULL;

	gx_image_t* heightmap2 = NULL;
	gx_vbuffer_t* heightbuffer2 = NULL;



	printf("Init graphics\n");
	gx_init(640, 480 , "Test Graphics Window");

	gx_clear_color(.1,.1,.6,1);
	gx_frame_clear(ztrue,ztrue);
	gx_frame_show();

	image1 = gx_image_load_tga( "rgbatarga.tga");
	image2 = gx_image_load_tga( "tex2.tga");
	
	
	//gx_image_enable(image1);  //don't need to enable because first use will

	vec3set(camera_pos,		0,	.5,	1);
	vec3set(camera_right,	1,	0,	0);
	vec3set(camera_up,		0,	1,	0);
	vec3set(camera_forward,	0,	0,	-1);

	sprite = gx_sprite_mk(image1,100,100, image1->width, image1->height, .1, .1);

	vbuf = gx_vbuffer_mk(100, 12, ztrue, 1);
	
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

	sharebuffer = gx_vbuffer_mk( 256*256*4, 3*256*256*4, ztrue, 2);

	heightbuffer = gx_vbuffer_from_image( sharebuffer, heightmap, 0.0,0.0,0.0,   //offset
		0,1,2,        //axis swizzle
		3.0,.3,3.0,  //scaling
		ztrue, 2, &start, &end);

//	

	mesh0 = gx_mesh_def( heightbuffer, &ds1, start, end, ztrue);
	
	

	heightmap2 = gx_image_load_tga("hmap.tga");

	heightbuffer2 = gx_vbuffer_from_image(sharebuffer, heightmap2, 0.0,0.0,-3.0,   //offset
		0,1,2,        //axis swizzle
		3.0,.3,3.0,  //scaling
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





	mesh0->next = mesh1;  //link both meshes


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

		vec3set(mins, 1,0, -2);
		vec3set(maxs, 2,2, -4);
		sector2 = gx_sector_mk(&mins, &maxs);

		vec3set(pos, .5,.5 ,-1);
		gx_sector_add_portal(sector0, &pos, .5, sector1);

		vec3set(pos, .5,.5 ,-3);
		gx_sector_add_portal(sector1, &pos, .5 , sector2);
		
		camera_sector = sector0;  //start here

	}


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
	
		gx_setup_3d( 70.0, gx_frame_get_dimensions(NULL,NULL), .1, 1000);

		

		{
			char c;
			
			zfloat32 yaw	= 0.0;
			zfloat32 pitch	= 0.0;
			zfloat32 roll	= rolls;
			
			czs=0;
			cys=0;
			cxs=0;



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

			c = gx_getkey();
			if (c=='Q') 
				break;
			
			if (c=='m')
				gx_mouse_capture(zfalse);
				
			

			if (c=='M')
				gx_mouse_capture(ztrue);
			


			pitch += my*.001;
			yaw += mx*.001;
	
			vec3madd(camera_pos, cxs, camera_right);
			vec3madd(camera_pos, cys, camera_up);
			vec3madd(camera_pos, czs, camera_forward);

			//try some spin crap
			gx_spin(yaw, pitch, roll,&camera_right, &camera_up, &camera_forward);

		}
		
			
		gx_camera_pos_rot( &camera_pos, &camera_right, &camera_up, &camera_forward);
		
	gx_set_active_textures(NULL, 0);

	
	//draw sectors, and return the new camera sector
	camera_sector = draw_sector(camera_sector, &camera_pos, &camera_forward, camera_sector );


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

	

	//	gx_mesh_draw(mesh0);

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
	
	int add_asteroids=0;
	float add_asteroids_size=0;
	float killed_x;
	float killed_y;
	float killed_xs;
	float killed_ys;


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

	for(i=1;i<START_ENTS;i++)
	{
		ents[i].x=  ((rand()&15)-8)/8.0  ;
		ents[i].y=((rand()&15)-8)/8.0;
		ents[i].xspeed=((rand()&15)-8)/8.0 *.005 ;
		ents[i].yspeed=((rand()&15)-8)/8.0 *.005;
		ents[i].rotation=0;
		ents[i].rotation_speed=((rand()&15)-8)/8.0 ;
		ents[i].sprite=  aster[i & 1] ;
		ents[i].ox= .15;
		ents[i].oy= .15;
		ents[i].type = ASTER;
		ents[i].size = .15;
		
	}
	active_ents = START_ENTS;

	gx_clear_color(0,0,0,1);
	
	gx_frame_clear(ztrue,ztrue);

	while( 1)
	{
		zchar c ;

		gx_window_event();

		c= gx_getkey();

		if (c=='Q') break;

		

		if (c==' ' && (bullet_index == -1 ))
		{

			ents[active_ents].x=ents[0].x;
			ents[active_ents].y=ents[0].y;

		
			ents[active_ents].xspeed = .01 * cos( ents[0].rotation / 180.0 *3.14159) + ents[0].xspeed*.1;
			ents[active_ents].yspeed = .01 * sin( ents[0].rotation / 180.0 *3.14159) + ents[0].yspeed*.1;

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
			bullet_time = 240;
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
			ents[0].rotation_speed = 5;
		else if (gx_key_state('6'))
			ents[0].rotation_speed =-5;
		else
			ents[0].rotation_speed=0;


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
				if ( ent_dist( ents+bullet_index, ents+i) < ents[i].size)
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
