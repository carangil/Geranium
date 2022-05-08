// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include <math.h>
#include "../ztypes.h"
#include "../memory/zmem.h"
#include "../vmath/zmath.h"
#include "../graphics/gx_sys.h"
#include "../graphics/gx_image.h"
#include "../graphics/gx_buffers.h"
#include "../graphics/gx_light.h"
#include "../structures/zvector.h"
#include "../graphics/gx_drawstyle.h"
//#include "../graphics/gx_misc.h"
//#include "../graphics/gx_mesh.h"
//#include "../graphics/gx_quadpatch.h"

#define STARCOUNT 2000

//#define FADE_PATCHES
//#define ALPHA_SPEED  .01

#if 0
gx_quadpatch_t* closest = NULL;

void do_detail( 	 gx_quadpatch_t* dest, gx_quadpatch_t* source, 
						int a_start, int a_end, int b_start, int b_end)
{
	int a;
	int b;
	int w = (a_end-a_start-1) * 2+1 ;
	int h = (b_end-b_start-1) * 2+1 ;
	int x;
	int y;
	vec3 p;
	int div;
	float fdiv;
	float s,t;

	printf(" GENERATION %d\n", source->generation);

	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			a = x/2 + a_start;
			b = y/2 + b_start;

			vec3mov(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a,b)));
			div = 1;
			fdiv = sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a,b))));
			s = gx_vbuffer_s(source->vb, qp_vindex( source, a, b),0);
			t = gx_vbuffer_t(source->vb, qp_vindex( source, a, b),0);
 

			if ((x & 1) && ( (a+1)< a_end) ) //if odd x
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b))));
				s += gx_vbuffer_s(source->vb, qp_vindex( source, a+1, b),0);
				t += gx_vbuffer_t(source->vb, qp_vindex( source, a+1, b),0);
			}

			if ((y & 1) && ( (b+1)< b_end) ) //if odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a,b+1))));
				s += gx_vbuffer_s(source->vb, qp_vindex( source, a, b+1),0);
				t += gx_vbuffer_t(source->vb, qp_vindex( source, a, b+1),0);
			}

			if ((y & 1) && ( (b+1)< b_end)  && (x & 1) && ( (a+1)< a_end)) //if odd x and odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b+1))));
				s += gx_vbuffer_s(source->vb, qp_vindex( source, a+1, b+1),0);
				t += gx_vbuffer_t(source->vb, qp_vindex( source, a+1, b+1),0);
			}


			vec3scale(p, 1.0/div);  //unit scale
	
			*gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y)) = p;

			//texcoord 0:
			gx_vbuffer_s(dest->vb, qp_vindex( dest, x, y),0) = s / div;
			gx_vbuffer_t(dest->vb, qp_vindex( dest, x, y),0) = t / div;

		}
	}

	gx_quadpatch_norm(dest);
	//add noise
	printf(" dsize of %f km (if planet is earth)\n", source->dsize * 6371);
	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			vec3* p;
			vec3* n;

			p = gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y));

			vec3sub(*p, source->origin); 
			{	float r = randf()-0.5;
				vec3scale(*p,  1.0  -  r*   source->dsize) ;
			}
			vec3add(*p, source->origin);
		
		}

	}
	if (source->generation < 10)
		dest->dsize*=1.3;	
}

#endif



