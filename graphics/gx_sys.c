// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#include <stdio.h>
#include <windows.h>

//#include "gl\glext.h"
#include "../ztypes.h"

#include "gl/glew.h"
#include "gl/wglew.h"

//#include <gl/GL.h>
#include "gl/freeglut.h"
#include "gx_sys.h"



static int _gx_doshutdown = GX_LOOP_NOTHING;


static zint32 _gx_window_width=0;
static zint32 _gx_window_height=0;
static zint32 _gx_auto_viewport_adjust=ztrue; /*true to automatically adjust viewport*/

// Required glut callbacks
void _gx_callback_keyboard(char key, int x, int y)
{
	printf(" %c at %d %d\n", key,x,y);
	if (key=='Q') 
		_gx_doshutdown=GX_LOOP_EXIT;
}

void _gx_callback_mouseclick(int button, int state, int x, int y)
{
	printf("mouseclick %d %d %d %d", button, state, x, y);
}

void _gx_callback_mousepassive(int x, int y)
{
	printf("mousemove passive %d %d\n", x, y);
}

void _gx_callback_mouseactive(int x, int y)
{
	printf("mousemove active %d %d\n", x, y);
}

void _gx_callback_reshape(int w, int h) //called when window is resized
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

void _gx_callback_disp()
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
	
	glutInit(&fakeargc, fakeargv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL );
	glutCreateWindow(window_title);

	//set callbacks
	glutReshapeFunc(_gx_callback_reshape);
	glutKeyboardFunc(_gx_callback_keyboard);
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
}


zfloat32 gx_get_image_dimensions(zuint32* width, zuint32* height)
{
	if (height)
		*height = _gx_window_height;

	if (width)
		*width = _gx_window_width;

	return ((zfloat32) _gx_window_width) / ((zfloat32)_gx_window_height);
}