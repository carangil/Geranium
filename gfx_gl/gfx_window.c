#define GFXINTERNAL
#include "ztypes.h"
#include "zmem.h"
#include "zwindow.h"
#include "gfx_gl.h"
#include "zvector.h"
#include "zstring.h"
#include "string.h"
#include "zarray.h"
#include <stdio.h>

/*opengl error checker*/
char* last_file;
int last_line;
char* last_hint = NULL;

void checkGLfunc(char* file, int line, char* hint, zbool tolerable) {
	int err;

	for (err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
		
			printf("OPENGL ERROR %x FROM %s:%d %s to %s:%d  %s\n", err, last_file, last_line, last_hint,  file, line, hint);
		
			if (tolerable)
				printf(" ^ error is potentially expected and OK\n");
			else
				printf("Error is not expected.  Set breakpoint here.\n");
			
	}

	last_hint = hint;
	last_file = file;
	last_line = line;

}

/*glfw windowing and zevent interface*/
typedef struct gfx_windowS {
	zwindowT iface;	//the zevent window interface
	GLFWwindow* fwindow;
}gfx_windowT;

void errorHandler(int error, const char* message) {
	printf(" glfw e");
	printf("GLFW ERROR:%d (0x%x) %s\n", error, error, message ? message : "null");

}

zuint32 keymodstate = 0;
zbool keystatus[256];

zbool gx_keystate(zuint32 key) {

	if (key < 256)
		return keystatus[key];

	return ZFALSE;

}

void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

	int zkey = 0;

	//printf(" Key: %d %x %c\n", key, key, key);

	//translate to ZEVENT keys

	switch (key) {
	case GLFW_KEY_ENTER:		zkey = ZKEY_ENTER;		break;
	case GLFW_KEY_BACKSPACE:	zkey = ZKEY_BACKSPACE;	break;
	case GLFW_KEY_ESCAPE:		zkey = ZKEY_ESCAPE;		break;
	case GLFW_KEY_TAB:			zkey = ZKEY_TAB;		break;

		//add others later
	case GLFW_KEY_LEFT:			zkey = ZKEY_LEFT;		break;
	case GLFW_KEY_RIGHT:		zkey = ZKEY_RIGHT;		break;
	case GLFW_KEY_UP:			zkey = ZKEY_UP;			break;
	case GLFW_KEY_DOWN:			zkey = ZKEY_DOWN;		break;
	case GLFW_KEY_LEFT_CONTROL:	zkey = ZKEY_CTRL;		break;
	case GLFW_KEY_LEFT_SHIFT:	zkey = ZKEY_SHIFT;		break;
	case GLFW_KEY_LEFT_ALT:		zkey = ZKEY_ALT;		break;
	case GLFW_KEY_RIGHT_CONTROL: zkey = ZKEY_RCTRL;		break;
	case GLFW_KEY_RIGHT_SHIFT:	zkey = ZKEY_RSHIFT;		break;
	case GLFW_KEY_RIGHT_ALT:	zkey = ZKEY_RALT;		break;

	}

	if ((key < 256) & (!zkey)) {

		//GLFW uses ascii for lots of printing characters, so does ZEVENT
		zkey = key;

		if ((zkey >= 'A') && (zkey <= 'Z'))
			zkey = zkey - 'A' + 'a'; //convert to lowercase

	}

	keymodstate = 0;

	if (mods & GLFW_MOD_SHIFT)
		keymodstate |= ZKEY_SHIFT;

	if (mods & GLFW_MOD_CONTROL)
		keymodstate |= ZKEY_CTRL;

	if (mods & GLFW_MOD_ALT)
		keymodstate |= ZKEY_ALT;

	// the modifiers
	int keystate = ZEVENT_KEY;

	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT) ) {
		keystate |= ZEVENT_DOWN | keymodstate;

		if (zkey < 256)
			keystatus[zkey] = 1;

	}

	if (action == GLFW_RELEASE) {
		keystate |= ZEVENT_UP;
		//zevent doesn't care about if shift/alt/ctrl are held down while releasing a key
		if (zkey < 256)
			keystatus[zkey] = 0;
	}

	gfx_windowT* win= glfwGetWindowUserPointer(window);
	
	if (win == NULL) {
		printf(" not a zevent window! serious bug?\n");
	}

	zw_enqueue(&win->iface, keystate, zkey, 0, 0);
}

