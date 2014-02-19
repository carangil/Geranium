// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include <math.h>
#include "../ztypes.h"
#include "../memory/ram.h"
#include "../vmath/vmath.h"
#include "../graphics/gx_sys.h"
#include "../graphics/gx_image.h"
#include "../graphics/gx_sprite.h"
#include "../graphics/gx_line.h"
#include "../graphics/gx_buffers.h"
#include "../graphics/gx_light.h"
#include "../structures/vector.h"
#include "../graphics/gx_drawstyle.h"
#include "../graphics/gx_misc.h"
#include "../graphics/gx_mesh.h"
#include "../graphics/gx_quadpatch.h"


#define STARCOUNT 2000



#define FADE_PATCHES
#define ALPHA_SPEED  .01


#define QUADARRAY_TAG_NONE        0
#define QUADARRAY_TAG_REMOVE	    1
#define QUADARRAY_TAG_DRAW_ONLY     2
#define QUADARRAY_TAG_DELETE	    3

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

	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			a = x/2 + a_start;
			b = y/2 + b_start;

			
		
			vec3mov(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a,b)));
			div = 1;
			fdiv = sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a,b))));

			if ((x & 1) && ( (a+1)< a_end) ) //if odd x
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b))));
			}

			if ((y & 1) && ( (b+1)< b_end) ) //if odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a,b+1))));
			}

			if ((y & 1) && ( (b+1)< b_end)  && (x & 1) && ( (a+1)< a_end)) //if odd x and odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b+1))));
			}


			vec3scale(p, 1.0/div);  //unit scale
		//	fdiv /= div;  //average length
		//	printf("div %d fdiv  %f\n", div, fdiv);
			//vec3scale(p, 1.0/fdiv); //round out

/*
			//add noise
			{
				vec3 u;
				vec3set(u, randf(),randf(),randf());

				vec3madd(p, ( .2+ ((rand()&0xff))/256.0) *- .1*  source->dsize, u );
			}*/


		



			*gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y)) = p;


			//texcoord: don't interpolate coordinates
			//gx_vbuffer_s(dest->vb, qp_vindex( dest, x, y),0) = ((float) x) / (w-1);
			//gx_vbuffer_t(dest->vb, qp_vindex( dest, x, y),0) = ((float) y) / (w-1);


		//	*gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y))
		//	=
		//	*gx_vbuffer_v( source->vb, qp_vindex( source, a,b));

//				printf("copy %d %d  to %d %d\n", a,b,x,y);
		}
	}

	gx_quadpatch_norm(dest);
	//add noise
	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			vec3* p;
			vec3* n;

		//	if (   ((x&1)|(y&1)) == 0)
		//		continue; //skip 



		//	float d;

		//	float r= randf();
		//	vec3 u;
		//	vec3set(u, randf(),randf(),randf());
			p = gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y));
		//	n = gx_vbuffer_n( dest->vb, qp_vindex( dest, x, y));

			vec3sub(*p, source->origin); 
			{	float r = randf();
				vec3scale(*p,  1.0  -  r*   source->dsize );
			}
			vec3add(*p, source->origin);


			//d = sqrt(vec3abs_sq(

//			vec3normalize( p); //round out

//			displace in direction of normal
	//		vec3madd(*p,     source->dsize, *n );

			//displace straight up and down
		//	vec3scale(*p, .9);
	//		vec3scale(*p,  1.0  - (randf()*source->dsize)  );
			
		}

	}




}







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

	gx_drawstyle_t ds_qa;
	
	vec_t*	 quadpatches;
	vec_t*	 quadpatches_fadeout;  //quadpatches fading out
	vec_t*	 quadpatches_fadein;  //quadpatches fading out
	vec_t*   toplevel_quadpatches;	


	gx_quadpatch_t*	 atmosqa[6];
	gx_quadpatch_t*	 oceanqa[6];

	
	gx_light_t* light = NULL;


	gx_image_t		*spacerock = NULL;  //holds rock texture for asteroids
	gx_drawstyle_t	spacerock_ds;   //drawstyle for the asteroid
	
	
	gx_image_t* hf = NULL;


	gx_image_t* atmosphere = NULL;
	gx_sprite_t* atmos_s = NULL;

	//meshtree_t* mtree = NULL;
	
