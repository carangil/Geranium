// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include <math.h>
#include "../ztypes.h"
#include "../memory/zmem.h"
#include "../vmath/zmath.h"
#include "../structures/zvector.h"
#include "../structures/zlist.h"
#include "../structures/zstring.h"
#include "../graphics/gx_sys.h"
#include "../graphics/gx_buffers.h"

#include "../graphics/gx_image.h"
#include "../graphics/gx_drawstyle.h"
#include "../graphics/gx_trans.h"
#include "../graphics/gx_light.h"

extern int transmode;



float ang=0;


gx_vbuffer_t* gensphere(int quality){

	vec3 p;
	float s,t;
	float ds =2*PI/quality;
	float dt = PI/quality;

	
	gx_vbuffer_t* sbuf = gx_vbuffer_mk((1+quality)*(1+quality)*4, 0, GX_VBUFFER_NORMAL |GX_VBUFFER_COLOR  | GX_VBUFFER_TEXCOORD  );
		
	
	for (s= -PI/2 ; (s+.5*ds)< PI/2; s+=ds) {
		for (t= 0 ; (t+.5*dt)<2*PI; t+=dt) {
					
			
			vec3 p;
			vec3 pds;
			vec3 pdsdt;
			vec3 pdt;
			
			vec3set(p, 		cos(s) * cos(t) , sin(t)*cos(s) , sin(s));
			vec3set(pds,	cos(s+ds) * cos(t) , sin(t)*cos(s+ds) , sin(s+ds));
			vec3set(pdsdt,	cos(s+ds) * cos(t+dt) , sin(t+dt)*cos(s+ds) , sin(s+ds));
			vec3set(pdt,	cos(s) * cos(t+dt) , sin(t+dt)*cos(s) , sin(s));
	
			gx_vbuffer_color4(sbuf, 1, 0,0,1);
			gx_vbuffer_tex2(sbuf, 0,  (s)/PI +.5, (t)/2/PI);
			gx_vbuffer_normal(sbuf, p);
			gx_vbuffer_vertex(sbuf,p);
			
			gx_vbuffer_color4(sbuf, 0, 1,0,1);
			gx_vbuffer_tex2(sbuf, 0, (s)/PI +.5,(t+dt)/2/PI);
			gx_vbuffer_normal(sbuf,pdt);
			gx_vbuffer_vertex(sbuf,pdt);
			
								
			gx_vbuffer_color4(sbuf, 0, 0,1, 1);
			gx_vbuffer_tex2(sbuf, 0, (s+ds)/PI +.5, (t+dt)/2/PI);
			gx_vbuffer_normal(sbuf,pdsdt);
			gx_vbuffer_vertex(sbuf,pdsdt);
			
			
			gx_vbuffer_color4(sbuf, 1, 1,1, 1);
			gx_vbuffer_tex2(sbuf, 0, (s+ds)/PI +.5, (t)/2/PI);
			gx_vbuffer_normal(sbuf,pds);
			gx_vbuffer_vertex(sbuf,pds);
			
			
	
			
		}
		
	}
	
	gx_vbuffer_update(sbuf);
	
	return sbuf;
	
}


