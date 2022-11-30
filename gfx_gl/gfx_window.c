#define GFXINTERNAL
#include "ztypes.h"
#include "zmem.h"
#include "zwindow.h"
#include "gfx_gl.h"
#include "zvector.h"
#include "zstring.h"
#include "string.h"
#include <stdio.h>

typedef struct gfx_windowS {
	zwindowT iface;	//the zevent window interface

	GLFWwindow* fwindow;

}gfxWindowT;

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

	gfxWindowT* win= glfwGetWindowUserPointer(window);
	
	if (win == NULL) {
		printf(" not a zevent window! serious bug?\n");
	}

	zw_enqueue(&win->iface, keystate, zkey, 0, 0);

}

void charHandler(GLFWwindow* window, int character) {
	//glfw doesn't do modifiers with character callbacks
	//todo: we could probably read or track keystate
	gfxWindowT* win = (gfxWindowT*)glfwGetWindowUserPointer(window); ;
	zw_enqueue(&win->iface, ZEVENT_CHAR, character, 0, 0);
}



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

	gfxWindowT* win = (gfxWindowT*)zw;

	if (glfwWindowShouldClose(win->fwindow)) {
		//glfw user is trying to close window
		ev->type = ZEVENT_CLOSE;
		return ZFALSE;
	}
		

	ev->type = ZEVENT_NONE;
	return ZFALSE;
}

void gfx_pixels(zwindowT * zw, void* px) {
	printf(" 'pixels' interface not supported in opengl\n");
	//TODO: Just render the image to a texture, and be done with it
	//TODO: the old pixeltoaster interface did not support resizing windows, BUT this does.  Not sure what to do... maybe add w/h/ fields here, not support pixels interface, or 
}

void gfx_close(zwindowT * zw) {
	printf(" 'close' interface not supported in opengl.  click the 'x' manually\n");
}



//flags currently don't do anything
//creation of first window will init glfw

zwindowT* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags) {

	gfxWindowT* win = ram_alloc(sizeof(gfxWindowT), NULL); //no destructor key

	if (!gfxi_inited)
		gfxi_init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	win->fwindow = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwSetWindowUserPointer(win->fwindow, win); //

	glfwSetKeyCallback(win->fwindow, keyHandler);
	glfwSetCharCallback(win->fwindow, charHandler);

	glfwMakeContextCurrent(win->fwindow);


	if (!gladLoadGLLoader(glfwGetProcAddress)) {
		printf("Can't init glad\n");
	}




	win->iface.close = gfx_close;
	win->iface.pixels = gfx_pixels;
	win->iface.event = gfx_event;
	
	return &(win->iface);
}


/* Vertex buffers */

typedef struct gfxVertexAttributeS {
	int		type;	// 1,2,3, or 4 are for float values
	char*	name;
	float*	data;
} gfxVertexAttributeT;

#define MAX_ATTRIBUTE 8

typedef struct gfxVertexBufferS {
	float* combined_data;


	gfxVertexAttributeT attributes[MAX_ATTRIBUTE];
	int numAttributes;

	int capacity;
	int count; //number of vertices to consider valid
	int vbo;
	int fcount; //number of float fields

	//positions for fixed/simple pipeline functionality
	//only valid if fixed_position != -1
	int fixed_position;
	int fixed_color;
	int fixed_texcoord;
	int fixed_normal;
} gfxVertexBufferT;


gfxVertexBufferT* gfxVertexBuffer(int vcount, char* spec) {

	char* s = spec;

	if (!s)
		return NULL;

	

	gfxVertexBufferT* vb = ram_alloc(sizeof(gfxVertexBufferT), NULL); //no destructor yet

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

		vb->attributes[vb->numAttributes].name = name;
		vb->attributes[vb->numAttributes++].type = size; //simple numbers 1 to 4 are just floats.  TODO: non-float attributes?
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
	int i;
	float* fp = vb->combined_data;
	for (i = 0; i < vb->numAttributes; i++) {
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

void gfxVertexData(gfxVertexBufferT* vb, int attr, float a, float b, float c, float d) {

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

void gfxVertexDone(gfxVertexBufferT* vb, int attr, float a, float b, float c, float d) {
	gfxVertexData(vb, attr, a, b, c, d);
	vb->count++;
}

char* last_file;
int last_line;
void gx_error(char* file, int line) {
	int err;
	int ec = 0;
	for (err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
		ec++;

	}

	if (ec) {
		printf(" %d OPENGL ERRORS FROM %s:%d to %s:%d\n", ec, last_file, last_line, file, line);
		while (1);

	}

	last_file = file;
	last_line = line;

}

void gfxDrawBuffer(gfxVertexBufferT* vb) {

	if (!vb->vbo) {
		//create VBO
	
		glGenBuffers(1, &(vb->vbo));
		printf(" Generated VBO %d\n", vb->vbo);
		
	
		glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(zfloat32) * vb->capacity * vb->fcount, vb->combined_data, GL_DYNAMIC_DRAW);
	
	}

	//switch to the right vbo
	glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
	



	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), (void*)((vb->attributes[vb->fixed_position].data  - vb->combined_data) * sizeof(zfloat32)));
	glEnableClientState(GL_VERTEX_ARRAY);
	
	
	glColorPointer(4, GL_FLOAT, 4 * sizeof(float), (void*)((vb->attributes[vb->fixed_color].data - vb->combined_data) * sizeof(zfloat32)));
	glEnableClientState(GL_COLOR_ARRAY);
	

	glDrawArrays(GL_TRIANGLES, 0, 3);



	gx_error(__FILE__, __LINE__);
}

void gfx_gl_test() {
		
	zwindowT* zwin = gfx_mkwindow("internal test", 1024, 768, 0);

	gfxWindowT* gfx_window = (gfxWindowT*)zwin; //cast to our own specific type
	
	GLFWwindow* window = gfx_window->fwindow;

	gfxVertexBufferT* vb = gfxVertexBuffer(100, "color:4|position:3");
	
	
														gfxVertexData(vb, 0, 1.0, 0.0, 0.0, 0.5);
	gfxVertexDone(vb, 1, 1.0, 0.0, 0.0, 0);


														gfxVertexData(vb, 0, 0.0, 1.0, 0.0, 1.0);
	gfxVertexDone(vb, 1, 0.0, 1.0, 0.0, 0);


														gfxVertexData(vb, 0, 0.0, 1.0, 1.0, 1.0);
	gfxVertexDone(vb, 1, 1.0, 1.0, 0.0, 0);
	
			

	

	zeventT ev;

	for (;;) {

		//glfwPollEvents(); //do the event loop

		while (zwin->event(zwin, &ev)) {
			printf(" ZEVENT %x %x %x %c\n", ev.type, ev.a, ev.b, ev.a);
		}

		if (ev.type == ZEVENT_CLOSE)
			break;

		int w, h;
		glfwGetWindowSize(window, &w, &h);
		//printf(" Window size is %d %d\n", w, h);
		glViewport(0, 0, w, h);
		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		gfxDrawBuffer(vb);

		glfwSwapBuffers(window);
	}

	printf(" window close button was pressed\n");
	getc(stdin);

}