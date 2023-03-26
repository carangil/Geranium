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


	zwindowT* zwin = gfx_mkwindow("Testing", 1024, 768, 0);
	zvec_add(garbage, zwin);

	zbitmapT* earthpic = zbitmap_load_tga("../../Zcore-data/earth-cylindrical-alpha-holes.tga", 1 * ZTGA_TOP);
	zbitmapT* strawpic = zbitmap_load_tga("../../Zcore-data/web/strawberry/Texture/Strawberry_basecolor.tga", 0 * ZTGA_TOP);
	zvec_add(garbage, earthpic);
	zvec_add(garbage, strawpic);

	zbitmapT* brickpic = zbitmap_load_tga("../../Zcore-data/web/brick.tga", 0);
	zbitmapT* bricknorm = zbitmap_load_tga("../../Zcore-data/web/brick_normal.tga", 0);
	zvec_add(garbage, brickpic);
	zvec_add(garbage, bricknorm);




	gfx_meshT* cube_mesh = gfx_mesh_load_obj("../../Zcore-data/cube.obj", 1.0);
	gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/strawberry/Strawberry_obj.obj", 1.0);
	zvec_add(garbage, strawberry_mesh);
	zvec_add(garbage, cube_mesh);
	float speed = .05;

	gfx_textureT* strawtex = gfx_texture_mk(strawpic);
	gfx_textureT* earthtex = gfx_texture_mk(earthpic);
	gfx_textureT* bricktex = gfx_texture_mk(brickpic);
	gfx_textureT* bricknormtex = gfx_texture_mk(bricknorm);
	zvec_add(garbage, strawtex);
	zvec_add(garbage, earthtex);
	zvec_add(garbage, bricktex);
	zvec_add(garbage, bricknormtex);


	gfx_styleT* st_sphere = gfx_style_mk();
	gfx_styleT* st_strawberry = gfx_style_mk();
	gfx_styleT* st_cube = gfx_style_mk();
	zvec_add(garbage, st_sphere);
	zvec_add(garbage, st_strawberry);
	zvec_add(garbage, st_cube);


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


	gfx_style_set_property(st_sphere, GFX_FLOAT4, "light_color", 0, 0, &lcol, 0);
	gfx_style_set_property(st_sphere, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);
	gfx_style_set_property(st_sphere, GFX_TEXTURE, "texture_diffuse", 0, 0, earthtex, 0);
	
	gfx_style_set_property(st_cube, GFX_TEXTURE, "texture_diffuse", 0, 0, bricktex, 0);
	gfx_style_set_property(st_cube, GFX_TEXTURE, "texture_normal_tangent", 0, 0, bricknormtex, 0);
	
	gfx_style_set_property(st_cube, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);

	gfx_style_set_property(st_cube, GFX_SWITCH, "pass_view_3x3", 0, 0, &lam, 0);


	gx_shadergroupT* tsg = gx_shader_source("@../gfxtest/finevertex.glsl", "@../gfxtest/finefragment.glsl");
	zvec_add(garbage, tsg);
	st_sphere->shader_group = ram_addref(tsg);
	st_strawberry->shader_group = ram_addref(tsg);
	st_cube->shader_group = ram_addref(tsg);

	gfx_cameraT cam;
	gfx_camera_init(&cam);

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

			gfx_vertex_data(vb, 0, fx, fy, fz, 0);						//position
			gfx_vertex_data(vb, 1, 1.0, 1.0, 1.0, 1.0);					//color
			gfx_vertex_data(vb, 2, k / 20.0, -(j + 10) / 20.0, 0, 0);	//texcoord
			gfx_vertex_done(vb, 3, fx, fy, fz, 0);						//normal
			


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



				if (ev.a == 'g') {
					ram_free(st_sphere->shader_group);
					ram_free(st_sphere->shader_group);
					st_sphere->shader_group = NULL;
					st_strawberry->shader_group = NULL;
				}
				if (ev.a == 'G') {
					st_sphere->shader_group = ram_addref(tsg);
					st_strawberry->shader_group = ram_addref(tsg);

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

		for (m = strawberry_mesh; m; m = m->next_piece) {
			
 			gfx_vertex_buffer_draw(m->vb, GFX_TRIANGLE, 0, zarray_count(m->vb->index_buffer), ZTRUE);
		}
				

		gfx_style_set_property(st_cube, GFX_FLOAT3, "specular", 0, 0, &gray, 0);

		gfx_style(st_cube);
		gfx_translate3(3, -5, -1);
   		gfx_vertex_buffer_draw(cube_mesh->vb, GFX_TRIANGLE, 0, zarray_count(cube_mesh->vb->index_buffer), ZTRUE);


		zwin->pixels(zwin, NULL);	//display the framebuffer
		ang += .01;
	}

	printf(" window close button was pressed\n");

	ram_free(garbage);

}


int main(int argc, char** args){

	gfx_gl_test();
	 
	printf("%d allocations left\n", ram_allocs());

}