//	patch_t*  patches[100];
	
	int numpatches=0;

	gx_vbuffer_t* vb =  NULL;

	gx_quadpatch_t* qp = NULL;

	vec3 skycolor;
	vec3 fogcolor;

	vec3set(skycolor, .7,.2,.1);
	vec3set(fogcolor, .6,.6,.6);


	memset (atmosqa, 0, sizeof(atmosqa));
	memset (oceanqa, 0, sizeof(oceanqa));

	//Initialize graphics
	gx_init(800, 600 , "Tri Mesh Quad Tree");
	//gx_clear_color(0,0,0,1);

	gx_clear_color(.7,.7,.7,1);
	//gx_mouse_capture(ztrue);  //mouse input will be relative 
	
	//Load assets
	spacerock  = gx_image_load_tga( "rock.tga");
//	spacerock_ds.textures = &spacerock;
//	spacerock_ds.numtextures=1;
	vec_mk(&spacerock_ds.textures,1);
	vec_add(&spacerock_ds.textures, spacerock);


//	atmosphere  = gx_image_load_tga( "atmosphere.tga");

//	atmos_s = gx_sprite_mk(atmosphere, 0, 0, 256,256,3 ,3);

	//initialize game data
	gx_camera_init(&player_camera);
	vec3set(camera_inertia, 0,0,0);

	player_camera.camera_pos.named.z +=2;
	player_camera.camera_pos.named.y +=1;
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

	//lets create some meshtree stuff



	//vb = gx_vbuffer_mk(10000000,10000000,zfalse, zfalse, 1);

	
	

//	gx_vbuffer_update(vb);
	
	//top level of the patch tree
	toplevel_quadpatches = vec_mk(NULL, 32);


	//these other 'working lists' are set to disown.  they won't free their contents
	//they are 'owned' by the top level list or their parent
	quadpatches = vec_mk(NULL, 32);
	vec_disown(quadpatches);  

		
	//low detail fading out
	quadpatches_fadeout = vec_mk(NULL, 32);
	vec_disown(quadpatches_fadeout);
	
	//low detail fading in
	quadpatches_fadein = vec_mk(NULL, 32);
	vec_disown(quadpatches_fadein);


	

	//create a stupud perect sphere for atmosphere
#if 0
	for (i=0;i<6;i++)
	{

		qp = gx_quadpatch_mk(33,33);

				
#define CUBE_TOP 0
#define CUBE_BOTTOM 1
#define CUBE_FRONT 2
#define CUBE_BACK 3
#define CUBE_LEFT 4
#define CUBE_RIGHT 5

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

										
					switch (i)
					{
					case CUBE_TOP: 
						vec3set(*vv, fa, 1, -fb);
						break;

					case CUBE_BOTTOM:
						vec3set(*vv, fa, -1, fb);
						break;


					case CUBE_FRONT:
						vec3set(*vv, fa, fb, 1);
						break;

					case CUBE_BACK:
						vec3set(*vv, -fa, fb, -1);
						break;

						
					case CUBE_LEFT:
						vec3set(*vv, -1, fb, fa);
						break;

					case CUBE_RIGHT:
						vec3set(*vv, 1, fb, -fa);
						break;

					}


					d = vec3abs_sq(*vv);
					d=sqrt(d);
					vec3scale(*vv, 1/d);

				

					vec3scale(*vv, 1.1);

					

					//vec3add(*vv, origin);

			
					//do normal
					vv = gx_vbuffer_n( qp->vb, qp_vindex( qp, a,b));
					vec3set(*vv, 0,1,0);

				}
			}

			qp->center = *gx_vbuffer_v( qp->vb, qp_vindex( qp, qp->w/2,qp->h/2));
			
					
			gx_quadpatch_norm(qp);
		
			atmosqa[i] = qp;
			gx_vbuffer_update(qp->vb);
			
		}
	}
