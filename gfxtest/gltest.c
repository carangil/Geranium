#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"

#include "math.h"
#include "zbitmap.h"

#include "gfx_gl.h"






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


	zbitmapT* grid = zbitmap_load_tga("../../Zcore-data/finechecker.tga", 1 * ZTGA_TOP);
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

	zvecT* ignore = zstrsplit(NULL, "__0|root-end|hips|arm.pool.r|arm.pool.l|arm.ik.r|arm.ik.l|leg.ik.l|leg.ik.r|leg.pool.l|leg.pool.r|root|hips.comtroll|arm.pool.r-end|arm.pool.l-end|arm.ik.r-end|arm.ik.l-end|leg.ik.l-end|leg.ik.r-end|leg.pool.l-end|leg.pool.r-end", '|');

	gfx_jointT* skel = load_bvh("../../Zcore-data/web/metal_hands.bvh", .1, ignore);
	//gfx_jointT* skel = load_bvh("../../Zcore-data/test.bvh", 2);

;
	zvec_add(garbage, skel);

	gfx_meshT* cube_mesh = gfx_mesh_load_obj("../../Zcore-data/cube.obj", 1.0);
	gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/strawberry/Strawberry_obj.obj", 1.0);
	zvec_add(garbage, strawberry_mesh);
	zvec_add(garbage, cube_mesh);

	gfx_meshT* model = gfx_mesh_load_obj("../../Zcore-data/web/metal_hands.obj", .1);
	zvec_add(garbage, model);


	//map points to bones
	gfx_identity();
	recurse_skeleton(skel, -1, 0); //process the skeleton rest pose


	{
		gfx_meshT* m;
		int i;

		for (m = model; m; m = m->next) {

			int count = m->vb->count;
			

			m->vbaux = gfx_vertex_buffer_mk(count, "position:3|color:4"); 
			m->bone = zarray_alloc(zuint16, count);

			for (i = 0; i < count; i++) {

				vec3 p;

				p.named.x = m->vb->attributes[m->vb->fixed_position].data[i * 3];
				p.named.y = m->vb->attributes[m->vb->fixed_position].data[i * 3+1];
				p.named.z = m->vb->attributes[m->vb->fixed_position].data[i * 3+2];

				vec4 color = vec4const(1, 0, 0, 1);
				float closestd = 9999;
				int j;
				for (j = 0; j < zvec_count(&skel->bones); j++) {
					gfx_jointT* joint = zvec_get_at(&skel->bones, j);
					if (joint->ignored)
						continue;

					float t;
					for (t = 0; t <= 1; t += .1) {  //step along the bone line

						

						vec3 q = joint->point;
						vec3scale(q, t);
						vec3madd(q, 1 - t, joint->parent->point);	



						vec3sub(q, p );
						float d = vec3abs_sq(q);
						if (d < closestd) {
							closestd = d;
							m->bone[i] = j;//set ith point to use jth bone
							color = joint->parent->debugcolor;
						}

					}


				}


				gfx_vertex_data3(m->vbaux, 0, p.named.x, p.named.y, p.named.z);

 				gfx_vertex_done4(m->vbaux,1,    color.named.x, color.named.y, color.named.z, 1);

			}


		}
	}


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
	vec4 lcol = vec4const(1, 1, .7, 1.0);
	vec4 lam = vec4const(.1, .1, .2, 1.0);


	float sh = 100.0;
	//set light parameters on both
	gfx_style_set_property(st_strawberry, GFX_FLOAT4, "light_color", 0, 0, &lcol, 0);
	gfx_style_set_property(st_strawberry, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);
	gfx_style_set_property(st_strawberry, GFX_TEXTURE, "texture_diffuse", 0, 0, strawtex, 0);

	gfx_style_set_property(st_grid, GFX_TEXTURE, "texture_diffuse", 0, 0, gridtex, 0);

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

		gfx_setup_3d(80, (float)zwin->w / (float)zwin->h, .1, 1000);
	
		gfx_camera_view(&cam);


		//vec3set(lpcam, 0, 0, 5);
		vec3set(ldcam, 1, 1, 0);
		//gfx_trans_vec3(&lpcam); //transform point for light 
		//gfx_trans_dir_vec3(&ldcam); //transform dir for light 

		lpcam = cam.pos; //light is at the camera position
		

 		gfx_style_set_property(st_strawberry , GFX_FLOAT3, "light_position", 0, 0, &lpcam, GFX_TRANSFORM_POINT);
		gfx_style_set_property(st_sphere, GFX_FLOAT3, "light_position", 0, 0, &lpcam, GFX_TRANSFORM_POINT);
		gfx_style_set_property(st_cube, GFX_FLOAT3, "light_position", 0, 0, &lpcam, GFX_TRANSFORM_POINT);
		
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
		
		
	//	for (m = model; m; m = m->next) {
		//	gfx_vertex_buffer_draw(m->vb, GFX_POINT, 0, zarray_count(m->vb->index_buffer), ZTRUE);
	//	}

	
		gfx_transformT tr;
		gfx_save_transform(&tr);

		gfx_identity();
		recurse_skeleton(skel, frame, 0);
		

		for (m = model; m; m = m->next) {
			
			int i;
			//copy from original
			memcpy(m->vbaux->attributes[m->vbaux->fixed_position].data, m->vb->attributes[m->vb->fixed_position].data, sizeof(float) * 3 * m->vbaux->count);
			for (i = 0; i < m->vbaux->count; i++) {
				gfx_jointT* joint = zvec_get_at(&skel->bones, m->bone[i]); //get the closest bone
				vec3* p = m->vbaux->attributes[m->vbaux->fixed_position].data + 3 * i;
			
				
				gfx_load_transform(&joint->parent->stransform);
				vec3add(*p, joint->parent->accumulated_offset);
			
				gfx_trans_vec3(p);
				
			}
			gfx_vertex_buffer_update(m->vbaux);
			gfx_load_transform(&tr);
			gfx_vertex_buffer_draw(m->vbaux, GFX_POINT, 0, m->vbaux->count, ZFALSE);
			//gfx_vertex_buffer_draw(m->vb, GFX_POINT, 0, m->vb->count, ZFALSE);
		}
		
		gfx_style(NULL);
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


int main(int argc, char** args){

	gfx_gl_test();
	 
	printf("%d allocations left\n", ram_allocs());

}