#define PLANET_SIZE 10
int main(int argc, char** argv)
{
	// IO variables
	zchar keypress=0;
	zint32 mouse_x=0;
	zint32 mouse_y=0;
	zbool  mouse_relative=zfalse;
	int i;
	int j;
	int k;
	//game/graphics variables
	gx_camera_t	player_camera;
	vec3		camera_inertia;

	int patchlevel=0;

	//need a starfield (we ARE in space)
	gx_vbuffer_t*	starfield = NULL;

	gx_quadpatch_t* rootqa[6];

	gx_quadpatch_sys_t qpsys;
	gx_drawstyle_t ds_qa;
	gx_light_t* light = NULL;


	gx_image_t		*spacerock = NULL;  //holds rock texture for asteroids
	gx_drawstyle_t	spacerock_ds;   //drawstyle for the asteroid
	
	
	gx_image_t* hf = NULL;


	gx_image_t* atmosphere = NULL;


	
	int numpatches=0;

	gx_vbuffer_t* vb =  NULL;

	gx_quadpatch_t* qp = NULL;

	vec3 skycolor;
	vec3 fogcolor;

	vec3set(skycolor, .7,.2,.1);
	vec3set(fogcolor, .6,.6,.6);

	
	if (!gx_quadpatch_sys_init(&qpsys))
		printf(" Could not init qpsys\n");



	//Initialize graphics
	gx_init(800, 600 , " Quad Patch");
	//gx_clear_color(0,0,0,1);

	gx_clear_color(.7,.7,.7,1);
	
	//Load assets
	spacerock  = gx_image_load_tga( "rock.tga");
//	spacerock_ds.textures = &spacerock;
//	spacerock_ds.numtextures=1;
	vec_mk(&spacerock_ds.textures,1);
	vec_add(&spacerock_ds.textures, spacerock);

	//initialize game data
	gx_camera_init(&player_camera);
	vec3set(camera_inertia, 0,0,0);

	player_camera.camera_pos.named.z +=2 * PLANET_SIZE;
	player_camera.camera_pos.named.y +=1 * PLANET_SIZE;

	//make light
	{
		vec3 lpos;
		vec3 lcolor;
		vec3 lamb;

		vec3set (lpos, sqrt(3)/3,sqrt(3)/3,sqrt(3)/3);
		vec3set(lcolor, 0.7,0.7,0.7);
		vec3set(lamb, .1,.1,.1);

		light = gx_light_mk( gx_light_directional, &lpos, &lcolor, &lamb );
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


	


	//generate planet terrain

	for (k=0;k<1;k++) {

		vec3 origin;

		//vec3set(origin, 10*k,0,0);

		vec3set(origin, 0,0,0);

	//all faces
	for (i=0;i<6;i++)
	{

		//make gx_quadpatch
//		qp = gx_quadpatch_mk(257,257, do_detail); 
		//qp = gx_quadpatch_mk(129,129);
		//qp = gx_quadpatch_mk(65,65, do_detail);  //make mesh of 64 by 64 quads( 65by65 points)
		qp = gx_quadpatch_mk(33,33, do_detail);
//		qp = gx_quadpatch_mk(17,9, do_detail);
//		qp = gx_quadpatch_mk(17,9, NULL);
		//qp = gx_quadpatch_mk(9,9);
		//qp = gx_quadpatch_mk(3,3);
		
		
		//qp->size = .3; //start at root size
		//qp->dsize = .1;

		qp->onlevel = 1;

		qp->size=  8.0 * PLANET_SIZE* PLANET_SIZE;
		//qp->dsize = .015  ;
	//	qp->dsize = .006  ;
	qp->dsize=.00130;

		qp->origin = origin;

//		qp->size =  qp->size / ((qp->w-1) * (qp->h-1));
//		qp->dsize =  qp->dsize / (qp->w );
		
#define  2018TOP 0
#define  2018BOTTOM 1
#define  2018FRONT 2
#define  2018BACK 3
#define  2018LEFT 4
#define  2018RIGHT 5
		{
			float d;

			int a,b;
			for (a=0;a < qp->w;a++)
			{
				for (b=0;b<qp->h;b++)
				{
					vec3* vv = gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b));

					float fa;
					float fb;

					fa = ((a-qp->w/2)/  (float) (qp->w-1)) *2 ;
					fb  =  ((b-qp->h/2)/ (float) (qp->h-1)) *2 ;

			
					
					gx_vbuffer_s(qp->vb, qp_vindex( qp, a,b),0) = fa /2+1.0;
					gx_vbuffer_t(qp->vb, qp_vindex( qp, a,b),0) = fb /2+1.0;

					srandf(fa+fb+a+b);
					
					switch (i)
					{
					case  2018TOP: 
						vec3set(*vv, fa, 1, -fb);
						break;

					case  2018BOTTOM:
						vec3set(*vv, fa, -1, fb);
						break;


					case  2018FRONT:
						vec3set(*vv, fa, fb, 1);
						break;

					case  2018BACK:
						vec3set(*vv, -fa, fb, -1);
						break;

						
					case  2018LEFT:
						vec3set(*vv, -1, fb, fa);
						break;

					case  2018RIGHT:
						vec3set(*vv, 1, fb, -fa);
						break;

					}

					//now normalize
			
					{
						vec3 vvf;

//						vec3set(vvf, ((int) (5*vv->named.x)) /5.0,  ((int) (7*vv->named.y)) /7.0,  ((int) (3*vv->named.z)) /3.0   );
//						d = vec3abs_sq(vvf); 

					}

					d = vec3abs_sq(*vv);
					d=sqrt(d);
					vec3scale(*vv, PLANET_SIZE/d);

					//make a little rough
					if (k!=1)
						vec3scale(*vv, 1 - randf() * qp->dsize);

			//		vv->named.z -= 2;
				//	vv->named.x += 60*k ;


					if (k==1)
					{
							vec3scale(*vv, 1.1);

					}

					vec3add(*vv, origin);


					

				
					
				//	vv->named.y -= (rand() & 0xff) / 255.0 * .05;

					
					//SCALE HUGE
					//vec3scale(*vv, 5000 );

						//do normal
					vv = gx_vbuffer_n( qp->vb, qp_vindex( qp, a,b));
					vec3set(*vv, 0,1,0);

				}
			}

			qp->center = *gx_vbuffer_v( qp->vb, qp_vindex( qp, qp->w/2,qp->h/2));
			

			//qp->mesh = gx_mesh_def( qp->vb, NULL, qp->startindex, qp->endindex, ztrue);
			
			gx_quadpatch_norm(qp);
			//gx_vbuffer_update(qp->vb);
			
			
		
			//vec_add(quadpatches, qp);
//			vec_add(toplevel_quadpatches, qp);


			//add to the quadpatch system

			gx_quadpatch_sys_add(&qpsys, qp); //add to the system
	
			


			rootqa[i] = qp;

			
			
		}
	}
	
	

	//loop	
	gx_quadpatch_sew(rootqa[ 2018FRONT], GX_EDGE_LEFT, rootqa[ 2018LEFT], GX_EDGE_RIGHT,0);
	gx_quadpatch_sew(rootqa[ 2018FRONT], GX_EDGE_RIGHT, rootqa[ 2018RIGHT], GX_EDGE_LEFT,0);
	gx_quadpatch_sew(rootqa[ 2018BACK], GX_EDGE_LEFT, rootqa[ 2018RIGHT], GX_EDGE_RIGHT,0);
	gx_quadpatch_sew(rootqa[ 2018BACK], GX_EDGE_RIGHT, rootqa[ 2018LEFT], GX_EDGE_LEFT,0);


	//top
	gx_quadpatch_sew(rootqa[ 2018TOP], GX_EDGE_BOTTOM, rootqa[ 2018FRONT], GX_EDGE_TOP,0);
	gx_quadpatch_sew(rootqa[ 2018TOP], GX_EDGE_LEFT, rootqa[ 2018LEFT], GX_EDGE_TOP,1);
	gx_quadpatch_sew(rootqa[ 2018TOP], GX_EDGE_RIGHT, rootqa[ 2018RIGHT], GX_EDGE_TOP,0);
	gx_quadpatch_sew(rootqa[ 2018TOP], GX_EDGE_TOP, rootqa[ 2018BACK], GX_EDGE_TOP,1);


	//bottom
	gx_quadpatch_sew(rootqa[ 2018BOTTOM], GX_EDGE_TOP, rootqa[ 2018FRONT], GX_EDGE_BOTTOM,0);
	gx_quadpatch_sew(rootqa[ 2018BOTTOM], GX_EDGE_LEFT, rootqa[ 2018LEFT], GX_EDGE_BOTTOM,0);
	gx_quadpatch_sew(rootqa[ 2018BOTTOM], GX_EDGE_RIGHT, rootqa[ 2018RIGHT], GX_EDGE_BOTTOM,1);
	gx_quadpatch_sew(rootqa[ 2018BOTTOM], GX_EDGE_BOTTOM, rootqa[ 2018BACK], GX_EDGE_BOTTOM,1);




	//update
	for (i=0;i<6;i++)
	{

#ifdef QUADARRAY_SKIRTS
		gx_quadpatch_skirt(rootqa[i]);
#endif
		gx_vbuffer_update(rootqa[i]->vb);

	}



	}



	//The game loop 
	for(;;)  
	{
		float dist;

 		gx_window_event();  //handles any window events (I/O)

		//set up projection matrix for this frame

		//how close are we to the planet?
			
	//	dist = vec3abs_sq(player_camera.camera_pos);
	//	dist = sqrt(dist);
	//	printf(" ### DIST %f", dist);

	//	dist = (.997- dist)*.01;
	//	if (dist > .01 || dist <0) dist = .01;
	//	printf(" ### clossness %f", dist);

 		gx_setup_3d( 80.0f,  gx_frame_get_dimensions(NULL,NULL),.000001*PLANET_SIZE, 10.0f* PLANET_SIZE);
		
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
//			patchlevel= (patchlevel +1) % patch->levels;
		}
		else if (keypress=='-')
		{
//			if (patchlevel > 0)
///				patchlevel--;
			
		}else if (keypress == 'h')
		{
			vec3scale(camera_inertia, .5);
		}
			


		//camera control
		{
			zfloat32 delta_yaw		= 0.0;
			zfloat32 delta_pitch	= 0.0;
			zfloat32 delta_roll		= 0.0;
		
			vec3	 delta_pos;
			vec3set	 (delta_pos, 0,0,0);
	
			//speed of motion
#define SSS .001
			if (gx_key_state('w')) delta_pos.vec3z+=SSS;
			if (gx_key_state('s')) delta_pos.vec3z=-SSS;
			if (gx_key_state('a')) delta_pos.vec3x=-SSS;
			if (gx_key_state('d')) delta_pos.vec3x=+SSS;
			if (gx_key_state('r')) delta_pos.vec3y=+SSS;
			if (gx_key_state('f')) delta_pos.vec3y=-SSS;


			

			if (gx_key_state('x')) {
				vec3set(camera_inertia, 0,0,0);
				delta_roll=0;
				delta_yaw=0;
				delta_pitch=0;
			}

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

			vec3madd( player_camera.camera_pos, .1 , camera_inertia);

			//lets spin camera  (relative to its own coord system)

			delta_pitch += mouse_y*.003;
			delta_yaw += mouse_x*.003;

			gx_spin(ztrue, delta_yaw, delta_pitch, delta_roll,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);

		}


		if (1){
			vec3 pos;

			float dist;
			//vec3 fogc;
		//	vec3 skyc;
		//	vec3set(fogc, .7,.5,.5);
		//	vec3set(skyc, .5,.3,.3);

			vec3mov(pos, player_camera.camera_pos);
			//subtract planet center( currently just 0,0,0)
			
			dist = vec3abs_sq(pos);
			dist = sqrt(dist);

			gx_light_fog(NULL, 0, 0);
			gx_clear_color(0,0,0,0);


			if (dist < 1.2)
			{
				float d2;
				if (dist > 1.1)
				{
					d2 = 1-fabs(dist - 1.1) / (1.2-1.1);
					d2 +=.1;
					if (d2 > 1)
							d2=1;
//					printf( "atmos dist %f\n", d2);
					//gx_clear_color(d2*skycolor.array[0], d2*skycolor.array[1], d2*skycolor.array[2], 0);
				}
				else
				{
//					gx_clear_color(skycolor.array[0], skycolor.array[1], skycolor.array[2], 0);
				}

				

				
				
				//gx_clear_color(1,0,0,0);
				{
					//float t = (dist -1) / (1.1-1);
					//if (t<0) t=0;

					//gx_light_fog(&skycolor,0,  t * 1 + (1-t)*.5   );

					float t = 1-dist;
					if (t>1)
						t=1;
					if (t<0)
						t=0;

//					gx_light_fog(&skycolor, 0, (dist/3) *t  + (10)*(1-t)  );
				}
			}


			//gx_light_fog(&fogc, 0, .3);
/*
			if (dist > 1.6)
			{
				gx_clear_color(0,0,0,0);
				gx_light_fog(NULL,0,0);
			}
			else if (dist < 1.2)
			{
				gx_clear_color(skyc.array[0], skyc.array[1], skyc.array[2],0);
				gx_light_fog(&fogc, 0,.3);
			}
			else
			{
				dist = dist - 1.2;
				dist = 1-fabs(dist *10);
				gx_clear_color(dist*skyc.array[0], dist*skyc.array[1], dist*skyc.array[2],0);
				gx_light_fog(&fogc, 0, 1.6-dist);
			}
*/

		}
		//clear screen		
		gx_frame_clear(ztrue,ztrue);
		
		//draw starfield
		gx_camera_home();
		gx_camera_pos_rot(NULL,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward); 
		gx_zbuffer(zfalse);
		
		
		gx_set_active_lights(NULL,0);
		gx_drawstyle_activate(NULL) ;
		gx_vbuffer_draw(starfield, 0, starfield->vertex_count, gx_points, zfalse);

		gx_zbuffer(ztrue);
		//render here

		gx_camera_pos_rot( &player_camera.camera_pos,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);
	
		//gx_drawstyle_activate(&spacerock_ds);