#endif

//create a stupud perect sphere for ocean
#if 0
	for (i=0;i<6;i++)
	{

		qp = gx_quadpatch_mk(33,33);

				
#define CUBE_TOP 0
#define CUBE_BOTTOM 1
#define CUBE_FRONT 2
#define CUBE_BACK 3
#define CUBE_LEFT 4
#define CUBE_RIGHT 5

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

										
					switch (i)
					{
					case CUBE_TOP: 
						vec3set(*vv, fa, 1, -fb);
						break;

					case CUBE_BOTTOM:
						vec3set(*vv, fa, -1, fb);
						break;


					case CUBE_FRONT:
						vec3set(*vv, fa, fb, 1);
						break;

					case CUBE_BACK:
						vec3set(*vv, -fa, fb, -1);
						break;

						
					case CUBE_LEFT:
						vec3set(*vv, -1, fb, fa);
						break;

					case CUBE_RIGHT:
						vec3set(*vv, 1, fb, -fa);
						break;

					}


					d = vec3abs_sq(*vv);
					d=sqrt(d);
					vec3scale(*vv, 1/d);

				

					vec3scale(*vv, .97);

					

					//vec3add(*vv, origin);

			
					//do normal
					vv = gx_vbuffer_n( qp->vb, qp_vindex( qp, a,b));
					vec3set(*vv, 0,1,0);

				}
			}

			qp->center = *gx_vbuffer_v( qp->vb, qp_vindex( qp, qp->w/2,qp->h/2));
			
					
			gx_quadpatch_norm(qp);
		
			oceanqa[i] = qp;
			gx_vbuffer_update(qp->vb);
			
		}
	}
#endif


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
//		qp = gx_quadpatch_mk(65,65, do_detail);  //make mesh of 64 by 64 quads( 65by65 points)
		qp = gx_quadpatch_mk(33,33, do_detail);
//		qp = gx_quadpatch_mk(17,9, do_detail);
//		qp = gx_quadpatch_mk(17,9, NULL);
		//qp = gx_quadpatch_mk(9,9);
		//qp = gx_quadpatch_mk(3,3);
		
		
		//qp->size = .3; //start at root size
		//qp->dsize = .1;

		qp->onlevel = 1;

		qp->size=  8.0 ;
		//qp->dsize = .015  ;
	//	qp->dsize = .006  ;
	qp->dsize=.0085;

		qp->origin = origin;

//		qp->size =  qp->size / ((qp->w-1) * (qp->h-1));
//		qp->dsize =  qp->dsize / (qp->w );
		
