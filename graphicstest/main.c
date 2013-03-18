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
#include "../graphics/gx_light.h"
#include "../structures/vector.h"
#include "../graphics/gx_misc.h"
#include "../graphics/gx_mesh.h"
#include "meshtree.h"


int g_toggle=0;

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


//game constants
#define STARCOUNT 1000


char get_char_in_string_vec( vec_t* v, int row, int col)
{
	
	char* s = 0;

	if ((row < 0) || (col < 0) ) 
		return 0;

	if (row >= vec_count(v))
		return 0;
	
	s = vec_get_x_at(v, char*, row);

	if (col >= strlen(col))
		return 0;

	return s[col];

}

float aspect = 3.0/4;



void draw_sector(gx_sector_t* sector, zbool use_tag)
{
	int i;
	if (!sector)
		return;


	//draw the sector's meshes
	for (i=0;i<sector->meshes.count; i++)
	{
		gx_mesh_draw( vec_get_at( &(sector->meshes) , i)); 	
	}

}

zbool point_in_box(vec3* v, vec3* min, vec3* max)
{
	return  (v->vec3x >= min->vec3x) && (v->vec3x <= max->vec3x) &&
		    (v->vec3y >= min->vec3y) && (v->vec3y <= max->vec3y) &&
		    ( v->vec3z >= min->vec3z) && (v->vec3z <= max->vec3z) ;
}


void restrict_point_to_box(vec3* v, vec3* min, vec3* max)
{
	if(v->vec3x < min->vec3x) 
			v->vec3x = min->vec3x ;
	
	if(v->vec3y < min->vec3y) 
			v->vec3y = min->vec3y ;
	
	if(v->vec3z < min->vec3z) 
			v->vec3z = min->vec3z ;
	
	if(v->vec3x > max->vec3x) 
			v->vec3x = max->vec3x ;
	
	if(v->vec3y > max->vec3y) 
			v->vec3y = max->vec3y ;
	
	if(v->vec3z > max->vec3z) 
			v->vec3z = max->vec3z ;
		
		
}