void charHandler(GLFWwindow* window, unsigned int character) {

	//glfw doesn't do modifiers with character callbacks
	//todo: we could probably read or track keystate
	//also might want to generate characters for backspace, enter, tab, etc, as they do have character codes (they keydown/repeat event should do that)
	gfx_windowT* win = (gfx_windowT*)glfwGetWindowUserPointer(window); ;
	zw_enqueue(&win->iface, ZEVENT_CHAR, character, 0, 0);
}

zint32 last_mouse_x=0;
zint32 last_mouse_y=0;
zuint32 mouse_button_state=0;

zbool mouse_relative = ZFALSE;

void gfx_mouse_relative(zwindowT* zw, zbool rel) {

	if (rel) {
		mouse_relative = 1;  //turn on relative mode  .throw away first value
		glfwSetInputMode(((gfx_windowT*)zw)->fwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}
	else {
		mouse_relative = 0;  //to disable
		glfwSetInputMode(((gfx_windowT*)zw)->fwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);		
	}

}

void mousemoveHandler(GLFWwindow* window, double x, double y) {

	int dx = 0;
	int dy = 0;
	gfx_windowT* win = (gfx_windowT*)glfwGetWindowUserPointer(window); ;
	
	if (mouse_relative) {
		dx = ((zint32)x) - last_mouse_x;
		dy = ((zint32)y) - last_mouse_y;
		
	}


	last_mouse_x = (zuint32)x;
	last_mouse_y = (zuint32)y;

	if (mouse_relative) {
		zw_enqueue(&win->iface, ZEVENT_MOUSE | ZEVENT_DELTA | mouse_button_state | keymodstate, (zuint32)dx, (zuint32)dy, NULL);
		
		return;
	}

	zw_enqueue(&win->iface, ZEVENT_MOUSE | ZEVENT_MOVE | mouse_button_state | keymodstate, (zuint32) last_mouse_x, (zuint32) last_mouse_y, NULL );
}


void mousebuttonHandler(GLFWwindow* window, int button, int action, int mods) {

	gfx_windowT* win = (gfx_windowT*)glfwGetWindowUserPointer(window);
	zuint32 event = ZEVENT_MOUSE;
	zuint32 statebit = 0;
	switch (button) {

	case GLFW_MOUSE_BUTTON_LEFT:
		event |= ZEVENT_MOUSE_L;	//for the event of pressing
		statebit = ZEVENT_MOUSE_STATE_L; //for the continous state of being pressed
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		event |= ZEVENT_MOUSE_R;
		statebit = ZEVENT_MOUSE_STATE_R;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		event |= ZEVENT_MOUSE_M;
		statebit = ZEVENT_MOUSE_STATE_M;
		break;

	default:
		return; //don't sent events for things we don't know

	}

	if (action == GLFW_PRESS) {

		mouse_button_state |= statebit;  //set mouse button state bit

		event |= ZEVENT_DOWN;  

	}
	else { //release

		mouse_button_state &= (~statebit);  //set clear state bit

		event |= ZEVENT_UP;

	}

	if (mouse_relative)
		zw_enqueue(&win->iface, event | mouse_button_state | keymodstate, 0, 0, NULL);  //button clicks don't move mouse
	else
		zw_enqueue(&win->iface, event | mouse_button_state | keymodstate, (zuint32)last_mouse_x, (zuint32)last_mouse_y, NULL);
	
}

/* one-time initialization */
zbool gfxi_inited = ZFALSE;
void gfxi_init() {
	
	if (!glfwInit()) {
		printf(" Can't init glfw\n");
		return;

	}
	glfwSetErrorCallback(errorHandler);

	gfxi_inited = ZTRUE;
}

//event interface
 zbool gfx_event(zwindowT * zw, zeventT * ev) {
		
	glfwPollEvents(); //enqueue events

	//read from queue first
 	if (zw_event(zw, ev)) {
		printf(" RETURNING QUEUED EVENT\n");
		return ZTRUE;
	}

	gfx_windowT* win = (gfx_windowT*)zw;

	if (glfwWindowShouldClose(win->fwindow))  {
		//glfw user is trying to close window
		ev->type = ZEVENT_CLOSE;
		return ZFALSE;
	}
		
	ev->type = ZEVENT_NONE;
	return ZFALSE;
}

void gfx_pixels(zwindowT * zw, void* px) {
	gfx_windowT* win = (gfx_windowT*)zw;

	if (px == NULL)
		glfwSwapBuffers(win->fwindow);
	else {
		printf(" non-framebuffer pixels not supported ");
	}

	//get the window size and fix the viewport
	glfwGetWindowSize(win->fwindow, &win->iface.w, &win->iface.h);
	glViewport(0, 0, win->iface.w, win->iface.h);
	//the above is also updating the 'w' and 'h' coordinates, so the app can use them in drawing the next frame, if they are adapting to window size

	checkGL();//check for errors
}

void gfx_close(zwindowT * zw) {
	printf(" 'close' interface not supported in opengl.  click the 'x' manually\n");
}

//flags currently don't do anything
//creation of first window will init glfw

struct zwindowS* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags) {

