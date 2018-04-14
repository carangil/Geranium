// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
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



float ang=0;


gx_vbuffer_t* gensphere(int quality){


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

			vec3set(p, 	cos(s) * cos(t) , sin(t)*cos(s) , sin(s));
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


//build cubes from quadpatches

#define CUBE_TOP	0
#define CUBE_BOTTOM	1
#define CUBE_FRONT	2
#define CUBE_BACK	3
#define CUBE_LEFT	4
#define CUBE_RIGHT	5

//CUBE_INSIDE reorders vertices so culling order is correct
//from viewing inside the CUBE_BACK

#define GEN_INSIDE 1
#define GEN_SPHERE		2

zbool gen_qpcube(gx_quadpatch_sys_t* qpsys,
				 vec3 * origin,
				 int tessel,
				 float radius,
				  gx_drawstyle_t* ds[6],
				  gx_quadpatch_t* qpo[6],
					gx_quadpatch_detailer_f detailer[6],
				 int flags,
				 float size, float dsize
 				)
{
	int i;
	int a,b;
	int astart, aend, ainc;

	gx_quadpatch_t* qp;
	float d;
	gx_quadpatch_t* qps[6];
	
	if(!qpo)
		qpo = qps;
	
	float flip=1;
	
	if (flags & GEN_INSIDE) {
			flip = -1;
			//flip some things when inside
	}
	
	
	for (i=0;i<6;i++)
	{
		
		qpo[i] = qp =  gx_quadpatch_mk(tessel,tessel, detailer[i]);

		if (flags|GEN_INSIDE)
			qp->skirtflip=ZTRUE;
		
		for (b=0;b<tessel;b++)
		{
			for (a=0; a<tessel;a++)
			{

				vec3 v;

				float fa;
				float fb;

				fa = ((a-qp->w/2)/  (float) (qp->w-1)) *2 ;
				fb  =  ((b-qp->h/2)/ (float) (qp->h-1)) *2 ;

				gx_vbuffer_tex2(qp->vb, 0, a / (float)(tessel-1), b / (float)(tessel-1) );

				switch (i)
				{
					case CUBE_TOP: 
						vec3set(v, fa, 1, -fb * flip);
						break;

					case CUBE_BOTTOM:
						vec3set(v, fa, -1, fb * flip);
						break;

						
						

					case CUBE_FRONT:
						vec3set(v, fa, fb, 1*flip);
						break;

					case CUBE_BACK:
						vec3set(v, -fa, fb, -1*flip);
						break;


					case CUBE_LEFT:
						vec3set(v, -1, fb, fa*flip);
						break;

					case CUBE_RIGHT:
						vec3set(v, 1, fb, -fa*flip);
						break;

				}

				//now normalize


				if (flags & GEN_SPHERE) {
					d = vec3abs_sq(v);
					d=sqrt(d);
					vec3scale(v, radius/d);
				}
				else {
					vec3scale(v, radius);
				}

				//make a little rough
				//if (k!=1)
				//vec3scale(v, 1 - randf() * qp->dsize);

				qp->dsize=dsize;

				vec3add(v, *origin);

				gx_vbuffer_vertex(qp->vb, v);

				
			}
		}
	
		qp->size = size;
	
		qp->origin=*origin;
		gx_quadpatch_norm(qp);
		
	}
	
	
	//sew all together
	//somehow both inside and outside came out the same, i don't know how
	//i thought for sure this was going to be confusing 
			
	//loop	
	gx_quadpatch_sew(qpo[CUBE_FRONT], GX_EDGE_LEFT, qpo[CUBE_LEFT], GX_EDGE_RIGHT,0,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_FRONT], GX_EDGE_RIGHT, qpo[CUBE_RIGHT], GX_EDGE_LEFT,0,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_BACK], GX_EDGE_LEFT, qpo[CUBE_RIGHT], GX_EDGE_RIGHT,0,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_BACK], GX_EDGE_RIGHT, qpo[CUBE_LEFT], GX_EDGE_LEFT,0,ZTRUE);


	//top
	gx_quadpatch_sew(qpo[CUBE_TOP], GX_EDGE_BOTTOM, qpo[CUBE_FRONT], GX_EDGE_TOP,0,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_TOP], GX_EDGE_LEFT, qpo[CUBE_LEFT], GX_EDGE_TOP,1,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_TOP], GX_EDGE_RIGHT, qpo[CUBE_RIGHT], GX_EDGE_TOP,0,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_TOP], GX_EDGE_TOP, qpo[CUBE_BACK], GX_EDGE_TOP,1,ZTRUE);


	//bottom
	gx_quadpatch_sew(qpo[CUBE_BOTTOM], GX_EDGE_TOP, qpo[CUBE_FRONT], GX_EDGE_BOTTOM,0,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_BOTTOM], GX_EDGE_LEFT, qpo[CUBE_LEFT], GX_EDGE_BOTTOM,0,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_BOTTOM], GX_EDGE_RIGHT, qpo[CUBE_RIGHT], GX_EDGE_BOTTOM,1,ZTRUE);
	gx_quadpatch_sew(qpo[CUBE_BOTTOM], GX_EDGE_BOTTOM, qpo[CUBE_BACK], GX_EDGE_BOTTOM,1,ZTRUE);

	
	//generate normals and add to qpsystem
	
	for (i=0;i<6;i++)
	{
		
		gx_quadpatch_sys_add(qpsys, qpo[i]);
	}
	

	return ZTRUE;  
}