gx_sector_t* portal_process(gx_sector_t * camera_sector, g_camera_t* player_camera, float fov, int framenum)
{
	int numdraw =0;
	gx_sector_t* new_camera_sector = NULL;
	//vec_t* pending_portals = NULL; // these are portals that are not yet confirmed visible
	vec_t* pending_sectors = NULL;
	int i=0;

	gx_sector_t* s = NULL;
	gx_portal_t* p = NULL;
	
	if (!g_toggle)
		draw_sector(camera_sector, ztrue);  //draw the camera sector
	else
		gx_sector_outline(camera_sector);

	numdraw++;
	pending_sectors = vec_mk(NULL, 16);

	camera_sector->lastframe = framenum;
	camera_sector->rdepth = 0;

	
	//start at camera sector
	vec_add(pending_sectors, camera_sector);
	
	//printf("\nSTART SECTORS from {%p}:", camera_sector);

	//for all sectors
	while( vec_count(pending_sectors) )
	{
		s = vec_remove_unordered(pending_sectors, vec_count(pending_sectors)-1);

		//go through all portals of this sector
		p = s->portals;

		while(p)
		{
			vec3 diff;
			float dist;
			float cosang;
			zbool visible = zfalse;

			vec3mov(diff, p->pos);
			vec3sub(diff, player_camera->camera_pos);  //diff = portal position - camera position
			dist = sqrt(vec3abs_sq(diff));
			vec3scale(diff, 1.0/dist);       //diff is normalized vector

		
			//if (s == camera_sector)
			//0 means consider only camera-sector portals for movement
			//1 means consider portals from room directly connected to camera
			if (s->rdepth <= 1)
			{
				if (point_in_box(&player_camera->camera_pos , &(p->target->min), &(p->target->max)       ))
				//if (!point_in_box(&player_camera->camera_pos , &(s->min), &(s->max)       ))
				{
						new_camera_sector = p->target;
				}
			}

		
			

			if (dist <= p->radius)
			{
				visible = ztrue;
				
			}
			


		//	if (dist < p->radius)
		//	{
		//		visible = ztrue;
		//	}
		//	else
			{
				cosang = vec3dot(player_camera->camera_forward, diff);
				if (cosang > cos( aspect*fov   /180*3.14159    +atan(p->radius/dist) ))
				{
					//we are inside view cone.  See if portal is forward or backward facing:


					//now lets check if camera faces the opposite way as the portal
					if ( vec3dot(diff, p->normal) < 0 )
					{
						//printf(" portal is facing correct way\n");
						visible = ztrue;
					}

				}
			}


			//if (visible &&(p->target->lastframe == framenum))
	//		{
	//			if (s->rdepth + 1 < p->target->rdepth)
	//				p->target->rdepth = s->target_rdepth +1;
//
//			}

			if (s->rdepth <= 50) //set render depth
			if (visible && p->target->lastframe != framenum)
			{
				p->target->lastframe = framenum;
				p->target->rdepth = s->rdepth +1;

				if (!g_toggle)
					draw_sector(p->target,ztrue);  //draw the sector we lead to
				else
					gx_sector_outline(p->target);

				//x_sector_outline(p->target);

				numdraw++;
				//printf(" Adding sector %x to list\n", p->target);
//				printf(" {%p} ", p->target);
				vec_add(pending_sectors, p->target);

			//	vec_add( active_portals, p);  // p is a confirmed active portal

				//if (s== camera_sector)
					//gx_portal_draw_test(p);
			}



			p = p->next_portal;
		}	
	}

	printf(" %d sectors \n", numdraw);
	//printf("\nEND");
	//Now test all pending portals and put on active list if visible

		
	//need to cleanup	
	ram_free(pending_sectors);

	if (new_camera_sector)
	{
		printf(" SWITCH from %p to %p \n", camera_sector, new_camera_sector);
		return new_camera_sector; //return new camera sector

	}
	else
		return camera_sector; //otherwise return old camera sector
}