	memset(keystatus, 0, sizeof(keystatus));

	gfx_windowT* win = ram_alloc(sizeof(gfx_windowT), NULL); //no destructor key

	if (!gfxi_inited)
		gfxi_init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 4);  //enable antialiasing buffers

	win->fwindow = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwSetWindowUserPointer(win->fwindow, win); //so glfw can give us back our own struct

	//set callbacks
	glfwSetKeyCallback(win->fwindow, keyHandler);
	glfwSetCharCallback(win->fwindow, charHandler);
	glfwSetCursorPosCallback(win->fwindow, mousemoveHandler);
	glfwSetMouseButtonCallback(win->fwindow, mousebuttonHandler);
	
	glfwMakeContextCurrent(win->fwindow);
		
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Can't init glad\n");
	}

	glEnable(GL_MULTISAMPLE); //enable antialiasing

	gfx_identity(); //clear the matrix

	//set interface functions
	win->iface.close = gfx_close;
	win->iface.pixels = gfx_pixels;
	win->iface.event = gfx_event;

	//get the window size and fix the viewport
	glfwGetWindowSize(win->fwindow, &win->iface.w, &win->iface.h);
	glViewport(0, 0, win->iface.w, win->iface.h);
	glClearColor(0, 0, 0, 1); //black window default

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	checkGL();

	return &(win->iface);
}


void gfx_arrow(vec3* p1, vec3* p2) {

	vec3 zero = vec3const(0, 0, 0);
	if (!p1)
		p1 = &zero;

	if (!p2)
		p2 = &zero;

	gxi_refresh_matrix(NULL);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);
	float s = .02f;
	glBegin(GL_TRIANGLES);
		
		glVertex3f(p1->VX-s,		p1->VY, p1->VZ-s);
		glVertex3f(p1->VX+s,	p1->VY+s, p1->VZ+s);
		glVertex3f(p2->VX,		p2->VY, p2->VZ);
				
	glEnd();


}

#if 0
/* test program */
extern int frame;


