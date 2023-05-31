#define _CRT_SECURE_NO_WARNINGS
#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"

#include "math.h"
#include "zbitmap.h"
#include "zxml.h"
#include "gfx_gl.h"



//#define USE_VERTEX_GROUPS



/* test program */
//extern int frame;


void gfx_gl_test() {
	zvecT* garbage = zvec_mk(NULL, 64);

	int frame = -1 ;

	zwindowT* zwin = gfx_mkwindow("Testing", 1024, 768, 0);
	zvec_add(garbage, zwin);

	zbitmapT* earthpic = zbitmap_load_tga("../../Zcore-data/earth-cylindrical-alpha-holes.tga", 1 * ZTGA_TOP);
	zbitmapT* strawpic = zbitmap_load_tga("../../Zcore-data/web/strawberry/Texture/Strawberry_basecolor.tga", 0 * ZTGA_TOP);
	zvec_add(garbage, earthpic);
	zvec_add(garbage, strawpic);


	//zbitmapT* grid = zbitmap_load_tga("../../Zcore-data/finechecker.tga", 1 * ZTGA_TOP);
	zbitmapT* grid = zbitmap_load_tga("../../Zcore-data/web/femalenude.tga", 0 * ZTGA_TOP);
	zvec_add(garbage, grid);

	//zbitmapT* brickpic = zbitmap_load_tga("../../Zcore-data/web/brick.tga", 0);
	//zbitmapT* bricknorm = zbitmap_load_tga("../../Zcore-data/web/brick_normal.tga", 0);

	zbitmapT* brickpic = zbitmap_load_tga("../../Zcore-data/web/Rock_044_SD/Rock_044_BaseColor.tga", 0);
	zbitmapT* bricknorm = zbitmap_load_tga("../../Zcore-data/web/Rock_044_SD/Rock_044_Normal.tga", 0);
	zbitmapT* brickheight = zbitmap_load_tga("../../Zcore-data/web/Rock_044_SD/Rock_044_Height.tga", 0);

	//zbitmapT* brickpic = zbitmap_load_tga("../../Zcore-data/finechecker.tga", 0);
	//zbitmapT* brickheight = zbitmap_load_tga("../../Zcore-data/testheight.tga", 0);

	zvec_add(garbage, brickpic);
	zvec_add(garbage, bricknorm);
	zvec_add(garbage, brickheight);

	zvecT* ignore = zstrsplit(NULL, "__0|root-end|hips|arm.pool.r|arm.pool.l|arm.ik.r|arm.ik.l|leg.ik.l|leg.ik.r|leg.pool.l|leg.pool.r|root|hips.comtroll|arm.pool.r-end|arm.pool.l-end|arm.ik.r-end|arm.ik.l-end|leg.ik.l-end|leg.ik.r-end|leg.pool.l-end|leg.pool.r-end|eye.l|eye.r|eye.l-end|eye.r-end", '|');

	//gfx_jointT* skel = load_bvh("../../Zcore-data/web/metal_hands.bvh", .1, ignore);
	//gfx_jointT* skel = load_bvh("../../Zcore-data/testhuman.bvh", .1, NULL);
	 gfx_jointT* skel = load_bvh("../../Zcore-data/test/blender-processed/test.bvh", .1, NULL);
   	//gfx_jointT* skel = load_bvh("../../Zcore-data/web/dance.bvh", 1, ignore);//not working
	//gfx_jointT* skel = load_bvh("../../Zcore-data/test.bvh", 2);

;
	zvec_add(garbage, skel);

	gfx_meshT* cube_mesh = gfx_mesh_load_obj("../../Zcore-data/cube.obj", 1.0);
	gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/strawberry/Strawberry_obj.obj", 1.0);
	zvec_add(garbage, strawberry_mesh);
	zvec_add(garbage, cube_mesh);

	//gfx_meshT* model = gfx_mesh_load_obj("../../Zcore-data/web/metal_hands.obj", .1);
	//gfx_meshT* model = gfx_mesh_load_obj("../../Zcore-data/web/metal_hands_vg.obj", .1);
	vec3 min;
	vec3 max;
	//gfx_meshT* model = gfx_mesh_load_objmm("../../Zcore-data/testhuman.obj", .1, &min, &max);
	gfx_meshT* model = gfx_mesh_load_objmm("../../Zcore-data/test/blender-processed/test.obj", .1, &min, &max);
	printf(" min %f %f %f   max %f %f %f\n", min.VX, min.VY, min.VZ, max.VX, max.VY, max.VZ);
	//gfx_meshT* model = gfx_mesh_load_obj("../../Zcore-data/web/dance.obj", 1);

	zvec_add(garbage, model);


	//map points to bones
	gfx_identity();
	recurse_skeleton(skel, -1, 0); //process the skeleton rest pose

	
#if 0
	{
		gfx_meshT* m;
		int i;

		for (m = model; m; m = m->next) {

			int count = m->vb->count;


			m->vbaux = gfx_vertex_buffer_mk(count, "position:3|normal:3|color:4");
			gfx_vertex_buffer_add_index(m->vbaux, zarray_count(m->vb->index_buffer));

			
			m->bone = zarray_alloc(zuint16, count);
			m->bone1 = zarray_alloc(zuint16, count);
			m->bone_blend = zarray_alloc(float, count);
	
			//copy original points to aux buffer
			memcpy(m->vbaux->attributes[m->vbaux->fixed_position].data, m->vb->attributes[m->vb->fixed_position].data, sizeof(float) * 3 * m->vb->count);
			memcpy(m->vbaux->attributes[m->vbaux->fixed_normal].data, m->vb->attributes[m->vb->fixed_normal].data, sizeof(float) * 3 * m->vb->count);
			m->vbaux->count = m->vb->count;

			memcpy(m->vbaux->attributes[m->vbaux->fixed_normal].data, m->vb->attributes[m->vb->fixed_normal].data, sizeof(float) * 3 * m->vb->count);
			
			memcpy(m->vbaux -> index_buffer, m->vb->index_buffer, sizeof(zuint16) * zarray_count(m->vb->index_buffer));
			zarray_use(m->vbaux->index_buffer, zarray_count(m->vb->index_buffer));
			
			
			m->vdebug = gfx_vertex_buffer_mk(count * 2, "position:3|color:4");

			vec4* color;
			

			//match up vertex group names to bone names



#ifdef USE_VERTEX_GROUPS
			for (i = 0; i < count; i++) {
				if (m->group_name[i] != -1) {
					int j;

					color = m->vbaux->attributes[m->vbaux->fixed_color].data + 4 * i;

					for (j = 0; j < zvec_count(&skel->bones); j++) {
						gfx_jointT* joint = zvec_get_at(&skel->bones, j);
						char* name = zvec_get_at(m->group_names, m->group_name[i]);
						if (!stricmp(joint->name, name)) {
							m->bone[i] = j;
							*color = joint->debugcolor;

							gfx_jointT* ch;
							gfx_jointT* closestchild = NULL;
							int k;
							//see if it makes sense to blend
							float closestt = 0;
							float closestd = 999999;
							int closestch = -1;

							vec3* p = &m->vb->attributes[m->vb->fixed_position].data[i * 3];

							for (k = 0; k < zvec_count(&joint->children); k++) {
								ch = zvec_get_at(&joint->children, k);




								float t;
								for (t = 0; t <= 1; t += .05) {  //step along the bone line

									vec3 q = joint->point;
									vec3scale(q, 1 - t);
									vec3madd(q, t, ch->point);

									vec3sub(q, *p);

									float d = vec3abs_sq(q);

									if (d < closestd) {


										closestd = d;
										closestt = t;
										closestch = k;
										closestchild = ch;
									}

								} //end t




							}//end k




							//if (closestt < .2) {
#if 0
							if (closestchild) {
								//vecset(*color, 1, 1, 1, 1, );
#define CC .15
								if ((closestt < CC)&&(!joint->parent->ignored)) {


									vec3scale(*color, closestt/CC);
									vec3madd(*color, 1 - closestt/CC, joint->parent->debugcolor);
									m->bone_blend[i] = 1-closestt/CC;
								}


							}
#endif					

							//}


						}

					}

				}

			}



#endif




#if 1   //choose closest bone
			for (i = 0; i < count; i++) {

				vec3 p;
				vec3 bestp;
				p.named.x = m->vb->attributes[m->vb->fixed_position].data[i * 3];
				p.named.y = m->vb->attributes[m->vb->fixed_position].data[i * 3 + 1];
				p.named.z = m->vb->attributes[m->vb->fixed_position].data[i * 3 + 2];

				vec3* n = m->vb->attributes[m->vb->fixed_normal].data + i * 3;
				vec4* color = m->vbaux->attributes[m->vbaux->fixed_color].data + 4 * i;
				
				//vec4 color = vec4const(1, 0, 0, 1);
				float closestd = 9999;

				int j;
				zbool found = ZFALSE;

				for (j = 0; j < zvec_count(&skel->bones); j++) {
					gfx_jointT* joint = zvec_get_at(&skel->bones, j);
					if (joint->ignored)
						continue;

					float t;
					for (t = 0; t <= .99; t += .1) {  //step along the bone line

						if (!joint->parent)
							continue;

						vec3 q = joint->point;
						vec3scale(q, t);
						vec3madd(q, 1 - t, joint->parent->point);
				
						vec3 qc = q;
						//find distance
						vec3sub(q, p);

						vec3 a = q;
						vec3 b = *n;
						vec3normalize(&a);
						vec3normalize(&b);

						float d = vec3abs_sq(q);

						if (d < closestd) {

							bestp = qc;
							closestd = d;
							m->bone[i] = j;//set ith point to use jth bone
							*color = joint->parent->debugcolor;
						}

					}//end t

				}//end jth bone
			
		

			} //end ith point
		}//end m



	} //end block
#endif
#endif




	float speed = .05;

	gfx_textureT* strawtex = gfx_texture_mk(strawpic);
	gfx_textureT* earthtex = gfx_texture_mk(earthpic);
	gfx_textureT* bricktex = gfx_texture_mk(brickpic);
	gfx_textureT* bricknormtex = gfx_texture_mk(bricknorm);
	gfx_textureT* brickheighttex = gfx_texture_mk(brickheight);
	gfx_textureT* gridtex = gfx_texture_mk(grid);
	zvec_add(garbage, strawtex);
	zvec_add(garbage, earthtex);
	zvec_add(garbage, bricktex);
	zvec_add(garbage, bricknormtex);
	zvec_add(garbage, brickheighttex);
	zvec_add(garbage, gridtex);


	gfx_styleT* st_sphere = gfx_style_mk();
	gfx_styleT* st_strawberry = gfx_style_mk();
	gfx_styleT* st_cube = gfx_style_mk();
	gfx_styleT* st_tex = gfx_style_mk();
	gfx_styleT* st_grid = gfx_style_mk();
	zvec_add(garbage, st_sphere);
	zvec_add(garbage, st_strawberry);
	zvec_add(garbage, st_cube);
	zvec_add(garbage, st_grid);


	gfx_style_set_property(st_sphere, 0, "blend", 0, GFX_BLEND_ALPHA, NULL, 0);


	vec3 ldcam;
	vec3 lpcam;
	vec4 lcol = vec4const(.8, .8, .7, 1.0);
	vec4 lam = vec4const(.1, .1, .1, 1.0);

	vec4 scol = vec4const(.2,.2,.2,0);

	float sh = 100.0;
	//set light parameters on both
	gfx_style_set_property(st_strawberry, GFX_FLOAT4, "light_color", 0, 0, &lcol, 0);
	gfx_style_set_property(st_strawberry, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);
	gfx_style_set_property(st_strawberry, GFX_TEXTURE, "texture_diffuse", 0, 0, strawtex, 0);

	gfx_style_set_property(st_grid, GFX_TEXTURE, "texture_diffuse", 0, 0, gridtex, 0);
	gfx_style_set_property(st_grid, GFX_FLOAT4, "light_color", 0, 0, &lcol, 0);
	gfx_style_set_property(st_grid, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);
	gfx_style_set_property(st_grid, GFX_FLOAT4, "specular", 0, 0, &scol , 0);


	gfx_style_set_property(st_sphere, GFX_FLOAT4, "light_color", 0, 0, &lcol, 0);
	gfx_style_set_property(st_sphere, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);
	gfx_style_set_property(st_sphere, GFX_TEXTURE, "texture_diffuse", 0, 0, earthtex, 0);
	
	gfx_style_set_property(st_cube, GFX_TEXTURE, "texture_diffuse", 0, 0, bricktex, 0);
	gfx_style_set_property(st_cube, GFX_TEXTURE, "texture_normal_tangent", 0, 0, bricknormtex, 0);
	gfx_style_set_property(st_cube, GFX_TEXTURE, "texture_height", 0, 0, brickheighttex, 0);
	
	gfx_style_set_property(st_cube, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);

	gx_set_basic_shader("@../gfxtest/vertex.glsl", "@../gfxtest/fragment.glsl");
	
	gx_shadergroupT* fsg = gx_shader_source("@../gfxtest/finevertex.glsl", "@../gfxtest/finefragment.glsl");
	zvec_add(garbage, fsg);

	st_sphere->shader_group = NULL;
	st_strawberry->shader_group = NULL;
	st_cube->shader_group = ram_addref(fsg);

	gfx_cameraT cam;
	gfx_camera_init(&cam);


	//make temp vertex buffer
	//gfx_vertex_bufferT* vbt = NULL; = gfx_vertex_buffer_mk(10000, "position:3|color:4");



	//create vertex buffer
	gfx_vertex_bufferT* vb = gfx_vertex_buffer_mk(10000, "position:3|color:4|texcoord:2|normal:3");
	gfx_vertex_buffer_add_index(vb, 10000);

	zvec_add(garbage, vb);

	//define a sphere
	int vend;

	int j, k;
	
	int vc = 0;
	for (j = -10; j <= 10; j++) {
		for (k = -10; k <= 10; k++) {

			float fy = j / 10.0f;

			float s = sqrtf(1 - fy * fy);

			float fx = s * sinf(k / 10.0 * 3.141);
			float fz = s * cosf(k / 10.0 * 3.141);

			gfx_vertex_data3(vb, 0, fx, fy, fz);						//position
			gfx_vertex_data4(vb, 1, 1.0, 1.0, 1.0, 1.0);					//color
			gfx_vertex_data2(vb, 2, k / 20.0, -(j + 10) / 20.0);	//texcoord
			gfx_vertex_done3(vb, 3, fx, fy, fz);						//normal
			


			if (k < 10 && j < 10) {
				 gfx_index_triangle(vb, vc, vc + 1, vc + 21);
				 vend = gfx_index_triangle(vb, vc + 1, vc + 21, vc + 22);
			}
			vc++;
			
		}
	}

	zeventT ev;

	float ang = 0;
	float ang1 = 0;
	float ang2 = 0;
	zbool mr = ZFALSE;


	vec3set(cam.pos, 0, 1.5, 5);

	//main loop
	for (;;) {

		float yaw = 0;
		float pitch = 0;
		float roll = 0;

		//inner event loop
		while (zwin->event(zwin, &ev)) {

			//printf(" ZEVENT %x %x %x %c     %x\n", ev.type, ev.a, ev.b, ev.a, ZKEY_CTRL);
			zprintevent(&ev);


			if (ZEVENTIS(ev.type, ZEVENT_CHAR)) {

				//	printf(" ZEVENT CHAR %x %x %x %c     %x\n", ev.type, ev.a, ev.b, ev.a, ZKEY_CTRL);

				if (ev.a == 'm')
					gfx_mouse_relative(zwin, mr ^= 1);

				if (ev.a == '+')
					speed *= 1.1;

				if (ev.a == '-')
					speed /= 1.1;



				if (ev.a == 'H')
					sh++;
				if (ev.a == 'h')
					sh--;

				if (ev.a == '.')
					frame++;

				if (ev.a == ',')
					frame--;


				if (ev.a == 'g') {
					ram_free(st_sphere->shader_group);
					ram_free(st_sphere->shader_group);
					st_sphere->shader_group = NULL;
					st_strawberry->shader_group = NULL;
				}
				if (ev.a == 'G') {
					st_sphere->shader_group = ram_addref(fsg);
					st_strawberry->shader_group = ram_addref(fsg);

				}

				if (ev.a == 'k') {
					gfx_style_set_property(st_sphere, GFX_SWITCH, "XXX", 0, 0, NULL, 0);
					gfx_style_set_property(st_strawberry, GFX_SWITCH, "XXX", 0, 0, NULL, 0);

				}
				
			}


			if (ZEVENTIS(ev.type, ZEVENT_KEY | ZEVENT_DOWN | ZKEY_CTRL) && (ev.a == 'q')) {
				
				zwin->close(zwin);
				break;
			}

			if (ZEVENTIS(ev.type, ZEVENT_DELTA)) {

				pitch -= ((zint32)ev.b) / 200.0;
				yaw -= ((zint32)ev.a) / 200.0;

			}
		}

		if (ev.type == ZEVENT_CLOSE)
			break;

		gfx_style_set_property(st_strawberry, GFX_FLOAT, "specular_exponent", 0, 0, &sh, 0);
		gfx_style_set_property(st_sphere, GFX_FLOAT, "specular_exponent", 0, 0, &sh, 0);
		gfx_style_set_property(st_cube, GFX_FLOAT, "specular_exponent", 0, 0, &sh, 0);

		gfx_style_set_property(st_grid, GFX_FLOAT, "specular_exponent", 0, 0, &sh, 0);

		float forward = 0.0;
		float right = 0.0;
		float up = 0.0;
		float rspeed = .04; //rotation speed for keyboard controls
		if (gx_keystate('a')) right -= speed;
		if (gx_keystate('d')) right += speed;
		if (gx_keystate('w')) forward += speed;
		if (gx_keystate('s')) forward -= speed;
		if (gx_keystate('r')) up += speed;
		if (gx_keystate('f')) up -= speed;

		if (gx_keystate('q'))  roll -= .01;
		if (gx_keystate('e'))  roll += .01;

		if (gx_keystate(ZKEY_LEFT))  yaw += rspeed;
		if (gx_keystate(ZKEY_RIGHT))  yaw -= rspeed;

		if (gx_keystate(ZKEY_UP))  pitch += rspeed;
		if (gx_keystate(ZKEY_DOWN))  pitch -= rspeed;

		gfx_camera_motion_6dof(&cam, forward, right, up, yaw, pitch, roll);

		gfx_depth_buffer(ZTRUE, ZTRUE);

		gfx_background_color(.3, .2, .1, 0);
		gfx_frame_clear(ZTRUE, ZTRUE);

		gfx_setup_3d(80, (float)zwin->w / (float)zwin->h, .01, 1000);
	
		gfx_camera_view(&cam);


		//vec3set(lpcam, 0, 0, 5);
		vec3set(ldcam, 1, 1, 0);
		//gfx_trans_vec3(&lpcam); //transform point for light 
		//gfx_trans_dir_vec3(&ldcam); //transform dir for light 

		lpcam = cam.pos; //light is at the camera position
		

 		gfx_style_set_property(st_strawberry , GFX_FLOAT3, "light_position", 0, 0, &lpcam, GFX_TRANSFORM_POINT);
		gfx_style_set_property(st_sphere, GFX_FLOAT3, "light_position", 0, 0, &lpcam, GFX_TRANSFORM_POINT);
		gfx_style_set_property(st_cube, GFX_FLOAT3, "light_position", 0, 0, &lpcam, GFX_TRANSFORM_POINT);
		gfx_style_set_property(st_grid, GFX_FLOAT3, "light_position", 0, 0, &lpcam, GFX_TRANSFORM_POINT);
		
		//gfx_style_set_property(st_sphere, GFX_FLOAT3, "light_direction", 0, 0, &ldcam, GFX_TRANSFORM_DIRECTION);
		//gfx_style_set_property(st_strawberry, GFX_FLOAT3, "light_direction", 0, 0, &ldcam, GFX_TRANSFORM_DIRECTION);
				
		
		vec3 latten = vec3const(.1, 0, .1);

		gfx_style_set_property(st_strawberry, GFX_FLOAT3, "light_attenuation", 0, 0, &latten, 0);


		vec4 purple;
		vec3set(purple, 1, 0, 1);
		vec3 green;
		vec3set(green, 0, 1, 0);

		vec3 black;
		vec3set(black, 0, 0, 0);

		vec3 white;
		vec3set(white, 1, 1, 1);


		vec3 gray;
		vec3set(gray, .3, .3,.3);

		float fd = 0.1;
		gfx_style_set_property(st_sphere, GFX_FLOAT3, "fog_color", 0, 0, &purple, 0);
		gfx_style_set_property(st_sphere, GFX_FLOAT, "fog_density", 0, 0, &fd, 0);


 		gfx_style_set_property(st_sphere, GFX_FLOAT3, "specular", 0, 0, &white, 0);
		gfx_style_set_property(st_strawberry, GFX_FLOAT3, "specular", 0, 0, &white, 0);


		



		gfx_translate3(0, 0, -5);
		//gfx_rotate_y(ang);

		gfx_style(st_sphere);

		
		gfx_vertex_buffer_draw(vb, GFX_TRIANGLE, 0, vend, ZTRUE);

	
	
		
		gfx_translate3(0, 5, -1);
		gfx_style(st_strawberry);

		gfx_meshT* m;

		for (m = strawberry_mesh; m; m = m->next) {
			
 			gfx_vertex_buffer_draw(m->vb, GFX_TRIANGLE, 0, zarray_count(m->vb->index_buffer), ZTRUE);
		}
				

		gfx_style_set_property(st_cube, GFX_FLOAT3, "specular", 0, 0, &gray, 0);

		gfx_style(st_cube);
		gfx_translate3(3, -5, 4);
   		gfx_vertex_buffer_draw(cube_mesh->vb, GFX_TRIANGLE, 0, zarray_count(cube_mesh->vb->index_buffer), ZTRUE);

		gfx_identity();
		gfx_camera_view(&cam);
		gfx_translate3(0,-2, -3);
		gfx_style(st_grid);
		//gfx_translate3(3, 0, 4);
		
		
	
	
		gfx_transformT tr;
		gfx_save_transform(&tr);

		gfx_identity();
		recurse_skeleton(skel, frame, 0);
		
#if 0
		for (m = model; m; m = m->next) {
			
			int i;
			//copy from original
			//copy from original
			//memcpy(m->vbaux->attributes[m->vbaux->fixed_position].data, m->vb->attributes[m->vb->fixed_position].data, sizeof(float) * 3 * m->vbaux->count);

			//copy to original
			memcpy(m->vb->attributes[m->vb->fixed_position].data, m->vbaux->attributes[m->vbaux->fixed_position].data, sizeof(float) * 3 * m->vbaux->count);
			memcpy(m->vb->attributes[m->vb->fixed_normal].data, m->vbaux->attributes[m->vbaux->fixed_normal].data, sizeof(float) * 3 * m->vbaux->count);

			for (i = 0; i < m->vbaux->count; i++) {
				gfx_jointT* joint = zvec_get_at(&skel->bones, m->bone[i]); //get the closest bone
				vec3* p = m->vb->attributes[m->vb->fixed_position].data + 3 * i;
			
				//if (!joint->parent)
				//	continue;
				gfx_load_transform(&joint->parent->stransform);  //for our own closest algo
				
				//gfx_load_transform(&joint->stransform);  //for blender imported vertex groups
			
				//if (m->bone1[i]) {
					//gfx_jointT* joint1 = zvec_get_at(&skel->bones, m->bone1[i]); //get the closest bone
				//gfx_blend_transform(1.0 - m->bone_blend[i], m->bone_blend[i], &joint1->stransform); //

			//	if (joint->parent)
				//	gfx_blend_transform(1.0 - m->bone_blend[i], m->bone_blend[i], & joint->parent->stransform); //

				//}

				//vec3add(*p, joint->parent->accumulated_offset);
			
				gfx_trans_vec3(p);
				
				p = m->vb->attributes[m->vb->fixed_normal].data + 3 * i;
				gfx_trans_dir_vec3(p);
				
			}
			gfx_vertex_buffer_update(m->vb);
			gfx_load_transform(&tr);
			//gfx_vertex_buffer_draw(m->vbaux, GFX_POINT, 0, m->vbaux->count, ZFALSE);
			//gfx_vertex_buffer_draw(m->vb, GFX_POINT, 0, m->vb->count, ZFALSE);
		}
	
#endif
		gfx_load_transform(&tr);
		for (m = model; m; m = m->next) {
			gfx_vertex_buffer_update(m->vb);
			gfx_vertex_buffer_draw(m->vb, GFX_TRIANGLE, 0, zarray_count(m->vb->index_buffer), ZTRUE);
			//gfx_vertex_buffer_draw(m->vb, GFX_POINT, 0, zarray_count(m->vb->index_buffer), ZTRUE);
		}
		gfx_style(NULL);
		gfx_translate3(-2, 0, 0);
		for (m = model; m; m = m->next) {
			//gfx_vertex_buffer_draw(m->vbaux, GFX_TRIANGLE, 0, zarray_count(m->vbaux->index_buffer), ZTRUE);
			if (m->vbaux)
				gfx_vertex_buffer_draw(m->vbaux, GFX_POINT, 0, m->vbaux->count, ZFALSE);
		//	gfx_vertex_buffer_draw(m->vdebug, GFX_LINE, 0, m->vdebug->count, ZFALSE);
		}
		

	
		vec3 z = vec3const(0, 0, 0);
		vec3 o = vec3const(1, 1, 1);


		gfx_arrow_start();
	//	gfx_arrow(&z, &o);
		
		recurse_skeleton(skel, frame, SKEL_OP_DRAW);
		gfx_identity();
		gfx_arrow_end();

		gfx_style(NULL);
		gfx_identity();

		/*
		gfx_vertex_bufferT* vbt = gfx_vertex_temp(zwin, "position:3|color:4"); //make or recycle a temp vertex buffer
		gfx_vertex_data4(vbt, 1, 1.0, 0.0, 0.0, 1.0);
		gfx_vertex_done3(vbt, 0, 0.0 + cam.pos.named.x , -.2, -2);

		gfx_vertex_data4(vbt, 1, 0.0, 1.0, 0.0, 1.0);
		gfx_vertex_done3(vbt, 0, 1.0, 0.0, -2);

		gfx_vertex_data4(vbt, 1, 1.0, 0.0, 1.0, 1.0);
		gfx_vertex_done3(vbt, 0, 1.0, 1.0, -2);


		gfx_vertex_data4(vbt, 1, 0.0, 0.0, 1.0, 1.0);
		gfx_vertex_done3(vbt, 0, 0.0, 1.0, -2);
		
		gfx_vertex_buffer_draw_clear(vbt, GFX_QUAD);
		*/
		zwin->pixels(zwin, NULL);	//display the framebuffer
		ang += .01;
	}

	printf(" window close button was pressed\n");
	gfx_free_basic_shader();
	ram_free(garbage);

}