#define CUBE_TOP 0
#define CUBE_BOTTOM 1
#define CUBE_FRONT 2
#define CUBE_BACK 3
#define CUBE_LEFT 4
#define CUBE_RIGHT 5
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
					case CUBE_TOP: 
						vec3set(*vv, fa, 1, -fb);
						break;

					case CUBE_BOTTOM:
						vec3set(*vv, fa, -1, fb);
						break;


					case CUBE_FRONT:
						vec3set(*vv, fa, fb, 1);
						break;

					case CUBE_BACK:
						vec3set(*vv, -fa, fb, -1);
						break;

						
					case CUBE_LEFT:
						vec3set(*vv, -1, fb, fa);
						break;

					case CUBE_RIGHT:
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
					vec3scale(*vv, 1/d);

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
			
			
		
			vec_add(quadpatches, qp);
			vec_add(toplevel_quadpatches, qp);
			rootqa[i] = qp;

			
			
		}
	}
	
	

	//loop	
	gx_quadpatch_sew(rootqa[CUBE_FRONT], GX_EDGE_LEFT, rootqa[CUBE_LEFT], GX_EDGE_RIGHT,0);
	gx_quadpatch_sew(rootqa[CUBE_FRONT], GX_EDGE_RIGHT, rootqa[CUBE_RIGHT], GX_EDGE_LEFT,0);
	gx_quadpatch_sew(rootqa[CUBE_BACK], GX_EDGE_LEFT, rootqa[CUBE_RIGHT], GX_EDGE_RIGHT,0);
	gx_quadpatch_sew(rootqa[CUBE_BACK], GX_EDGE_RIGHT, rootqa[CUBE_LEFT], GX_EDGE_LEFT,0);


	//top
	gx_quadpatch_sew(rootqa[CUBE_TOP], GX_EDGE_BOTTOM, rootqa[CUBE_FRONT], GX_EDGE_TOP,0);
	gx_quadpatch_sew(rootqa[CUBE_TOP], GX_EDGE_LEFT, rootqa[CUBE_LEFT], GX_EDGE_TOP,1);
	gx_quadpatch_sew(rootqa[CUBE_TOP], GX_EDGE_RIGHT, rootqa[CUBE_RIGHT], GX_EDGE_TOP,0);
	gx_quadpatch_sew(rootqa[CUBE_TOP], GX_EDGE_TOP, rootqa[CUBE_BACK], GX_EDGE_TOP,1);


	//bottom
	gx_quadpatch_sew(rootqa[CUBE_BOTTOM], GX_EDGE_TOP, rootqa[CUBE_FRONT], GX_EDGE_BOTTOM,0);
	gx_quadpatch_sew(rootqa[CUBE_BOTTOM], GX_EDGE_LEFT, rootqa[CUBE_LEFT], GX_EDGE_BOTTOM,0);
	gx_quadpatch_sew(rootqa[CUBE_BOTTOM], GX_EDGE_RIGHT, rootqa[CUBE_RIGHT], GX_EDGE_BOTTOM,1);
	gx_quadpatch_sew(rootqa[CUBE_BOTTOM], GX_EDGE_BOTTOM, rootqa[CUBE_BACK], GX_EDGE_BOTTOM,1);




	//update
	for (i=0;i<6;i++)
	{

#ifdef QUADARRAY_SKIRTS
		gx_quadpatch_skirt(rootqa[i]);
#endif
		gx_vbuffer_update(rootqa[i]->vb);

	}



	}



	//create an image
#if 0
	{	
		int start;
		int end;
		gx_vbuffer_t* vv  = NULL;
		int a;
		int b;
		hf = gx_image_mk(128,128,GX_IMAGE_GRAY);
 
		for (a=0;a<128;a++)
		{
			for (b=0;b<128;b++)
			{
				hf->data[b*128 +a] = 127 + 10 * (sin( (float)a/20.0) + cos((float)b/20.0));
			}
		}

		//make heightfield from it
		vv = gx_vbuffer_from_image(NULL, hf, 0, -1, -5, 0, 1, 2, 5, 5, 5, zfalse, 0, &start, &end, zfalse);

		gx_vbuffer_update(vv);
		hmesh = gx_mesh_def(vv, NULL, start, end, ztrue);


	}
