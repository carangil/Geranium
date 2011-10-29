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

void graphics_main();
void game_main();

void main(int argc, char** argv)
{

	

	graphtest_main();
//	game_main();



	printf("allocations left: %d\n", ram_allocs());
	return 0;
}

/*Simple game entity management*/

typedef struct entity_s
{
	zfloat32 x;
	zfloat32 y;
	zfloat32 offsetx;
	zfloat32 offsety;
	zfloat32 x_speed;
	zfloat32 y_speed;
	zfloat32 rotation;	
	zfloat32 rotation_speed;	
	zfloat32 draw_size;
	zfloat32 intersect_size;
	gx_sprite_t* sprite;
} entity_t;

void entity_apply_movement(entity_t* e, zfloat32 minx, zfloat32 maxx, zfloat32 miny, zfloat32 maxy)
{
	e->x += e->x_speed;
	e->y += e->y_speed;
	e->rotation += e->rotation_speed;


    if (e->x > maxx) e->x = minx ;
	if (e->y > maxy) e->y = miny ;
	
	if (e->x < minx) e->x = maxx ;
	if (e->y < miny) e->y = maxy ;

}

void setup_player(entity_t* p, gx_sprite_t* s)
{
	//set up player entity
	p->draw_size = 1.0;
	p->intersect_size =.05;
	p->offsetx = .05;
	p->offsety=.05;
	p->sprite = s;
}

/* numbers less than 1.0 allow object colliding to partially overlap */
#define COLLIDE_BIAS 0.9

zbool entity_collide( entity_t* e , entity_t* f)
{

	//crap collision detection:  just compare distances

	zfloat32 dist = sqrt((e->x-f->x)*(e->x-f->x) + (e->y-f->y)*(e->y-f->y));
	zfloat32 mindist =  COLLIDE_BIAS * (e->intersect_size*e->draw_size) + (f->intersect_size*f->draw_size);


	if (dist <  mindist)
		return ztrue;

	return zfalse;

}

