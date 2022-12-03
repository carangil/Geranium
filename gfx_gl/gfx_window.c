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

#define checkGL()   checkGLfunc(__FILE__, __LINE__)

void checkGLfunc(char* file, int line) {
	int err;

	for (err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
		printf("OPENGL ERROR %x FROM %s:%d to %s:%d\n", err, last_file, last_line, file, line);
	}

	last_file = file;
	last_line = line;

}

/*glfw windowing and zevent interface*/
typedef struct gfx_windowS {
	zwindowT iface;	//the zevent window interface
	GLFWwindow* fwindow;
}gfx_windowT;

void errorHandler(int error, const char* message) {

	printf("GLFW ERROR:%d (0x%x) %s\n", error, error, message ? message : "null");

}

void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

	int zkey = 0;

	printf(" Key: %d %x %c\n", key, key, key);

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


	//translate the modifiers
	int keystate = ZEVENT_KEY;

	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {
		keystate |= ZEVENT_DOWN;
		
		//translate modifiers too

		if (mods & GLFW_MOD_SHIFT)
			keystate |= ZKEY_SHIFT;

		if (mods & GLFW_MOD_CONTROL)
			keystate |= ZKEY_CTRL;

		if (mods & GLFW_MOD_ALT)
			keystate |= ZKEY_ALT;

	}

	if (action == GLFW_RELEASE) {
		keystate |= ZEVENT_UP;
		//zevent doesn't care about if shift/alt/ctrl are held down while releasing a key
	}

	gfx_windowT* win= glfwGetWindowUserPointer(window);
	
	if (win == NULL) {
		printf(" not a zevent window! serious bug?\n");
	}

	zw_enqueue(&win->iface, keystate, zkey, 0, 0);

}

void charHandler(GLFWwindow* window, int character) {
	//glfw doesn't do modifiers with character callbacks
	//todo: we could probably read or track keystate
	//also might want to generate characters for backspace, enter, tab, etc, as they do have character codes (they keydown/repeat event should do that)
	gfx_windowT* win = (gfx_windowT*)glfwGetWindowUserPointer(window); ;
	zw_enqueue(&win->iface, ZEVENT_CHAR, character, 0, 0);
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
		return ZTRUE;
	}

	gfx_windowT* win = (gfx_windowT*)zw;

	if (glfwWindowShouldClose(win->fwindow)) {
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

zwindowT* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags) {

	gfx_windowT* win = ram_alloc(sizeof(gfx_windowT), NULL); //no destructor key

	if (!gfxi_inited)
		gfxi_init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 4);  //enable antialiasing buffers


	win->fwindow = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwSetWindowUserPointer(win->fwindow, win); //

	glfwSetKeyCallback(win->fwindow, keyHandler);
	glfwSetCharCallback(win->fwindow, charHandler);

	glfwMakeContextCurrent(win->fwindow);



	if (!gladLoadGLLoader(glfwGetProcAddress)) {
		printf("Can't init glad\n");
	}

	glEnable(GL_MULTISAMPLE); //enable antialiasing


	gfx_identity(); //clear the matrix

	win->iface.close = gfx_close;
	win->iface.pixels = gfx_pixels;
	win->iface.event = gfx_event;

	//get the window size and fix the viewport
	glfwGetWindowSize(win->fwindow, &win->iface.w, &win->iface.h);
	glViewport(0, 0, win->iface.w, win->iface.h);
	glClearColor(0, 0, 0, 1); //black window default

	return &(win->iface);
}

/*frame clear functions */

void gfx_clear_color(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void gfx_frame_clear(zbool color, zbool depth)
{
	glClear(
		(color ? GL_COLOR_BUFFER_BIT : 0)
		|
		(depth ? GL_DEPTH_BUFFER_BIT : 0)
	);
}



/* Some simple setup functions*/


void gfx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist)
{

	//projection matrix
	gfx_projection3d(fovy, aspect, neardist, fardist);

	//by default, a full range depth buffer
	glClearDepth(1.0); //when clearing depth buffer, set to infinity
	glDepthRange(0, 1);  //set range for full depth bufer
	glDepthFunc(GL_LEQUAL);  //draw things equally far or closer
	glDepthMask(GL_TRUE); //write to depth bufer
	glEnable(GL_DEPTH_TEST);  //enable depth testing


	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

}


void gfx_setup_2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom)
{

	//projection matrix (ortho 2d)
	gfx_projection2d(left, right, top, bottom);

	//depth buffer disabled
	glDepthMask(GL_FALSE);		//don't write to depth bufer
	glDisable(GL_DEPTH_TEST);	//don't enable depth testing

	gfx_identity();  //modelview matrix is reset to identity
}


/* Vertex Buffer Objects */

typedef struct gfxVertexAttributeS {
	int		type;	// 1,2,3, or 4 are for float values
	char*	name;
	float*	data;
} gfx_vertex_attributeT;