//		mt_update(mtree,  &player_camera.camera_pos);
		
		
		//gx_vbuffer_draw( mtree->vbuffer, 0, mtree->vbuffer->index_count, gx_triangles, ztrue);

	/*	{	
			vec3 spos;
			//vec3set(spos, 0,0,0);
		//	vec3mov(spos, player_camera.camera_forward);
			vec3scale(spos, -.5);
			gx_sprite_draw_3d( atmos_s, &spos, &player_camera.camera_up, &player_camera.camera_right, ztrue, ztrue);

		}
		*/

		


		gx_set_active_lights(&light, 1);
		
	//	gx_set_active_lights(NULL,0);
		
		ram_clear(&ds_qa, sizeof(ds_qa));
		
		vec3set( ds_qa.specular_color, 1,1, 1);
		ds_qa.specular_exponent = 10;
		ds_qa.blending = gx_blend_nothing;
		ds_qa.alpha = .5;

		memcpy(&ds_qa.textures, &spacerock_ds.textures, sizeof(spacerock_ds.textures));
	//	ds_qa.textures = spacerock_ds.textures;
	//	ds_qa.numtextures = 1;



		gx_quadpatch_sys_eval(&qpsys, &player_camera);

		//gx_drawstyle_activate(&ds_qa);

		gx_drawstyle_activate(NULL) ;
		gx_quadpatch_sys_draw(&qpsys, &player_camera);
				
		//printf(" %d opaque patches \n", vec_count(quadpatches));
		//draw opaque patches
		


		ds_qa.use_constant_alpha = 1;


//		printf(" %d skirted %d nonskirted   %d drawn  %d total\n", skirted, nonskirted, drawn, total);

		skirted=nonskirted=drawn=0;
		gx_frame_show();  //show the frame


	}

	//ram_free(toplevel_quadpatches);
	//ram_free(quadpatches);
	//ram_free(quadpatches_fadeout);
	//ram_free(quadpatches_fadein);

	gx_quadpatch_sys_cleanup(&qpsys);  //delete the root patches and all their descentandts

	ram_free(starfield);
	gx_disable();
	ram_free(light);
	vec_cleanup(&spacerock_ds.textures);

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