#endif

	//The game loop 
	for(;;)  
	{
		

 		gx_window_event();  //handles any window events (I/O)

		//set up projection matrix for this frame
 		gx_setup_3d( 80.0f,  gx_frame_get_dimensions(NULL,NULL),0.001f, 10.0f);
		
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

		gx_drawstyle_activate(&ds_qa);
		
				
		//printf(" %d opaque patches \n", vec_count(quadpatches));
		//draw opaque patches
		{
			int cull=0;
			int i;

			for (i=0;i< vec_count(quadpatches);i++)
			{
				int skipdraw=0;
				qp = vec_get_at(quadpatches, i);

				{
					vec3 d;
					vec3mov (d, qp->center);
					vec3sub (d, player_camera.camera_pos);
					vec3normalize(&d);

					if (vec3dot(d, player_camera.camera_forward) < 0)
					{
						//outside of view cone
						cull++;
						skipdraw=1;
					}
					else
					if ( vec3dot(qp->avgnorm, player_camera.camera_forward) > .8)  //faces away from camera
					{
						cull++;
						skipdraw =1;
					}



				}
				
				//draw
				if (qp->tag == QUADARRAY_TAG_NONE || qp->tag == QUADARRAY_TAG_DRAW_ONLY)
				{
					if (!skipdraw)
							gx_quadpatch_draw(qp);
					
				}
				else if ((qp->tag == QUADARRAY_TAG_REMOVE) || (qp->tag == QUADARRAY_TAG_DELETE)) //remove from opaque draw list
				{

					vec_remove_unordered(quadpatches, i);
					i--; //repeat this position 

					if (qp->tag == QUADARRAY_TAG_DELETE)
					{
						//printf("DELETE\n");
						if (qp->deleted) {
							printf(" DOUBLE DELETE!\n");
							exit(0);

						}
						qp->deleted=1;
						ram_free(qp);
						qp = NULL;
					}
					else 
					{
						qp->tag = QUADARRAY_TAG_NONE;

					}

					



				}
			}
		}

		ds_qa.use_constant_alpha = 1;

#ifdef FADE_PATCHES
		//draw fadeout patches (low detail fading out into high detail below it)
		{ 
			int i;

			for (i=0;i<vec_count(quadpatches_fadeout);i++)
			{
				qp = vec_get_at(quadpatches_fadeout, i);
				if (qp->alpha > 0)
				{
					ds_qa.blending = gx_blend_alpha;		
					ds_qa.alpha = qp->alpha;
					gx_drawstyle_activate(&ds_qa);

					gx_quadpatch_draw(qp);
					qp->alpha -= ALPHA_SPEED;
				}
				else
				{
					vec_remove_unordered(quadpatches_fadeout, i);
					i--; //repeat this position
					
					//untag the children
					qp->children[0]->tag=0;
					qp->children[1]->tag=0;
					qp->children[2]->tag=0;
					qp->children[3]->tag=0;
								
				}

			}
			//printf("  %d patches fadeout\n", vec_count(quadpatches_fadeout));
		}


		//draw fadein patches
#if 1
		{ 
			int i;
		//	printf(" %d patches ", vec_count(quadpatches));

			for (i=0;i<vec_count(quadpatches_fadein);i++)
			{
//				exit(1);
				qp = vec_get_at(quadpatches_fadein, i);
				
				{
					ds_qa.blending = gx_blend_alpha;		
					ds_qa.alpha = qp->alpha;
					gx_drawstyle_activate(&ds_qa);

					gx_quadpatch_draw(qp);
		 			qp->alpha +=ALPHA_SPEED;
			//		printf("%f\n", qp->alpha);
				}
				if (qp->alpha > 1)
				{
					vec_remove_unordered(quadpatches_fadein, i);
					i--; //repeat this position 
					//add to running patch
					vec_add(quadpatches, qp);
					qp->tag=0;
					//tag children for removal

/*
					qp->children[0]->tag=QUADARRAY_TAG_REMOVE;
					qp->children[1]->tag=QUADARRAY_TAG_REMOVE;
					qp->children[2]->tag=QUADARRAY_TAG_REMOVE;
					qp->children[3]->tag=QUADARRAY_TAG_REMOVE;
*/

					qp->children[0]->tag=QUADARRAY_TAG_DELETE;
					qp->children[1]->tag=QUADARRAY_TAG_DELETE;
					qp->children[2]->tag=QUADARRAY_TAG_DELETE;
					qp->children[3]->tag=QUADARRAY_TAG_DELETE;






					qp->children[0]=NULL;
					qp->children[1]=NULL;
					qp->children[2]=NULL;
					qp->children[3]=NULL;

				}

			}
			//printf("  %d patches fadein\n", vec_count(quadpatches_fadein));
		}
#endif
#endif

		//gx_set_active_lights(&light, 1);


if (0) {

			//draw ocean
			float d;
			float fc[] = {0,0,0,0};
			gx_light_t ll;

			gx_light_t* lll;

			gx_drawstyle_t ds;
			ram_clear(&ds, sizeof(ds));
			ram_clear(&ll, sizeof(ll));
			//vec3mov(ds.specular_color, skycolor);
			//ds.specular_exponent = 0;

			ll.light_type = gx_light_point;
			vec3mov(ll.color, skycolor);
			//vec3scale(ll.color, .6);
			
			vec3mov(ll.position, player_camera.camera_pos);
			//vec3scale(ll.position, -1);
			
			//vec3scale (ll.ambient, .25);
			
			//lll = &ll;
			//gx_set_active_lights(&lll, 1);


			
			ds.use_constant_alpha = ztrue;
			ds.blending = 0; 
			gx_drawstyle_activate(&ds);

			


			d = vec3abs_sq(player_camera.camera_pos);
			d = sqrtf(d);
			//gx_light_fog( fc, 0, d+2);
			gx_light_fog( NULL,0,0);

			
			for (i=0;i<6;i++)
			{
				gx_quadpatch_draw(oceanqa[i]);

			}
		}




		gx_set_active_lights(NULL, 0);

		{

			//draw atmos
			float d;
			float fc[] = {0,0,0,0};
			gx_light_t ll;

			gx_light_t* lll;

			gx_drawstyle_t ds;
			ram_clear(&ds, sizeof(ds));
			ram_clear(&ll, sizeof(ll));
			//vec3mov(ds.specular_color, skycolor);
			//ds.specular_exponent = 0;

			ll.light_type = gx_light_point;
			vec3mov(ll.color, skycolor);
			//vec3scale(ll.color, .6);
			
			vec3mov(ll.position, player_camera.camera_pos);
			//vec3scale(ll.position, -1);
			
			//vec3scale (ll.ambient, .25);
			
			lll = &ll;
			gx_set_active_lights(&lll, 1);


			
			ds.use_constant_alpha = ztrue;
			ds.blending = gx_blend_add; 
			gx_drawstyle_activate(&ds);

			


			d = vec3abs_sq(player_camera.camera_pos);
			d = sqrtf(d);
			//gx_light_fog( fc, 0, d+2);
			gx_light_fog( NULL,0,0);

			
			for (i=0;i<6;i++)
			{
				gx_quadpatch_draw(atmosqa[i]);

			}
		}


		gx_light_fog( NULL,0,0);


		


		printf(" %d skirted %d nonskirted   %d drawn  %d total\n", skirted, nonskirted, drawn, total);
		skirted=nonskirted=drawn=0;
		gx_frame_show();  //show the frame

#ifdef FADE_PATCHES
		//remove anything that was tagged for it
	 	{
			int i;
			for (i=0;i<vec_count(quadpatches);i++)
			{
				gx_quadpatch_t* qp = vec_get_at(quadpatches, i);
				if (qp->tag==1)
				{
					vec_remove_unordered(quadpatches, i);
					i--;
				}

			}

		}
#endif

		//evaluate quadpatches
		{
			int i;
			int ocount = 0;
			int splitcount=0;
			int splitlimit=1000;

			
			
			float closest_d=0;//quick:find the closest one


			closest = NULL;

			for (i=0;i<vec_count(quadpatches);i++)
			{
				vec3 p;
				float d;
				gx_quadpatch_t* qp = vec_get_at(quadpatches, i);

				vec3mov(p, qp->center);
//				vec3normalize(&p);
				vec3sub(p, player_camera.camera_pos);
				
				if (!closest ||  (d=vec3abs_sq(p)) < closest_d)
				{
					closest_d = d;
					closest = qp;

				}

			}
			
			if ( qp = closest)
			{
			
				vec3 p;
				float dplayer;
				float d;
				int i;
				int j;

				vec3 p_closest;
				float closest_point_d=10000000;


				//closest->boo=1;  //flag to draw differently

				//ok now check all these points against the camera to make sure we don't go into the planet
				//(slow)
				dplayer = vec3abs_sq(player_camera.camera_pos);  // planet center is at 0,0, so ok
				dplayer = sqrt(dplayer);

				vec3set(p_closest, 0,0,0);

				//find point on gx_quadpatch closest to player
				for (i=0;i<qp->w;i++)
				{
					for (j=0;j<qp->h;j++)
					{

						vec3mov(p, *gx_vbuffer_v( qp->vb, qp_vindex( qp, i,j)));

						vec3sub(p, player_camera.camera_pos);

						d = vec3abs_sq(p);

						if (d< closest_point_d)
						{
							closest_point_d = d;
							vec3mov(p_closest, *gx_vbuffer_v( qp->vb, qp_vindex( qp, i,j)));
						}

					}
				}

				//now if the closest point to us on the surface is farther away from the planet center than us, then we are inside the planet, and thats bad
				d = vec3abs_sq(p_closest);
				d = sqrt(d) + .004f; 

				

				if (dplayer < d)
				{
					vec3normalize(& (player_camera.camera_pos));
					vec3scale(player_camera.camera_pos, d)
					//vec3scale(camera_inertia, .5);
					//vec3set(camera_inertia,0,0,0);
				}
						



			}
				


			ocount = vec_count(quadpatches);
			

			for(i=0;(i<vec_count(quadpatches)) && (i<ocount) ;i++)
			{
				vec3 p;
				float d;
				gx_quadpatch_t* qp = vec_get_at(quadpatches, i);
				

				//find its distance from camera
				vec3mov(p, player_camera.camera_pos);
				vec3sub(p, qp->center);
				d = vec3abs_sq(p);
				d = sqrt(d);
				
//				printf(" distance: %f  size: %f\n", d, qp->size / d );

		
				if (qp->tag != QUADARRAY_TAG_NONE)
					continue;  //don't process unless it's a fully active gx_quadpatch


				if (   ((((qp->size)/(d*d)) > 1.0) &&(splitcount < splitlimit)))
				{
					gx_quadpatch_t* dest[4];


				
			
			

					//remove the current qp
					splitcount++;
					
					
					if (gx_quadpatch_split(qp))
					{
						qp->tag = QUADARRAY_TAG_REMOVE;  //remove parent
						qp->onlevel = 0; //will no longer be active
				

						vec_add(quadpatches, qp->children[0]);
						vec_add(quadpatches, qp->children[1]);
						vec_add(quadpatches, qp->children[2]);
						vec_add(quadpatches, qp->children[3]);

#ifndef FADE_PATCHES
						

						//add in the children
						qp->children[0]->tag = QUADARRAY_TAG_NONE;
						qp->children[1]->tag = QUADARRAY_TAG_NONE;
						qp->children[2]->tag = QUADARRAY_TAG_NONE;
						qp->children[3]->tag = QUADARRAY_TAG_NONE;

						qp->children[0]->onlevel = 1;
						qp->children[1]->onlevel = 1;
						qp->children[2]->onlevel = 1;
						qp->children[3]->onlevel = 1;


#endif

					/*	dirty_edges(qp);
						{int i;
							for (i=0;i<4;i++)
							{
								dirty_edges(qp->children[i]);
							}
						}
						*/



#ifdef FADE_PATCHES
						//tag children to draw, but not to process (until the fadeout is done)
						qp->children[0]->tag = 2;
						qp->children[1]->tag = 2;
						qp->children[2]->tag = 2;
						qp->children[3]->tag = 2;

						qp->children[0]->onlevel = 1;
						qp->children[1]->onlevel = 1;
						qp->children[2]->onlevel = 1;
						qp->children[3]->onlevel = 1;


						//add to fadeout list
						vec_add(quadpatches_fadeout, qp);
				
						qp->alpha = 1.0; 
#endif

						
						

					}
				}
				//combine patches
				else if ( qp->parent && ( qp->parent->tag == QUADARRAY_TAG_NONE)
					&& qp->parent->children[0] && qp->parent->children[0]->onlevel
					&& qp->parent->children[1] && qp->parent->children[1]->onlevel
					&& qp->parent->children[2] && qp->parent->children[2]->onlevel
					&& qp->parent->children[3] && qp->parent->children[3]->onlevel
					)
				{
					vec3mov(p, player_camera.camera_pos);
					vec3sub(p, qp->parent->center);
					d = vec3abs_sq(p);
					d = sqrt(d);
					
					if (((qp->parent->size)/(d*d)) < 1.0)
					{  //combine threshhold
					
					//	vec_add(quadpatches, qp->parent); //put parent back in
	

						//remove these
						//qp->parent->children[0]->tag = 1; //2 is draw but don't remove, dont process
						//qp->parent->children[1]->tag = 1;
						//qp->parent->children[2]->tag = 1;
						//qp->parent->children[3]->tag = 1;

						//will no longer be on level
						qp->parent->children[0]->onlevel = 0;
						qp->parent->children[1]->onlevel = 0;
						qp->parent->children[2]->onlevel = 0;
						qp->parent->children[3]->onlevel = 0;

#ifndef FADE_PATCHES
/*
						qp->parent->children[0]->tag = QUADARRAY_TAG_REMOVE; 
						qp->parent->children[1]->tag = QUADARRAY_TAG_REMOVE;
						qp->parent->children[2]->tag = QUADARRAY_TAG_REMOVE;
						qp->parent->children[3]->tag = QUADARRAY_TAG_REMOVE;
*/


						qp->parent->children[0]->tag = QUADARRAY_TAG_DELETE; 
						qp->parent->children[1]->tag = QUADARRAY_TAG_DELETE;
						qp->parent->children[2]->tag = QUADARRAY_TAG_DELETE;
						qp->parent->children[3]->tag = QUADARRAY_TAG_DELETE;
//						qp->parent->children[0] = NULL;
//						qp->parent->children[1] = NULL;
//						qp->parent->children[2] = NULL;
//						qp->parent->children[3] = NULL;



						vec_add(quadpatches, qp->parent); //put parent back in
						qp->parent->onlevel = 1;
						qp->parent->tag = QUADARRAY_TAG_NONE;

					/*	dirty_edges(qp);
						{int i;
							for (i=0;i<4;i++)
							{
								dirty_edges(qp->children[i]);
							}
						}
						*/

					
#endif

#ifdef FADE_PATCHES
						qp->parent->alpha = 0;
						qp->parent->tag = QUADARRAY_TAG_DRAW_ONLY;
						qp->parent->onlevel=1;

						vec_add(quadpatches_fadein, qp->parent); //put parent back in

						//tag all siblings
						qp->parent->children[0]->tag = QUADARRAY_TAG_DRAW_ONLY; //2 is draw but don't remove, dont process
						qp->parent->children[1]->tag = QUADARRAY_TAG_DRAW_ONLY;
						qp->parent->children[2]->tag = QUADARRAY_TAG_DRAW_ONLY;
						qp->parent->children[3]->tag = QUADARRAY_TAG_DRAW_ONLY;

						//also mark as dirty
						/*
						dirty_edges(qp->parent);
						dirty_edges(qp->parent->children[0]);
						dirty_edges(qp->parent->children[1]);
						dirty_edges(qp->parent->children[2]);
						dirty_edges(qp->parent->children[3]);
						*/

#endif

					}
				}
		
			}
			//printf(" SPLIT %d\n", splitcount);
		}

	}

	ram_free(toplevel_quadpatches);
	ram_free(quadpatches);
	ram_free(quadpatches_fadeout);
	ram_free(quadpatches_fadein);
	ram_free(starfield);
	gx_disable();
	ram_free(light);
	vec_cleanup(&spacerock_ds.textures);

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