void game_main()
{
	//image
	gx_image_t* image1 = NULL;
	gx_image_t* font = NULL;
	
	//sprites
	gx_sprite_t* s_ship[3];  //3 ship animation sprites
	gx_sprite_t* s_aster[2]; // 2 asteroid shapes
	gx_sprite_t* s_bullet=NULL; //one player 'bullet' to shoot with

	//entities
	vec_t*		e_aster;
	entity_t	e_player;
	entity_t	e_bullet;

	zuint32		current_ship = 0;
	zbool		fired = zfalse;
	zuint32		bullet_time = 0;
	zuint32		place_asteroids = 5;
	zfloat32	place_asteroids_size = 1.0 ;
	zbool		place_asteroids_random = ztrue;
	zfloat32	place_asteroids_x;
	zfloat32	place_asteroids_y;


	zuint32		i=0;

	//initialize graphics
	if (GX_OK != gx_init(640, 480 , "Test Graphics Window"))
	{
		printf("Init Fail\n");
		return;
	}

	//load game data
	image1 = gx_image_load_tga( "asteroids.tga");
	font = gx_image_load_tga("font8.tga");

	if (!image1 || ! font)
	{
		printf("Load images fail\n");

		/*Free anything we can*/
		ram_free(image1);
		ram_free(font);
		return;
	}

	/*RGB=0 (black) A=1 */
	gx_clear_color(0, 0, 0, 1);

	//set up sprites
	s_ship[0] =  gx_sprite_mk(image1,0,64, 64, 64, .1, .1);
	s_ship[1] =  gx_sprite_mk(image1,64,64, 64, 64, .1, .1);
	s_ship[2] =  gx_sprite_mk(image1,128,64, 64, 64, .1, .1);
	s_aster[0] = gx_sprite_mk(image1,128+64,64, 64, 64, .3, .3);
	s_aster[1] = gx_sprite_mk(image1,0,0, 64, 64, .3, .3);
	s_bullet =   gx_sprite_mk(image1,32,32 ,3,3,.015,.015);

	/* set up game entities */
	
	ram_clear(&e_player, sizeof(e_player));
	ram_clear(&e_bullet, sizeof(e_bullet));
	setup_player(&e_player, s_ship[0]);

	e_aster = vec_mk(NULL, 10);

	while(1)
	{
		//accept input
		zchar c = gx_getkey();

		if (c=='Q') 
			break;

		/* Player rotation control*/
		if (gx_key_state('4'))
			e_player.rotation_speed = -5;

		else if (gx_key_state('6'))
			e_player.rotation_speed = 5;
		
		else e_player.rotation_speed = 0;

		/* Player thrust control */
		if (gx_key_state('8'))
		{
			printf(" %f %f \n", e_player.x_speed , e_player.y_speed);

			e_player.x_speed += (.001 * cos( e_player.rotation / 180.0 *3.14159));
			e_player.y_speed += (.001 * sin( e_player.rotation / 180.0 *3.14159));

			e_player.sprite = s_ship[current_ship++];

			current_ship = current_ship % 3;
		
		}
		else
			e_player.sprite = s_ship[0];
		


		//update game state

		entity_apply_movement(&e_player,  -1.0, 1.0, -1.0, 1.0 );
		
		for (i=0;i<e_aster->count;i++)
		{
			entity_t* e = (entity_t*) vec_get_at(e_aster, i);
			if (e)
			{
				entity_apply_movement(e, -1,1,-1,1);

				if ( entity_collide( e, &e_player))
					exit(0);

			}
		}

		//place any new objects required
	
		if (place_asteroids)
		{
			entity_t* e;

			for (i=0;i<place_asteroids;i++)
			{
				e = ram_alloc(sizeof(*e), NULL);
				if (e)
				{
					e->draw_size = place_asteroids_size;
					e->intersect_size = .15;
				
					if (place_asteroids_random)
					{
						e->x= ((rand()&15)-8)/8.0  ;
						e->y= ((rand()&15)-8)/8.0  ;
					} 
					else
					{
						e->x = place_asteroids_x;
						e->y = place_asteroids_y;
					}
					e->x_speed=((rand()&15)-8)/8.0 *.005 ;
					e->y_speed=((rand()&15)-8)/8.0 *.005;
					e->rotation_speed=((rand()&15)-8)/8.0 ;

					e->offsetx = (e->draw_size * e->intersect_size);
					e->offsety = (e->draw_size * e->intersect_size);
					e->sprite =  s_aster[i & 1] ;

					vec_add(e_aster, e);
				}
		
			}
			place_asteroids=0;
		}

		//draw screen
		gx_frame_clear(ztrue, ztrue);

		gx_text_color(1,1,1,1);
		gx_text_size( .05,.1);
		gx_text_draw(font, -1, -1, 0,"Hello test");

		gx_text_size( .03,.2);
		gx_text_draw(font, -1, -.8, 0,"Different Size");

		
		//draw asteroids

		for (i=0;i<e_aster->count;i++)
		{
			entity_t* e = (entity_t*) vec_get_at(e_aster, i);
			if (e)
			{
				gx_sprite_draw_rotozoom(e->sprite, e->x, e->y, ztrue, e->offsetx, e->offsety, e->draw_size, e->rotation);		
			}
		}
		printf(" %d asteroids\n", e_aster->count);

		//draw player
		gx_sprite_draw_rotozoom(e_player.sprite, e_player.x, e_player.y, ztrue, e_player.offsetx, e_player.offsety, e_player.draw_size, e_player.rotation);
		
		gx_line_color(0,1,0,1);
		gx_arcgon( e_player.x, e_player.y,  e_player.intersect_size, e_player.intersect_size,0, 2*3.14159, 50, zfalse);

		gx_line_color(0,0,1,1);
		gx_box
			(e_player.x - e_player.intersect_size, e_player.y - e_player.intersect_size,
			e_player.x + e_player.intersect_size, e_player.y + e_player.intersect_size);

		
		//lets draw circles around all the asteroids
#if 1
		gx_line_color(1,0,0,1);
		for (i=0;i<e_aster->count;i++)
		{
			entity_t* e = (entity_t*) vec_get_at(e_aster, i);
			if (e)
				gx_arcgon( e->x, e->y, e->intersect_size, e->intersect_size,0, 2*3.14159, 50, zfalse);
		}
		

#endif 

		gx_line_color(1,0,0,1);
		

		gx_line_finish();

		gx_line_color(1,1,1,1);
				
		gx_frame_show();
		gx_window_event(); //process window system events (must be called once per loop)
	}


	//clean up sprites
	ram_free(e_aster);
	
	ram_free(s_ship[0]);
	ram_free(s_ship[1]);
	ram_free(s_ship[2]);
	ram_free(s_aster[0]);
	ram_free(s_aster[1]);
	ram_free(s_bullet);

	ram_free(image1);
	ram_free(font);

	gx_disable();  

}