#if 1

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
	vec3 q;
	vec3 r;
	int div;

	float s,t;
	float tmp;

	printf(" GENERATION %d\n", source->generation);

	for (y=0;y<h;y++) {
		for (x=0;x<w;x++){

			a = x/2 + a_start;
			b = y/2 + b_start;

//			vec3mov(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a,b)));
			gx_vbuffer_get_v(p, source->vb, qp_vindex( source, a,b));
			
			div = 1;
		//	fdiv = sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a,b))));
			
			
			gx_vbuffer_get_s(s,source->vb, 0,qp_vindex( source, a, b));
			gx_vbuffer_get_t(t,source->vb, 0,qp_vindex( source, a, b));


			if ((x & 1) && ( (a+1)< a_end) ) //if odd x
			{
				
				gx_vbuffer_get_v(q, source->vb, qp_vindex( source, a+1,b));
				vec3add(p,q);
			
				div++;
	
				gx_vbuffer_get_s(tmp, source->vb,0, qp_vindex( source, a+1, b));
				s+=tmp;
				
				gx_vbuffer_get_t(tmp, source->vb,0, qp_vindex( source, a+1, b));
				t+=tmp;
			}

			if ((y & 1) && ( (b+1)< b_end) ) //if odd y
			{
				gx_vbuffer_get_v(q, source->vb, qp_vindex( source, a,b+1));
				vec3add(p,q);
				
					div++;
				
				gx_vbuffer_get_s(tmp, source->vb,0, qp_vindex( source, a, b+1));
				s+=tmp;
				
				gx_vbuffer_get_t(tmp, source->vb,0, qp_vindex( source, a, b+1));
				t+=tmp;
			}

			if ((y & 1) && ( (b+1)< b_end)  && (x & 1) && ( (a+1)< a_end)) //if odd x and odd y
			{
				gx_vbuffer_get_v(q, source->vb, qp_vindex( source, a+1,b+1));
				vec3add(p,q);
				
				div++;
				
				
				gx_vbuffer_get_s(tmp, source->vb,0, qp_vindex( source, a+1, b+1));
				s+=tmp;
				
				gx_vbuffer_get_t(tmp, source->vb,0, qp_vindex( source, a+1, b+1));
				t+=tmp;
			}


			vec3scale(p, 1.0/div);  //find average

			vec3set(r, randfs(), randfs(), randfs());
			vec3scale(r, dest->dsize);
			vec3add(p,r);
			//random 
			//vec3scale(p, 1 - randf() * dest->dsize);
			
			//*gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y)) = p;

			//texcoord 0:
			//gx_vbuffer_s(dest->vb, qp_vindex( dest, x, y),0) = s / div;
			//gx_vbuffer_t(dest->vb, qp_vindex( dest, x, y),0) = t / div;

			gx_vbuffer_tex2(dest->vb,0, s/div, t/div);
			gx_vbuffer_vertex(dest->vb, p);
			
		}
	}

//	gx_quadpatch_norm(dest);
	//add noise

	printf(" dsize of %f km (if planet is earth)\n", source->dsize * 6371);
	/*
	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			vec3* p;
			vec3* n;

			p = gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y));
////
			vec3sub(*p, source->origin); 
			{	float r = randf()-0.5;
				vec3scale(*p,  1.0  -  r*   source->dsize) ;
			}
			vec3add(*p, source->origin);

		}

	}
	*/
	
	//if (source->generation < 10)
	//	dest->dsize*=1.3;	
	
	
//	dest->vb->vertex_count = w*h;
	gx_quadpatch_norm(dest);

	//printf(" made child quad\n");
	//getc(stdin);
}


#endif



int main(int argc, char** argv)
{
	int i,j,k;
	int run_tesselator=1;
	gx_image_t * tex;
	gx_camera_t	player_camera;
	gx_camera_init(&player_camera);

	gx_vbuffer_t* vb = NULL;
	//gx_init(800, 600 , "Test", GX_OPTION_NO_SHADER);
	gx_init(800, 600 , "Test", 0);

	gx_vbuffer_t* sph = NULL;

	tex = gx_image_load_tga("../shared/label.tga");

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
			for (k=3;k<6;k++) {
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


	//create gx_quadpatch

	#define PLANET_SIZE 3


	gx_quadpatch_sys_t qpsys;

	gx_quadpatch_sys_init(&qpsys);


	gx_quadpatch_t* qp;
	vec3 origin;
	//vec3set (origin, 0,-3,-3);
	vec3set (origin, 0, 0,0);
	float d;

	gx_quadpatch_detailer_f detailers[] = {
		do_detail,do_detail,do_detail,
		do_detail,do_detail,do_detail};
		
	
	gen_qpcube(&qpsys,
				&origin,
				17,
				2.0,
				NULL,
				NULL,
				detailers,
				/*GEN_SPHERE|*/GEN_INSIDE,
				30, 0.07
  			);

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
		//
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




			//gx_vbuffer_draw(vb, 0, 3*3*3*3 , gx_triangles, ZFALSE);
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

			//draw qp

			//gx_quadpatch_draw(qp);
			if (run_tesselator)
				gx_quadpatch_sys_eval(&qpsys, &player_camera);

			gx_quadpatch_sys_draw(&qpsys, &player_camera);




		ang+=.1;


		GX_TRACE
			gx_drawstyle_activate(teststyle);
		{
			vec3 p;
			vec3set(p, -4, -1, -2);
			gx_translate(&p);
		}
		GX_TRACE
			gx_rotate_y(ang * DEGREE);
		GX_TRACE
			gx_rotate_x(.1*ang * DEGREE);
		GX_TRACE

			//gx_set_active_textures(NULL, 0);
			//gx_vbuffer_draw(sph, 0, sph->vertex_count  , gx_quads, ZFALSE);

		GX_TRACE

			gx_frame_show();  //show the frame
		GX_TRACE

	}
	gx_disable();

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