int main(int argc, char** argv)
{
	// IO variables
	zchar keypress=0;
	zint32 mouse_x=0;
	zint32 mouse_y=0;
	zbool  mouse_relative=zfalse;

	int frame_number = 1;

	//game/graphics variables
	g_camera_t	player_camera;
	vec3		camera_inertia;


	gx_light_t*	light = NULL;
	//int sheetlevel=0;


	gx_vbuffer_t*  wall_buffer = NULL;


	//need a starfield (we ARE in space)
	gx_vbuffer_t*	starfield = NULL;

	

	gx_image_t		*brick = NULL;  //holds rock texture for asteroids
	gx_drawstyle_t	brick_ds;   //drawstyle for the asteroid
	
	gx_sector_t* camera_sector = NULL;
	
	//need character sprites
	gx_image_t*  alien_image[8];
	gx_sprite_t* alien_sprite[8];


	//meshtree_t* mtree = NULL;
	
//	sheet_t*  sheet = NULL;
	gx_vbuffer_t* vb =  NULL;

	vec_t sectors;

	//Initialize graphics
	//gx_init(1024, 768 , "Test");
	gx_init(640, 480 , "RetroMaze");
	gx_clear_color(0,0,0,1);
	gx_mouse_capture(ztrue);  //mouse input will be relative 
	
	//Load assets
	//brick  = gx_image_load_tga( "brick.tga");
//	brick  = gx_image_load_tga( "stone.tga");
	brick  = gx_image_load_tga( "stone-lowres.tga");
	//brick  = gx_image_load_tga( "brick2.tga");
//	brick  = gx_image_load_tga( "brick_lowres.tga");
//	brick  = gx_image_load_tga( "rock.tga");
	//brick  = gx_image_load_tga( "red.tga");
	ram_clear(&brick_ds, sizeof(brick_ds));
	brick_ds.textures = &brick;
	brick_ds.numtextures=1;


	//load alien
	alien_image[0] = gx_image_load_tga ("alien\\edit\\front-a.tga");
	alien_image[1] = gx_image_load_tga ("alien\\edit\\front-b.tga");
	
	alien_image[2] = gx_image_load_tga ("alien\\edit\\left-a.tga");
	alien_image[3] = gx_image_load_tga ("alien\\edit\\left-b.tga");

	alien_image[4] = gx_image_load_tga ("alien\\edit\\back-a.tga");
	alien_image[5] = gx_image_load_tga ("alien\\edit\\back-b.tga");

	alien_image[6] = gx_image_load_tga ("alien\\edit\\right-a.tga");
	alien_image[7] = gx_image_load_tga ("alien\\edit\\right-b.tga");

	

	{
		int i;
		for (i=0;i<8;i++)
		{
			alien_sprite[i]= gx_sprite_mk(alien_image[i], 0,0,alien_image[i]->width, alien_image[i]->height, 1, 1);

		}

	}


	wall_buffer = gx_vbuffer_mk(65535,65535,zfalse, ztrue, 1);	
	

	//initialize game data
	g_camera_init(&player_camera);
	vec3set(camera_inertia, 0,0,0);

	{
		vec3 lightpos;
		vec3 lightcolor;
		vec3 lightambient;
		vec3set (lightpos, 3, 0, -1.5 );
		vec3set (lightcolor, 1, 1, 1);
		vec3set (lightambient, .1, .1, .1);

		light =  gx_light_mk(gx_light_point, &lightpos, &lightcolor, &lightambient);

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

	


	//load up a level file
	{
		char rowbuf[1024];
		int row=0;
		int col=0;
		
		gx_sector_t** sectorgrid = NULL;
		int colcnt=0;


		vec_t rows;

		vec3 norm;

		FILE* f = fopen( "level.txt", "rt");
		vec_mk(&sectors, 32);
		vec_mk(&rows, 32);
		
		if (f)
		{
			while (1== fscanf(f, "%1023s", &rowbuf))
			{
				vec_add_or_free( &rows, ram_strdup(rowbuf));  //copy out
			}

			//flip upside down 

			for (row=0; row < (vec_count(&rows)/2); row ++)
			{
				void* t1, *t2 ;

				t1 = vec_get_at(&rows, row);
				t2 = vec_get_at(&rows, vec_count(&rows) -1 -row );

				vec_set_at(&rows, row, t2);
				vec_set_at(&rows, vec_count(&rows) -1 -row, t1);



			}



			for (row=0;row < vec_count(&rows);row++)
			{
				int cnt;
				cnt = strlen( vec_get_at(&rows, 0));

				if (row==0)
				{
					colcnt = cnt;
					sectorgrid = ram_alloc(sizeof (gx_sector_t*) * colcnt*vec_count(&rows), NULL);
				}
				else
				{
					if (colcnt != cnt)
					{
						printf("row lenth mismatch on row %d\n", row);
						exit(1);
					}
				}

				for (col=0;col<cnt;col++)
				{

					char ch = vec_get_x_at( &rows, char*, row)[col];
						
					
					//room
					if (ch=='.' || ((ch >='0') && (ch <= '9')) )
					{
						int meshstart=0;
						int meshend=0;

						float ylev = 0;
						float ylev2 = 0;

						
						vec3 min, max;
						gx_sector_t* s = NULL;

						if ((ch >='0') && (ch <= '9'))
						{
							ylev = ch-'0';
							ylev=ylev/5;
						}


						vec3set(min,  col, -.5 +ylev, -row-1);

						vec3set(max, col+1, .5 +ylev, -row);


						meshstart = wall_buffer->vertex_count;

						s = gx_sector_mk( &min, &max);

						sectorgrid[ row *colcnt + col] = s;

						//now add portal to neighbors

#define PSIZE  (sqrt(2)/2)
//#define PSIZE .1

#if 1
						if (row > 0 )
						{
							gx_sector_t* t =sectorgrid[(row-1)*colcnt + (col)];
							if (t)
							{
								vec3 pos;
								vec3set(pos, col+.5, ylev , -row  );

								vec3set(norm, 0, 0, -1);
								gx_sector_add_portal(s, &pos, PSIZE ,t, &norm); 
								vec3set(norm, 0, 0, 1);
								gx_sector_add_portal(t, &pos, PSIZE,s, &norm); 

							}

						}
#endif

						if (col > 0 )
						{
							gx_sector_t* t =sectorgrid[(row)*colcnt + (col-1)];
							if (t)
							{
								vec3 pos;
								vec3set(pos, col , ylev , -row -.5);

								vec3set(norm, 1, 0, 0);
								gx_sector_add_portal(s, &pos, PSIZE,t, & norm); 
								vec3set(norm, -1, 0, 0);
								gx_sector_add_portal(t, &pos, PSIZE,s, & norm); 

							}
						}

						//create floor
						gx_vbuffer_add_tex(wall_buffer, 0, 0, 0);
						gx_vbuffer_add_normal(wall_buffer, 0, 1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , -.5 + ylev, min.vec3z);

						gx_vbuffer_add_tex(wall_buffer, 0, 0, 1);
						gx_vbuffer_add_normal(wall_buffer, 0, 1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , -.5+ ylev, max.vec3z);

						gx_vbuffer_add_tex(wall_buffer, 0, 1, 1);
						gx_vbuffer_add_normal(wall_buffer, 0, 1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , -.5+ ylev, max.vec3z);

						gx_vbuffer_add_tex(wall_buffer, 0, 1, 0);
						gx_vbuffer_add_normal(wall_buffer, 0, 1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , -.5+ ylev, min.vec3z);

						//create ceiling
#if 1
						gx_vbuffer_add_tex(wall_buffer, 0, 0, 0);
						gx_vbuffer_add_normal(wall_buffer, 0, -1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  min.vec3x ,  .5+ ylev, min.vec3z);

						gx_vbuffer_add_tex(wall_buffer, 0, 0, 1);
						gx_vbuffer_add_normal(wall_buffer, 0, -1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  min.vec3x ,  .5+ ylev, max.vec3z);

						gx_vbuffer_add_tex(wall_buffer, 0, 1, 1);
						gx_vbuffer_add_normal(wall_buffer, 0, -1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  max.vec3x ,  .5+ ylev, max.vec3z);

						gx_vbuffer_add_normal(wall_buffer, 0, -1, 0);
						gx_vbuffer_add_tex(wall_buffer, 0, 1, 0);
						gx_vbuffer_add_vertex(wall_buffer,  max.vec3x ,  .5+ ylev, min.vec3z);

#endif


						//create walls

						//col-1
							if ( col>0)
								ch = vec_get_x_at( &rows, char*, row)[col-1];
							else 
							{
								ch ='x';
								ylev2 = ylev +1;
							}

							//calc other level
							if ((ch >='0') && (ch <= '9'))
							{
								ylev2 = ch-'0';
								ylev2=ylev2/5;
							}


							if ( ylev2 > (ylev+.01) )
							{
								gx_vbuffer_add_normal(wall_buffer, 1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 0);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , -.5 + ylev, min.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , (-.5 +ylev2-ylev) , min.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 1);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , (-.5 +ylev2-ylev), max.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 0);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , -.5 +ylev, max.vec3z);

							}



							//col+1
							if ( col<colcnt)
								ch = vec_get_x_at( &rows, char*, row)[col+1];
							else 
								ch ='x';

							if (ch == 'x')
							{
								gx_vbuffer_add_normal(wall_buffer, -1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 0);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , -.5, min.vec3z);

								gx_vbuffer_add_normal(wall_buffer, -1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , .5, min.vec3z);

								gx_vbuffer_add_normal(wall_buffer, -1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 1);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , .5, max.vec3z);

								gx_vbuffer_add_normal(wall_buffer, -1, 0, 0);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 0);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , -.5, max.vec3z);

							}



