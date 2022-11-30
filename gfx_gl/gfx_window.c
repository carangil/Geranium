#define GFXINTERNAL
#include "ztypes.h"
#include "zmem.h"
#include "zwindow.h"
#include "gfx_gl.h"
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	win->fwindow = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwSetWindowUserPointer(win->fwindow, win); //

	glfwSetKeyCallback(win->fwindow, keyHandler);
	glfwSetCharCallback(win->fwindow, charHandler);

	glfwMakeContextCurrent(win->fwindow);

	win->iface.close = gfx_close;
	win->iface.pixels = gfx_pixels;
	win->iface.event = gfx_event;
	
	return &(win->iface);
}




void gfx_gl_test() {
		
	zwindowT* zwin = gfx_mkwindow("internal test", 1024, 768, 0);

	gfxWindowT* gfx_window = (gfxWindowT*)zwin; //cast to our own specific type
	
	GLFWwindow* window = gfx_window->fwindow;
	
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

		glfwSwapBuffers(window);
	}

	printf(" window close button was pressed\n");
	getc(stdin);

}