#define MAX_ATTRIBUTE 8

typedef struct gfx_VertexBufferS {
	float* combined_data;

	gfx_vertex_attributeT attributes[MAX_ATTRIBUTE];
	int num_attributes;

	zuint16 capacity;
	zuint16 count; //number of vertices to consider valid
	int vbo;
	int fcount; //number of float fields

	zuint16* index_buffer;	//zarray
	int index_vbo;
	
	//positions for fixed/simple pipeline functionality
	//only valid if fixed_position != -1
	int fixed_position;
	int fixed_color;
	int fixed_texcoord;
	int fixed_normal;
} gfx_vertex_bufferT;

zuint16* gfx_vertex_buffer_add_index(gfx_vertex_bufferT* vb, int num) {

	if (vb) {
		vb->index_buffer = zarray_alloc(zuint16, num);

		return vb->index_buffer; //the index, or null
	}
	return 0;
}

zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb,  zuint16 a, zuint16 b, zuint16 c) {

	zarray_add(vb->index_buffer, a);
	zarray_add(vb->index_buffer, b);
	zarray_add(vb->index_buffer, c);
	return zarray_count(vb->index_buffer);
}

gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount, char* spec) {

	char* s = spec;

	if (!s)
		return NULL;
	
	gfx_vertex_bufferT* vb = ram_alloc(sizeof(gfx_vertex_bufferT), NULL); //no destructor yet

	while (*s) {

		char* ne = strchr(s, ':');
		if (!ne)
			break;
		char* name = zstrndup(s, (ne - s));
		int size = atoi(ne + 1);

		ne = strchr(s, '|');

		if (!size)
			break;

		//add the attribute
		printf(" name is [%s] size is [%d]", name, size);

		vb->attributes[vb->num_attributes].name = name;
		vb->attributes[vb->num_attributes++].type = size; //simple numbers 1 to 4 are just floats.  TODO: non-float attributes?
		vb->fcount += size;

		if (!ne)
			break;

		s = ne + 1;
	}

	printf(" There are %d float components by %d vertices\n", vb->fcount, vcount);

	//allocate the buffer
	vb->combined_data = ram_alloc(sizeof(float) * vcount * vb->fcount, NULL);

	//set attribute data pointers
	vb->fixed_position = -1; //not valid
	vb->fixed_color = -1; //not valid
	vb->fixed_normal = -1; //not valid
	vb->fixed_texcoord = -1; //not valid
	int i;
	float* fp = vb->combined_data;
	for (i = 0; i < vb->num_attributes; i++) {
		vb->attributes[i].data = fp;
		printf(" Set ptr to %s  base+%d\n", vb->attributes[i].name, fp - vb->combined_data);
		fp += vb->attributes[i].type * vcount;

		//some vertex attributes are special (can be used with fixed function pipeline.  If ever target old computers, or if I want to implement some generic default behavior with a default shader)
		if (!strcmp(vb->attributes[i].name, "color")) {
			vb->fixed_color = i;
		}
		if (!strcmp(vb->attributes[i].name, "position")) {
			vb->fixed_position = i;
		}
		if (!strcmp(vb->attributes[i].name, "normal")) {
			vb->fixed_normal = i;
		}
		if (!strcmp(vb->attributes[i].name, "texcoord")) {
			vb->fixed_texcoord = i;
		}
	}

	vb->capacity = vcount;

	return vb;

}

void gfx_vertex_data(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d) {

	int  pos = vb->count * vb->attributes[attr].type;
	printf(" Setting to attribute %d at %d", attr, pos);

	switch (vb->attributes[attr].type) {
	case 4: vb->attributes[attr].data[pos + 3] = d;		//printf("@3");
		case 3: vb->attributes[attr].data[pos + 2] = c; // printf("@2");
		case 2: vb->attributes[attr].data[pos + 1] = b; // printf("@1");
		case 1: vb->attributes[attr].data[pos + 0] = a;	// printf("@0");
	}

	printf("\n");
}

zuint16 gfx_vertex_done(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d) {
	gfx_vertex_data(vb, attr, a, b, c, d);
	return vb->count++;
}




//track which vbo is active
//only set to nonzery when the vertex attribs are set to this vbo as well
int gxi_current_vbo = 0;
int gxi_current_index_vbo = 0;