typedef struct  {
	int v1;
	int v2;
	int v3;
	char* contentstr;
}face;

zbool free_face(void* v) {
	face* f = v;
	ram_free(f->contentstr);
	return ZTRUE;
}

typedef struct {
	float x, y, z;
}point;



typedef struct {
	int a;
	zvecT faces;
	zvecT points;
	zvecT normals;
	zvecT texcoords;
	char* materialname;
}submesh;

zbool free_submesh(void* v) {
	submesh* s = v;
	zvec_cleanup(&s->faces);
	zvec_cleanup(&s->points);
	zvec_cleanup(&s->normals);
	zvec_cleanup(&s->texcoords);
	ram_free(s->materialname);
	return ZTRUE;
}

typedef struct {
	zvecT submeshes;
} doc;

zbool free_doc(void*v) {
	doc* d = v;
	zvec_cleanup(&d->submeshes);
	return ZTRUE;
}

int main(int argc, char** args){
	
	yxml_t* parser = zxml_mk(64);


	//FILE* f = fopen("../../zcore-data/test.mesh.mesh.xml", "rb");
	FILE* f = fopen("../../zcore-data/mini.xml", "rb");

	face ff;
	face* tf = &ff;
	ff.v1 = 88888;
	zxmlhandlerT handlers[30];
	memset(handlers, 0, sizeof(handlers));
	int hc = 0;

	zxmlhandlerT* hface = 
		zxml_set_handler(&handlers[hc++], "@v1", ZXML_WRITE_INT, offsetof(face, v1), 0, NULL, NULL);
		zxml_set_handler(&handlers[hc++], "@v2", ZXML_WRITE_INT, offsetof(face, v2), 0, NULL, NULL);
		zxml_set_handler(&handlers[hc++], "@v3", ZXML_WRITE_INT, offsetof(face, v3), 0, NULL, NULL);
		zxml_set_handler(&handlers[hc++], ".", ZXML_ADD_STRING, offsetof(face, contentstr), 0, NULL, NULL);
	
	hc++;

	zxmlhandlerT* hpoint =
		zxml_set_handler(&handlers[hc++], "@x", ZXML_WRITE_FLOAT32, offsetof(point, x), 0, NULL, NULL);
		zxml_set_handler(&handlers[hc++], "@y", ZXML_WRITE_FLOAT32, offsetof(point, y), 0, NULL, NULL);
		zxml_set_handler(&handlers[hc++], "@z", ZXML_WRITE_FLOAT32, offsetof(point, z), 0, NULL, NULL);
		zxml_set_handler(&handlers[hc++], "@u", ZXML_WRITE_FLOAT32, offsetof(point, x), 0, NULL, NULL);
		zxml_set_handler(&handlers[hc++], "@v", ZXML_WRITE_FLOAT32, offsetof(point, y), 0, NULL, NULL);
	hc++;




	zxmlhandlerT* hsubmesh = 
		zxml_set_handler(&handlers[hc++], "face",     ZXML_ADD_VECTOR, offsetof(submesh, faces), sizeof(face), hface, free_face);
		zxml_set_handler(&handlers[hc++], "position", ZXML_ADD_VECTOR, offsetof(submesh, points), sizeof(point), hpoint, NULL);
		zxml_set_handler(&handlers[hc++], "normal", ZXML_ADD_VECTOR, offsetof(submesh, normals), sizeof(point), hpoint, NULL);
		zxml_set_handler(&handlers[hc++], "texcoord", ZXML_ADD_VECTOR, offsetof(submesh, texcoords), sizeof(point), hpoint, NULL);
		zxml_set_handler(&handlers[hc++], "@material", ZXML_ADD_STRING, offsetof(submesh, materialname), 0, NULL, NULL);
		

	hc++;

	
	zxmlhandlerT* hdoc =
		zxml_set_handler(&handlers[hc++], "submesh", ZXML_ADD_VECTOR, offsetof(doc, submeshes), sizeof(submesh), hsubmesh, free_submesh);
	
	hc++;

	doc* dd;
	dd = ram_alloc(sizeof(doc), free_doc);


	zparsexml(parser, f, fgetc, hdoc, dd);
	{
		int i;
		int j;
		for (j = 0; j < zvec_count(&dd->submeshes); j++) {
			submesh* d = zvec_get_at(&dd->submeshes, j);
			
			for (i = 0; i < zvec_count(&d->faces); i++) {
				face* f = zvec_get_at(&d->faces, i);
				printf(" %d %d %d\n", f->v1, f->v2, f->v3);

				if (f->contentstr)
					printf(" ---- content %s\n", f->contentstr);


			}

			for (i = 0; i < zvec_count(&d->points); i++) {
				point* f = zvec_get_at(&d->points, i);
				printf("position %f %f %f", f->x, f->y, f->z);


				f = zvec_get_at(&d->normals, i);
				printf("normal  %f %f %f ", f->x, f->y, f->z);


				f = zvec_get_at(&d->texcoords, i);
				printf("texcoords  %f %f %f (zero) \n ", f->x, f->y, f->z);



				printf("\n");
			}

			

			

		}

	}
	fclose(f);
	ram_free(parser);
	ram_free(dd);

	printf("%d allocations left\n", ram_allocs());
	return 1;

	gfx_gl_test();
	 
	printf("%d allocations left\n", ram_allocs());

}