void gfx_gl_test() {
	
	//void* skel = load_bvh("H:/projects/Zcore-data/web/Example1.bvh");
	//void* skel = load_bvh("H:/projects/Zcore-data/web/realistickoreanwoman/skeleton.bvh");
	//void* skel = load_bvh("H:/projects/Zcore-data/web/metal_hands.bvh", .2);
	void* skel = load_bvh("H:/projects/Zcore-data/web/metal_hands_mod.bvh", .2);
	zwindowT* zwin = gfx_mkwindow("internal test", 1024, 768, 0);

	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/label.tga", ZTGA_TOP);
	zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/web/grid.tga", ZTGA_TOP);
	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/earth-cylindrical-alpha-holes.tga", ZTGA_TOP);
	zbitmapT* strawpic = zbitmap_load_tga("../../Zcore-data/web/strawberry/Texture/Strawberry_basecolor.tga", 0*ZTGA_TOP);

	
	
	gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/strawberry/Strawberry_obj.obj", 1.0);
	//gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/strawberry/Strawberry_obj.obj");
	//gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/1950s_Upholstered_Lounge_Chair_OBJ/1950s Upholstered Lounge Chair_OBJ.obj", .005);
	//gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/metal_hands.obj", .2);
	
	gfx_meshT* mesh2 = gfx_mesh_load_obj("../../Zcore-data/web/metal_hands.obj", .2);  //make the same mesh again (todo: proper cloning or whatever);




	//gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/metal_handspos.obj", .2);

	//gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/realistickoreanwoman/obj_file/female.obj", //.05f);

	//gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/cube.obj");

	printf(" loaded %x %d %d %d\n", pic->format, pic->w, pic->h, pic->size);

	gfx_windowT* gfx_window = (gfx_windowT*)zwin; //cast to our own specific type
	float speed = .05;
	gfx_textureT* tex = gfx_texture_mk(pic);
	gfx_textureT* strawtex = gfx_texture_mk(strawpic);
	gfx_styleT* st = gfx_style_mk(NULL);
	
	gfx_style_set_property(st, 0, "blend", 0, GFX_BLEND_ALPHA, NULL, 0);

	vec3 lpcam = vec3const(0, -2, 0);
	vec4 lcol = vec4const(1, 0, 0, 1.0);
	vec4 lam = vec4const(0, 0, .5, 1.0);

	gfx_style_set_property(st, GFX_FLOAT3, "light_position", 0, 0, &lpcam, 0);
	gfx_style_set_property(st, GFX_FLOAT4, "light_color", 0, 0, &lcol, 0);
	gfx_style_set_property(st, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);

	/*
	
	vec4set(lcol, 0, 1, 0, 1);
	vec3set(lpcam, 0, 1, 0);
		
	gfx_style_set_property(st, GFX_FLOAT3, "light_direction",	1, 0, &lpcam, 0);
	gfx_style_set_property(st, GFX_FLOAT4, "light_color",		1, 0, &lcol, 0);
	gfx_style_set_property(st, GFX_FLOAT4, "light_ambient",		1, 0, &lam, 0);
	*/
	//GLFWwindow* window = gfx_window->fwindow;


	zvec_add(&st->textures, tex);

 	gfx_style(st);

	gfx_vertex_bufferT* vb = gfx_vertex_buffer_mk(10000, "color:4|position:3|texcoord:2|normal:3");
	gfx_vertex_buffer_add_index(vb, 10000);


	gfx_cameraT cam;
 	gfx_camera_init(&cam);

	

	//junky sphere

	int j, k;
	int v=0;
	int vc = 0;
	for (j = -10; j <= 10; j++) {
		for (k = -10; k <= 10; k++) {

			float fy = j / 10.0f;

			float s = sqrtf(1- fy*fy);

			float fx = s*sinf(k /10.0 *3.141);
			float fz = s*cosf(k /10.0 * 3.141);

			gfx_vertex_data(vb, 0, 1, 1, 1, 1);
			gfx_vertex_data(vb, 2,  k/20.0  , -(j + 10) / 20.0, 0, 1);
 			gfx_vertex_data(vb, 3, fx, fy, fz, 1);
			gfx_vertex_done(vb, 1, fx, fy, fz, 0);
			

			if (k < 10 && j < 10) {
				v=gfx_index_triangle(vb, vc, vc + 1, vc + 21);
				v = gfx_index_triangle(vb, vc+1, vc + 21, vc + 22);
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


	for (;;) {
		 
		float yaw = 0;
		float pitch = 0;
		float roll = 0;

		while (zwin->event(zwin, &ev)) {

			//printf(" ZEVENT %x %x %x %c     %x\n", ev.type, ev.a, ev.b, ev.a, ZKEY_CTRL);
			zprintevent(&ev);
			

			if (ZEVENTIS(ev.type ,ZEVENT_CHAR)) {

			//	printf(" ZEVENT CHAR %x %x %x %c     %x\n", ev.type, ev.a, ev.b, ev.a, ZKEY_CTRL);
				
				if (ev.a == 'm') 
					gfx_mouse_relative(zwin, mr ^= 1);

				if (ev.a == '+')
					speed *= 1.1;

				if (ev.a == '-')
					speed /= 1.1;

				if (ev.a == '.')
					frame++;

				if (ev.a == ',')
					frame--;

				if (ev.a == '0')
					frame = 0;
			}


			if (ZEVENTIS(ev.type,  ZEVENT_KEY|ZEVENT_DOWN|ZKEY_CTRL   ) &&(ev.a=='q') ) {
				printf("CLOSE\n");
				exit(1);
				break;
			}

			if (ZEVENTIS(ev.type, ZEVENT_DELTA)) {
			
				pitch -= ((zint32)ev.b ) / 200.0;
				yaw   -=  ((zint32)ev.a ) / 200.0;

			}
		}

		if (ev.type == ZEVENT_CLOSE)
		if (ev.type == ZEVENT_CLOSE)
			break;
	
		float forward = 0.0;
		float right = 0.0;
		float up = 0.0;
		

		if (gx_keystate('a')) right -= speed;
		if (gx_keystate('d')) right += speed;
		if (gx_keystate('w')) forward += speed;
		if (gx_keystate('s')) forward -= speed;
		if (gx_keystate('r')) up += speed;
		if (gx_keystate('f')) up -= speed;

		if (gx_keystate('q'))  roll -= .01;
		if (gx_keystate('e'))  roll += .01;

		if (gx_keystate(ZKEY_LEFT))  yaw += .01;
		if (gx_keystate(ZKEY_RIGHT))  yaw -= .01;


		if (gx_keystate(ZKEY_UP))  pitch += .01;
		if (gx_keystate(ZKEY_DOWN))  pitch -= .01;



		gfx_camera_motion_6dof(&cam, forward, right, up, yaw, pitch, roll);

		//printf(" Window size is %d %d\n", w, h);
		gfx_background_color(.3, .2, .1, 0);
		gfx_frame_clear(ZTRUE, ZTRUE);
		
		gfx_setup_3d(80,  (float)zwin->w / (float) zwin->h , .1, 1000);
	//	gfx_depth_buffer(ZFALSE, ZFALSE);
		
		gfx_camera_view(&cam);

		vec3set(lpcam, 0, 20, 0);
		gfx_trans_vec3(&lpcam); //transform point for light 
		gfx_style_set_property(st, GFX_FLOAT3, "light_position", 0, 0, &lpcam, 0);
		gfx_style(st);

		//gfx_translate3(0, 0, -5);
		//gfx_rotate_y(ang);
		
		
	//	gfx_vertex_buffer_draw(vb, GFX_TRIANGLE, 0, v, ZTRUE);
	

		

		
#if 1
		/*
		while (m) {

			gfx_vertex_buffer_draw(m->vb, GFX_TRIANGLE, 0, zarray_count(m->vb->index_buffer), ZTRUE);
			m = m->next_piece;
		}
		*/
#endif

#if 0
		vec3 a, b;

		vec3set(a, 0, 0, 0);		//root spot
		


		gfx_rotate_z(ang1);
		vec3set(b, 0, 1, 0);
		gfx_arrow(&a, &b);
		gfx_translate(&b);


		gfx_rotate_x(ang2);
		gfx_arrow(&a, &b);
		gfx_translate(&b);

		gfx_style(st);
		gfx_scale3(.5, .5, .5);
		gfx_depth_buffer(ZTRUE, ZTRUE);
		while (m) {

			gfx_vertex_buffer_draw(m->vb, GFX_TRIANGLE, 0, zarray_count(m->vb->index_buffer), ZTRUE);
			m = m->next_piece;
		}

		vec3set(b, 0, 10, 0);
		gfx_trans_vec3(&a);
		gfx_trans_vec3(&b);  //transform to camera coords


		gfx_identity();  //identity matrix (no camera motion either);
		
		gfx_arrow(&a, &b);

		ang1 += .001;
		ang2 += .003;

#endif
	//	gfx_translate3(2, 0, 0);
		//gfx_rotate_x(-90 * DEGREE);

		gfx_meshT* m;
		for (m = strawberry_mesh; m; m = m->next_piece) {
			gfx_vertex_buffer_draw(m->vb, GFX_TRIANGLE, 0, zarray_count(m->vb->index_buffer), ZTRUE);
		}

		//gfx_scale3(.2, .2, .2);  //TODO: add scaling to the BVH import
		gfx_transformT tmp;
		gfx_save_transform(&tmp);
		
		gfx_identity();//no camera transform
		
		

		debug_draw_skeleton(skel, 0);


		gfx_load_transform(&tmp);


		/*
		gfx_translate(&tmp.pos);
		gfx_rotate_3x3(&tmp.rot);
		*/

		gfx_depth_buffer(ZTRUE, ZTRUE);
		gfx_style(st);

		//gfx_translate(&test_trans.pos);
		//gfx_rotate_3x3(&test_trans.rot);

		
		//gfx_load_transform(&test_trans);
	//	while (m) {

		gfx_camera_view(&cam);

		//saved bone transformation
		gfx_translate(&test_trans.pos);
		gfx_rotate_3x3(&test_trans.rot);
		gfx_vertex_buffer_draw(mesh2->vb, GFX_TRIANGLE, 0, zarray_count(mesh2->vb->index_buffer), ZTRUE);

		gfx_camera_view(&cam);
		debug_draw_skeleton(skel, 0);
		//	m = m->next_piece;
		//}

		zwin->pixels(zwin, NULL);	//display the framebuffer
		ang += .01;
	}

	printf(" window close button was pressed\n");
	

}

#endif