// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include <math.h>


#include "ztypes.h"
#include "zmem.h"
#include "zmath.h"
#include "zvector.h"
#include "zlist.h"
#include "zstring.h"
#include "gx_sys.h"
#include "gx_buffers.h"
#include "gx_image.h"
#include "gx_drawstyle.h"
#include "gx_trans.h"
#include "gx_light.h"
#include "gx_quadpatch.h"

#include "gx_misc.h"
#include "gen.h"


#if 1


#endif



int main(int argc, char** argv)
{


	int run_tesselator=1;
	gx_image_t * tex;
	gx_camera_t	player_camera;
	gx_camera_init(&player_camera);

	zvec_t sectors;
	gx_sector_t *camera_sector=NULL;
	

	//gx_init(800, 600 , "Test", GX_OPTION_NO_SHADER);
	gx_init(800, 600 , "Test", 0);



	tex = gx_image_load_tga("../shared/label.tga");

	gx_drawstyle_t* teststyle = gx_drawstyle_mk( tex);
	gx_drawstyle_t* teststylenotex = gx_drawstyle_mk( NULL);


	ram_free(tex);  //reference counts by owning objects keep these alive

	gx_environment_t* testenv = gx_environment_mk();
	gx_environment_t* nolights = gx_environment_mk();
	
	//	gx_shadergroup_t* sg = gx_shader_source("@../graphics/shader.v", "@../graphics/shader.f");

	gx_shadergroup_t* sg = NULL;  //should force use of default shader

	{
		vec3 p,c,ca;
		gx_light_t* li;
		vec3set(p, 1, 2, 1);
		vec3set(c, .1,1,.5);
		vec3set(ca, .1, .1, .3);

		li = gx_light_mk(gx_light_point, &p, &c, &ca  );

		gx_light_set_attenuation( li, ZTRUE, 10, 10, 1);

		zvec_add_or_free(&testenv->lights, li);

		vec3set(c, 1,1,0);
		vec3set(p, 0, -2, 1);
		//li = gx_light_mk(gx_light_directional, &p, &c, &ca  );

		//zvec_add_or_free(&testenv->lights, li);

	}


	gx_clear_color(.1,.1,.1,1);


	gx_setup_3d( 90.0, 4.0/3.0, .1, 100);



	//create gx_quadpatch

	#define PLANET_SIZE 3


	gx_quadpatch_sys_t qpsys;

	gx_quadpatch_sys_init(&qpsys);



	vec3 origin;
	//vec3set (origin, 0,-3,-3);
	vec3set (origin, 0, 0,0);


	gx_quadpatch_detailer_f detailers[] = {
		gen_random_detail,gen_random_detail,gen_random_detail,
		gen_random_detail,gen_random_detail,gen_random_detail};
		
	
	gen_qpcube(&qpsys,
				&origin,
				17,
				2.0,
				NULL,
				NULL,
				detailers,
				/*GEN_SPHERE | GEN_INSIDE*/ GEN_INSIDE,
				30, 0.07
  			);

#define S_NX_NY_NZ	0
#define S_NX_PY_NZ	1
#define S_PX_PY_NZ	2
#define S_PX_NY_NZ	3
#define S_NX_NY_PZ	4
#define S_NX_PY_PZ	5
#define S_PX_PY_PZ	6
#define S_PX_NY_PZ	7
	
	/* generate 1 sector */
	{
		vec3 min,max;
	//	vec3set(min,-1,-1,-1);
	//	vec3set(max,1,1,1);
	
		
		int a,b;
		
		zvec_mk(&sectors, 16);
		
		
		
		for (b=0;b<10;b++) {
			
			for (a=0;a<10;a++) {
				
				
			vec3set(min, -.5+ a,-.5,-b);
			vec3set(max, -.5+a+1,.5,-b+1);
			gx_sector_t* sector = gx_sector_mk(&min,&max);
		
			if (!camera_sector) 
				camera_sector = sector; //spawn the camera here
		
			zvec_add(&sectors, sector);
			}
		}
		
	}
	
	//now make all the portals
	{
		int a=0;
		int b=0;
		for (b=0;b<9;b++) {
			vec3 pos;
			vec3set(pos, a, 0, -b);
			gx_sector_t* s = zvec_get_at(&sectors, (b*10)+a );
			gx_sector_t* t =zvec_get_at(&sectors, (b+1)*10 + a);
			gx_sector_add_portal_sphere(s,&pos, sqrt(2)/2, t, NULL);
			
		
			
		}
	}
	
	printf("start\n");
	
	gx_mouse_capture(ZTRUE);
	//The game loop 
	for(;;)  
	{


		gx_window_event();  //handles any window events (I/O)


		{
			//move the light position around

			//	vec3set(zvec_elements_as(gx_light_t*, &testenv->lights)[0]->position, cos(ang*DEGREE * .1), 0, 0);

		}

		//camera control
		{
			zfloat32 delta_yaw		= 0.0;
			zfloat32 delta_pitch	= 0.0;
			zfloat32 delta_roll		= 0.0;

			vec3	 delta_pos;
			vec3set	 (delta_pos, 0,0,0);

			int mouse_x, mouse_y;
			zbool	mouse_relative;

			//speed of motion
#define SSS .005
			if (gx_key_state('w')) delta_pos.vec3z+=SSS;
			if (gx_key_state('s')) delta_pos.vec3z=-SSS;
			if (gx_key_state('a')) delta_pos.vec3x=-SSS;
			if (gx_key_state('d')) delta_pos.vec3x=+SSS;
			if (gx_key_state('r')) delta_pos.vec3y=+SSS;
			if (gx_key_state('f')) delta_pos.vec3y=-SSS;

			char x = gx_getkey();

			if (x=='m') gx_mouse_capture(ZFALSE);
			if (x=='M') gx_mouse_capture(ZTRUE);
			if (x=='t') run_tesselator^=1;


			if (x=='~') exit(0);

			if (gx_key_state('q')) delta_roll=-.005;
			if (gx_key_state('e')) delta_roll=.005;

			if (gx_key_state('z')) delta_yaw=-.005;
			if (gx_key_state('x')) delta_yaw=.005;

			if (gx_key_state('g')) delta_pitch=-.02;
			if (gx_key_state('b')) delta_pitch=.02;

			//move camera using camera's basis
			vec3madd(player_camera.pos, delta_pos.vec3x, player_camera.rot.x_axis);
			vec3madd(player_camera.pos, delta_pos.vec3y, player_camera.rot.y_axis);
			vec3madd(player_camera.pos, delta_pos.vec3z, player_camera.rot.z_axis);


			//lets spin camera  (relative to its own coord system)
			gx_mouse_pos(&mouse_x, &mouse_y, &mouse_relative);

			if (!mouse_relative)
			{
				//if some some reason we get an absolute mouse position, clear it out
				mouse_x=0;
				mouse_y=0;
			}

			delta_pitch += mouse_y*.003;
			delta_yaw += mouse_x*.003;

			if (fabsf(delta_pitch) < .3 && fabsf(delta_yaw) < .3)  //large mouse movements are probably just entering/leaving window
				gx_spin(ZTRUE, delta_yaw, delta_pitch, delta_roll, &player_camera.rot);

		}	





		GX_TRACE


		gx_camera_pos_rot( &player_camera.pos,&player_camera.rot.x_axis, &player_camera.rot.y_axis, &player_camera.rot.z_axis);


		gx_env_evaluate_lights(testenv);
	
		GX_TRACE

		//clear screen		
		gx_frame_clear(ZTRUE, ZTRUE);

		//teststyle->specular_exponent=40.0;
		//vec3set(teststyle->specular_color, 1, 1, 1);
		vec3set(teststyle->specular_color, 0, 0, 0);

		teststyle->shadergroup =sg;
		teststylenotex->shadergroup =sg;


		GX_TRACE
		gx_set_environment(testenv);
		GX_TRACE
		gx_drawstyle_activate(teststyle);
		GX_TRACE


	/*
		if (run_tesselator)
			gx_quadpatch_sys_eval(&qpsys, &player_camera);
*/
	
		//gx_quadpatch_sys_draw(&qpsys, &player_camera);
	
	
		gx_set_environment(nolights);
		gx_drawstyle_activate(teststylenotex);

		
		gx_traverse_sectors(&player_camera, camera_sector ); 
/*
		gx_sector_outline(camera_sector,ZTRUE);
		{
			int a=0;
			int b=0;
			gx_sector_t* t =zvec_get_at(&sectors, (a+1)*10+b);
			gx_sector_outline(t,ZTRUE);
		}*/
#if 0
		gx_sector_outline(camera_sector,ZTRUE);
	
		{
				gx_sector_t * s;
				int i;
				for (i=0;i<zvec_count(&sectors);i++) {
					gx_sector_t * s = zvec_elements_as(gx_sector_t*, &sectors)[i];
					
					gx_sector_outline(s,ZTRUE);
					
				}
		}
#endif


		gx_frame_show(); 
		
		GX_TRACE

	}
	gx_disable();

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