int main(int argc, char** argv)
{
	int i,j,k;
    gx_image_t * tex;
	gx_camera_t	player_camera;
	gx_camera_init(&player_camera);

	gx_vbuffer_t* vb = NULL;
	//gx_init(800, 600 , "Test", GX_OPTION_NO_SHADER);
	gx_init(800, 600 , "Test", 0);
	
	gx_vbuffer_t* sph = NULL;

    tex = gx_image_load_tga("../shared/rock.tga");
	
	gx_drawstyle_t* teststyle = gx_drawstyle_mk( tex);
	gx_drawstyle_t* teststylenotex = gx_drawstyle_mk( NULL);
	
	
	ram_free(tex);  //reference counts by owning objects keep these alive
			
	gx_environment_t* testenv = gx_environment_mk();
	
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
	
	
	gx_clear_color(.3,.5,.5,1);


	gx_setup_3d( 90.0, 4.0/3.0, .1, 100);

	//create vbuffer

	
	vb = gx_vbuffer_mk(3*3*3*3, 0, /*GX_VBUFFER_COLOR  |*/ GX_VBUFFER_TEXCOORD |GX_VBUFFER_NORMAL  );

	for (i=0;i<3;i++){
		for(j=0;j<3;j++){
			for (k=0;k<3;k++) {
				gx_vbuffer_color4(vb,1,1,1,1);
                gx_vbuffer_tex2(vb,0,   0.0,  0.0);
				gx_vbuffer_normal3(vb, 0, 0, 1);
				gx_vbuffer_vertex3(vb,i,j,-k-1);
                                
                                
				gx_vbuffer_color4(vb,1,0,0,1);
                gx_vbuffer_tex2(vb,0,1.0,0.0);
				gx_vbuffer_normal3(vb, 0, 0, 1);
				gx_vbuffer_vertex3(vb,i+1,j,-k-1);
                
				gx_vbuffer_color4(vb,0,1,1,1);
                gx_vbuffer_tex2(vb,0,0.0,1.0);
				gx_vbuffer_normal3(vb, 0, 0, 1);
				gx_vbuffer_vertex3(vb,i,j+1,-k-1.1);
			}
		}
	}
	gx_vbuffer_update(vb); 

	sph = gensphere(16);


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
#define SSS .01
			if (gx_key_state('w')) delta_pos.vec3z+=SSS;
			if (gx_key_state('s')) delta_pos.vec3z=-SSS;
			if (gx_key_state('a')) delta_pos.vec3x=-SSS;
			if (gx_key_state('d')) delta_pos.vec3x=+SSS;
			if (gx_key_state('r')) delta_pos.vec3y=+SSS;
			if (gx_key_state('f')) delta_pos.vec3y=-SSS;

			char x = gx_getkey();
			
			if (x==('T')) transmode ^=1;
			
		//	if (x==('P')) shademode ^=1;
			if (x=='~') exit(0);

			if (gx_key_state('q')) delta_roll=-.02;
			if (gx_key_state('e')) delta_roll=.02;

			if (gx_key_state('z')) delta_yaw=-.02;
			if (gx_key_state('x')) delta_yaw=.02;

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
//
GX_TRACE
	
		//clear screen		
		gx_frame_clear(ZTRUE, ZTRUE);

		teststyle->specular_exponent=40.0;
		vec3set(teststyle->specular_color, 1, 1, 1);
		
		
	
			
		teststyle->shadergroup =sg;
		teststylenotex->shadergroup =sg;
			
	
		
		
		GX_TRACE
		gx_set_environment(testenv);
		GX_TRACE
		gx_drawstyle_activate(teststyle);
		GX_TRACE
	
		
	
	
		gx_vbuffer_draw(vb, 0, 3*3*3*3 , gx_triangles, ZFALSE);
		GX_TRACE
		
			
		GX_TRACE

		
		
	/*	
			vec3 p;


		
			vec3set(p, -2,0,-2);
			gx_translate(&p);

			gx_rotate_z(ang *DEGREE);


			gx_scale3(sin(DEGREE*ang), cos(DEGREE*ang), sin(DEGREE*ang)+cos(DEGREE*ang));

			vec3set(p, -.5,0,0);
			gx_translate(&p);

			gx_rotate_y(.5*ang*DEGREE);

			vec3set(p, 0,.1,0);
			gx_translate(&p);
	
			gx_rotate_x( .25*ang*DEGREE);

			

*/
		
			
		
				


		ang+=1;

	
	GX_TRACE
		gx_drawstyle_activate(teststyle);
			{
			vec3 p;
			vec3set(p, -2, -1, -2);
			gx_translate(&p);
		}
		GX_TRACE
		gx_rotate_y(ang * DEGREE);
		GX_TRACE
		gx_rotate_x(.1*ang * DEGREE);
		GX_TRACE
		
		//gx_set_active_textures(NULL, 0);
		gx_vbuffer_draw(sph, 0, sph->vertex_count  , gx_quads, ZFALSE);
		
		GX_TRACE
		
		gx_frame_show();  //show the frame
GX_TRACE

	}
	gx_disable();

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
