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
 	if (zw_queued(zw, ev)) {
		//printf(" RETURNING QUEUED EVENT\n");
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


void gfx_close(zwindowT* zw) {
	gfx_windowT* win = (gfx_windowT*)zw;

	glfwSetWindowShouldClose(win->fwindow, GLFW_TRUE);

}

zvecT* gxi_window_tempbuffers(gfx_windowT* gw) {
	return gw->tempvbufs;
}

zbool window_free(void* v) {
	gfx_windowT* gw = v;
	ram_free(gw->tempvbufs);
	return ZTRUE;
}

gfx_windowT* gxi_current_window = NULL;
//flags currently don't do anything
//creation of first window will init glfw

struct zwindow_s* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags) {

	memset(keystatus, 0, sizeof(keystatus));

	gfx_windowT* win = ram_alloc(sizeof(gfx_windowT),  window_free ); 

	win->tempvbufs = zvec_mk(NULL, 10);

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
	gxi_current_window = win;
		
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

	
	checkGL();

	return &(win->iface);
}