//Call after modifying vertex buffer data
void gfx_vertex_buffer_update(gfx_vertex_bufferT* vb) {
	if (!vb)
		return;
	checkGL();

	if (!vb->vbo) {
		//create VBO

		glGenBuffers(1, &(vb->vbo));
		printf(" Generated VBO %d\n", vb->vbo);
	
	}

	if (vb->index_buffer && (zarray_count(vb->index_buffer) >0 )) {

		if (!vb->index_vbo) {
			glGenBuffers(1, &(vb->index_vbo));
			printf(" Generated index VBO %d\n", vb->vbo);
		}
		//send index data, if we have it
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, zarray_count(vb->index_buffer) * sizeof(vb->index_buffer[0]), vb->index_buffer, GL_DYNAMIC_DRAW);
		printf("send %d index values to vbo\n", zarray_count(vb->index_buffer));
		gxi_current_index_vbo = vb->index_vbo;
	}



	//switch to buffer's vbo and send data
	glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(zfloat32) * vb->capacity * vb->fcount, vb->combined_data, GL_DYNAMIC_DRAW);
	gxi_current_vbo = 0; //set to zero, so first draw sets up the vertex arrays
}


//using fixed function or not
zbool gxi_fixed_function = ZTRUE; //set to true 



#define GFX_POINT	1
#define GFX_LINE	2
#define GFX_TRIANGLE	3
zuint32 gl_prims[] = { 0, GL_POINTS, GL_LINES, GL_TRIANGLES };

int max_attrs_active;

void gfx_vertex_buffer_draw(gfx_vertex_bufferT* vb, int prim, int start, int end, zbool indexed) {

	zbool setup_arrays = ZFALSE;

	if (!vb)
		return;

	if (!vb->vbo)
		gfx_vertex_buffer_update(vb);	//update if a vbo was never made for this object


	if (gxi_current_vbo != vb->vbo) {
		glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
		gxi_current_vbo = vb->vbo;
		setup_arrays = ZTRUE;  //need to setup vertex arrays
	}

	if (gxi_current_index_vbo != vb->index_vbo) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->index_vbo);
		gxi_current_index_vbo = vb->index_vbo;
	}

	//TODO: check if the shader changed, if so, setup arrays
	gxi_refresh_matrix();
	
	if (setup_arrays) {


		if (gxi_fixed_function) {


			//set each attribute - fixed function
			if (vb->fixed_position != -1) {

				glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), (void*)((vb->attributes[vb->fixed_position].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_VERTEX_ARRAY);
			}
			
			//check for other FF attributes
			if (vb->fixed_color != -1) {
				glColorPointer(4, GL_FLOAT, 4 * sizeof(float), (void*)((vb->attributes[vb->fixed_color].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_COLOR_ARRAY);
			}
						

		}
		else {
			//setup arrays for shader use

		}

		

	}

	if (indexed)
		glDrawElements(gl_prims[prim], end - start, GL_UNSIGNED_SHORT, (void*) (sizeof(zuint16) * start));
	else
		glDrawArrays(gl_prims[prim], start, end - start);

	checkGL();
}

void gfx_gl_test() {
		
	zwindowT* zwin = gfx_mkwindow("internal test", 1024, 768, 0);

	gfx_windowT* gfx_window = (gfx_windowT*)zwin; //cast to our own specific type
	
	GLFWwindow* window = gfx_window->fwindow;

	gfx_vertex_bufferT* vb = gfx_vertex_buffer_mk(100, "color:4|position:3");
	
	gfx_vertex_buffer_add_index(vb, 60); //60 points
	
	gfx_vertex_data(vb, 0, 1.0, 0.0, 0.0, 0.5);
	int a = gfx_vertex_done(vb, 1, 5.0, 0.0, 0.0, 0);


	gfx_vertex_data(vb, 0, 0.0, 1.0, 0.0, 1.0);
	int b = gfx_vertex_done(vb, 1, 0.0, 5.0, 0.0, 0);


	gfx_vertex_data(vb, 0, 0.0, 1.0, 1.0, 1.0);
	int c = gfx_vertex_done(vb, 1, 5, 5, 0.0, 0);
	

	gfx_vertex_data(vb, 0, 0.0, 1.0, 1.0, 1.0);
	int d = gfx_vertex_done(vb, 1, 0.3, 0.3, 0.0, 0);

			
	gfx_index_triangle(vb, a, b, c);
	gfx_index_triangle(vb, a, b, d);
	

	zeventT ev;

	for (;;) {

		//glfwPollEvents(); //do the event loop

		while (zwin->event(zwin, &ev)) {
			printf(" ZEVENT %x %x %x %c\n", ev.type, ev.a, ev.b, ev.a);
		}

		if (ev.type == ZEVENT_CLOSE)
			break;

		int w, h;
		
		//printf(" Window size is %d %d\n", w, h);
		gfx_clear_color(.3, .2, .1, 0);
		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		gfx_setup_2d(0, (float)10, 0, (float)10);  //set to pixels
		

		gfx_vertex_buffer_draw(vb, GFX_TRIANGLE, 0,6, ZTRUE);
					
		zwin->pixels(zwin, NULL);	//display the framebuffer
				
	}

	printf(" window close button was pressed\n");
	

}