#if 1
						//row+1
							if ( row< vec_count(&rows) )
								ch = vec_get_x_at( &rows, char*, (row+1))[col];
							else 
								ch ='x';

							if (ch == 'x')
							{
								gx_vbuffer_add_normal(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 0);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , -.5, min.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , .5, min.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 1);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , .5, min.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 0);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , -.5, min.vec3z);

							}
#endif	

							//row-1
							if ( row< vec_count(&rows) )
								ch = vec_get_x_at( &rows, char*, (row-1))[col];
							else 
								ch ='x';

							if (ch == 'x')
							{
								gx_vbuffer_add_normal(wall_buffer, 0, 0, -1);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 0);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , -.5, max.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 0, 0, -1);
								gx_vbuffer_add_tex(wall_buffer, 0, 0, 1);
								gx_vbuffer_add_vertex(wall_buffer,  min.vec3x , .5, max.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 0, 0, -1);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 1);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , .5, max.vec3z);

								gx_vbuffer_add_normal(wall_buffer, 0, 0, -1);
								gx_vbuffer_add_tex(wall_buffer, 0, 1, 0);
								gx_vbuffer_add_vertex(wall_buffer,  max.vec3x , -.5, max.vec3z);

							}




						meshend = wall_buffer->vertex_count;

						if (meshstart != meshend)
						{
							gx_mesh_t* mesh;
							mesh = gx_mesh_def(wall_buffer, &brick_ds, meshstart, meshend, zfalse);
							mesh->prim = gx_quads;
							
							vec_add( & s->meshes, mesh);
						}

 						vec_add(&sectors, s);
						if (!camera_sector )
							camera_sector=s;

					}

				}
			}

		}
		ram_free(sectorgrid);
	}

	

	gx_vbuffer_update(wall_buffer);

	//The game loop 
	for(;;)  
	{

		//light->position.vec3x += .02;
		light->position = player_camera.camera_pos;
		vec3madd(light->position, -1,player_camera.camera_forward);

 		gx_window_event();  //handles any window events (I/O)

		//set up projection matrix for this frame
 		//gx_setup_3d( 70.0f,  gx_frame_get_dimensions(NULL,NULL), .01f, 1000.0f);
		gx_setup_3d( 90.0f,  gx_frame_get_dimensions(NULL,NULL), .01f, 1000.0f);
		
		//Read mouse input
		gx_mouse_pos(&mouse_x, &mouse_y, &mouse_relative);
		if (!mouse_relative)
		{
			//if some some reason we get an absolute mouse position, clear it out
			mouse_x=0;
			mouse_y=0;
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
		else if (keypress =='+')
		{
//			sheetlevel= (sheetlevel +1) % sheet->levels;
		}
		else if (keypress=='-')
		{
//			if (sheetlevel > 0)
//				sheetlevel--;
			
		}
		else if (keypress=='t')
			 g_toggle ^= 1;


		//camera control
		{
			zfloat32 delta_yaw		= 0.0;
			zfloat32 delta_pitch	= 0.0;
			zfloat32 delta_roll		= 0.0;
		
			vec3	 delta_pos;
			vec3set	 (delta_pos, 0,0,0);
			
			//vec3set(camera_inertia, 0,0,0);

			if (gx_key_state('w')) delta_pos.vec3z+=.015;
			if (gx_key_state('s')) delta_pos.vec3z=-.015;
			if (gx_key_state('a')) delta_pos.vec3x=-.015;
			if (gx_key_state('d')) delta_pos.vec3x=+.015;
			if (gx_key_state('r')) delta_pos.vec3y=+.015;
			if (gx_key_state('f')) delta_pos.vec3y=-.015;
			if (gx_key_state('x')) vec3set(camera_inertia, 0,0,0);

			if (gx_key_state('q')) delta_roll=-.02;
			if (gx_key_state('e')) delta_roll=.02;

			if (gx_key_state('4')) delta_yaw=-.02;
			if (gx_key_state('6')) delta_yaw=.02;

			if (gx_key_state('8')) delta_pitch=-.02;
			if (gx_key_state('2')) delta_pitch=.02;

			

			//move camera using camera's basis
			vec3madd( camera_inertia, delta_pos.vec3x, player_camera.camera_right);
			vec3madd( camera_inertia, delta_pos.vec3y, player_camera.camera_up);
			vec3madd( camera_inertia, delta_pos.vec3z, player_camera.camera_forward);

			vec3madd( player_camera.camera_pos, 3 , camera_inertia);


			vec3set(camera_inertia, 0,0,0);//reset every frame

			//lets spin camera  (relative to its own coord system)

			delta_pitch += mouse_y*.003;
			delta_yaw += mouse_x*.003;

			gx_spin(ztrue, delta_yaw, delta_pitch, delta_roll,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);

		}

		//clear screen		
		gx_frame_clear(ztrue,ztrue);
		
		//draw starfield
		gx_camera_home();
		gx_camera_pos_rot(NULL,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward); 
		gx_zbuffer(zfalse);
		
		gx_drawstyle_activate(NULL) ;
		gx_vbuffer_draw(starfield, 0, starfield->vertex_count, gx_points, zfalse);

		gx_zbuffer(ztrue);
		//render here

		gx_camera_pos_rot( &player_camera.camera_pos,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);
	
		vec3set(brick_ds.specular_color, 0,0,0);
		brick_ds.specular_exponent= 20;
		
	//	gx_drawstyle_activate(&brick_ds);
		
		
		gx_set_active_lights(NULL,0);

		gx_drawstyle_activate(NULL) ;
		//gx_debug_show_light( light, .1);


		{
			vec3 pos;
			vec3 up;
			vec3 right;

			//vec3set (pos, 3, -.1, -1.5);

			vec3set (pos, 6, -.1, -2.5);
			vec3set (up, 0, 1, 0);
			vec3cross( right,  player_camera.camera_forward, up);
			vec3norm(&right);
			//vec3set (right, 1, 0, 0);
			
			gx_sprite_draw_3d( alien_sprite[ (frame_number/20)% 8  ], &pos, &up, &right, ztrue, ztrue);

		}
		gx_set_active_lights(&light, 1);

//		mt_update(mtree,  &player_camera.camera_pos);
		//gx_vbuffer_draw( mtree->vbuffer, 0, mtree->vbuffer->index_count, gx_triangles, ztrue);

		
//		gx_vbuffer_draw( sheet->vb,  sheet->indexstart[sheetlevel],  sheet->indexstop[sheetlevel], gx_quads, ztrue);
#if 1
		{
			int i;
			for (i=0;i<vec_count(&sectors); i++)
			{
				gx_portal_t* p;
				gx_sector_t* s = vec_get_at(&sectors, i);

				gx_sector_outline(s);
			//	draw_sector(s, zfalse);
				p = s->portals;
				
				while(p)
				{
					//gx_portal_draw_test(p);
					p = p->next_portal;
				}


			}

		}
#endif


		camera_sector = portal_process(camera_sector,  &player_camera, 90.0, frame_number);

		//gx_sector_outline(camera_sector);
		
		//don't let outside sector
	//	restrict_point_to_box( &player_camera.camera_pos, &camera_sector->min, &camera_sector->max);


		frame_number++;
		gx_frame_show();  //show the frame

	}

	vec_cleanup(&sectors);
	

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
