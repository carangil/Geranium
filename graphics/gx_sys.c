// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include <windows.h>


#include "../ztypes.h"
#include "../vmath.h"

#include "glstuff.h"

#include "gx_sys.h"
#include <math.h>


static int _gx_doshutdown = GX_LOOP_NOTHING;

static zint32 _gx_window_width=0;
static zint32 _gx_window_height=0;
static zint32 _gx_auto_viewport_adjust=ztrue; /*true to automatically adjust viewport*/

// Required glut callbacks

#define KEYBSIZE 10
static zchar _keybuffer[KEYBSIZE];
static zuint32 _kbpos=0;

zbool _gx_keystate[256];

void _gx_callback_keyboard(char key, int x, int y)
{

	if (_kbpos < KEYBSIZE)
	{
		_keybuffer[_kbpos++]= key;
	}

	_gx_keystate[  (unsigned char) key] = ztrue;  //indicate the key is pressed

}

void _gx_callback_keyboard_up(char key, int x, int y)
{
	_gx_keystate[  (unsigned char) key] = zfalse;  //indicate the key is not pressed
}

//returns the state of a single key
zbool gx_key_state(zbyte a)
{
	return _gx_keystate[a];
}

zchar gx_getkey()  //goes to 
{
	if (_kbpos)
	{
		return _keybuffer[--_kbpos];
	}
	return 0;  //no key!
}


static void _gx_callback_mouseclick(int button, int state, int x, int y)
{
	printf("mouseclick %d %d %d %d", button, state, x, y);
}


static zbool	_gx_mouse_capture = zfalse;
static zuint32	_gx_mouse_capture_last_x = 0;
static zuint32	_gx_mouse_capture_last_y = 0; 

static zuint32	_gx_last_mouse_x = 0;
static zuint32	_gx_last_mouse_y = 0; 



void gx_mouse_capture(zbool cap)
{
	if (cap)
	{
		_gx_mouse_capture_last_x=_gx_window_width/2;
		_gx_mouse_capture_last_y=_gx_window_height/2;
		
		//windows function!
		SetCursorPos(_gx_mouse_capture_last_x,_gx_mouse_capture_last_y);
		_gx_mouse_capture = ztrue;
	}
	else
	{
		_gx_mouse_capture = zfalse;
	}
}

void gx_mouse_pos(zint32* x, zint32* y)
{

	if (!_gx_mouse_capture)
	{
		*x = _gx_last_mouse_x;
		*y = _gx_last_mouse_y;

	}
	else
	{
		POINT point;
		GetCursorPos(&point);
		
		
		*x = point.x - _gx_mouse_capture_last_x;
		*y = point.y - _gx_mouse_capture_last_y;

		_gx_mouse_capture_last_x=_gx_window_width/2;
		_gx_mouse_capture_last_y=_gx_window_height/2;
		
	
	
		if ( *x || *y)
			SetCursorPos(_gx_mouse_capture_last_x,_gx_mouse_capture_last_y);
		

	}
}

static void _gx_callback_mousepassive(int x, int y)
{

	
	_gx_last_mouse_x = x;
	_gx_last_mouse_y = y;


}



static void _gx_callback_mouseactive(int x, int y)
{

	
	printf("mousemove active %d %d\n", x, y);
	
}

static void _gx_callback_reshape(int w, int h) //called when window is resized
{
	printf(" window resize %d %d\n", w, h);

	/*Remember window size*/
	_gx_window_width=w;
	_gx_window_height=h;

	if (_gx_auto_viewport_adjust)
	{
		glViewport(0,0,w,h);
	}

}

static void _gx_callback_disp()
{
	/*Don't do anything here, its just required to keep GLUT happy*/
}


//Initialization 

int gx_init(zint32 width, zint32 height, zchar* window_title )
{
	/* Fake argc/argv fool GLUT into getting different parameters */
	int fakeargc=0;
	char *fakeargv0;
	char ** fakeargv= &fakeargv0;
	
	memset(_gx_keystate, 0, sizeof(_gx_keystate));		

	glutInit(&fakeargc, fakeargv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL );
	glutCreateWindow(window_title);

	//set callbacks
	glutReshapeFunc(_gx_callback_reshape);
	glutKeyboardFunc(_gx_callback_keyboard);
	glutKeyboardUpFunc(_gx_callback_keyboard_up);
	glutMouseFunc(_gx_callback_mouseclick);
	glutMotionFunc(_gx_callback_mouseactive);
	glutPassiveMotionFunc(_gx_callback_mousepassive);
	glutDisplayFunc(_gx_callback_disp);
	
	_gx_callback_reshape( width, height);  //reshape will use defaults

	gx_setup_2d(-1.0, -1.0, 1.0, 1.0) ;  //default coords are -1,-1 to 1,1

	if (glewInit()!=GLEW_OK)
	{
		printf("Cannot initialize GLEW\n");
		return GX_ERROR;
	}

	//default blending mode is alpha, but is off by default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	return GX_OK;
}

//Window event handling:  must call this function periodically to handle events

zint32 gx_window_event()
{
	glutMainLoopEvent();
	return _gx_doshutdown;
}

/* Simple framebuffer control */

void gx_clear_color(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);	
}

void gx_frame_clear(zbool color, zbool depth)
{
	glClear(
			(color ? GL_COLOR_BUFFER_BIT : 0)
			|
			(depth ? GL_DEPTH_BUFFER_BIT : 0)
		);
}

void gx_frame_show()
{
	glutSwapBuffers();
}

/* Two dimensional coord system */
void gx_setup_2d(float left,  float top, float right, float bottom)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, -1.0,1.0);
	glMatrixMode(GL_MODELVIEW);

	//makes most sense to disable depth:
	glDepthMask(GL_FALSE);  //don't write to depth bufer
	glDisable(GL_DEPTH_TEST); //don't test depth buffer when drawing

	glDisable(GL_CULL_FACE);

}


/* Three dim coord system */

void gx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (fovy,aspect,neardist,fardist);
	glMatrixMode (GL_MODELVIEW);

	//probably want depth buffer:
	glClearDepth(1.0); //when clearing depth buffer, set to infinity
	glDepthRange(0,1);  //set range for full depth bufer
	glDepthFunc(GL_LEQUAL);  //draw things equally far or closer
	glDepthMask(GL_TRUE); //write to depth bufer
	glEnable(GL_DEPTH_TEST);  //enable depth testing

	//glEnable(GL_CULL_FACE); //we want face culling (for now)
	//glCullFace(GL_BACK);
}

zfloat32 gx_get_image_dimensions(zuint32* width, zuint32* height)
{
	if (height)
		*height = _gx_window_height;

	if (width)
		*width = _gx_window_width;

	return ((zfloat32) _gx_window_width) / ((zfloat32)_gx_window_height);
}

