// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>


#include "../ztypes.h"
#include "../vmath.h"
#include "glstuff.h"
#include "gx_sys.h"
#include <math.h>

//internal data
static zint32 _gx_window_width=0;
static zint32 _gx_window_height=0;
static zint32 _gx_auto_viewport_adjust=ztrue; /*true to automatically adjust viewport*/

//keyboard data

static zchar _keybuffer = 0;;
zbool _gx_keystate[256];  //up/down state of all possible chars

//mouse data
static zbool	_gx_mouse_capture = zfalse;
static zuint32	_gx_mouse_capture_last_x = 0;
static zuint32	_gx_mouse_capture_last_y = 0; 
static zuint32	_gx_last_mouse_x = 0;
static zuint32	_gx_last_mouse_y = 0; 

// GLUT callbacks

void _gx_callback_keyboard(char key, int x, int y)
{
	_keybuffer = key;  //store last key pressed
	

	_gx_keystate[  (unsigned char) key] = ztrue;  //store updated key state

}

void _gx_callback_keyboard_up(char key, int x, int y)
{
	_gx_keystate[  (unsigned char) key] = zfalse;  //indicate the key is not pressed
}





zbool gx_mousebuttons[] = {zfalse, zfalse, zfalse};

zbool gx_mouse_state(zuint32 button)
{
	if (button <= GX_MOUSE_MAX)
	{
		return gx_mousebuttons[button];

	}

	return zfalse;
}

static void _gx_callback_mouseclick(int button, int state, int x, int y)
{


	zbool val;

	if (state==GLUT_DOWN)
		val = ztrue;
	else 
		val = zfalse;
	
	switch (button)
	{

	case GLUT_LEFT_BUTTON:
		gx_mousebuttons[GX_MOUSE_LEFT] = val;
		break;

	case GLUT_MIDDLE_BUTTON:
		gx_mousebuttons[GX_MOUSE_MIDDLE] = val;
		break;

	case GLUT_RIGHT_BUTTON:
		gx_mousebuttons[GX_MOUSE_RIGHT] = val;
		break;


	}

#ifdef DOPRINTF 
	printf("mouseclick %d %d %d %d", button, state, x, y);
#endif

}




static void _gx_callback_mouseactive(int x, int y)
{

		_gx_last_mouse_x = x;
		_gx_last_mouse_y = y;
	
}


static void _gx_callback_mousepassive(int x, int y)
{
	//called when mouse is moved and no buttons are pressed
	//at this point, there is no reason to differentiate behavior (clicks already cause events)

	_gx_callback_mouseactive(x,y);
}


static void _gx_callback_reshape(int w, int h) //called when window is resized
{
#ifdef DOPRINTF 
	printf(" window resize %d %d\n", w, h);
#endif

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

static int _gx_window = 0;


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
	_gx_window = glutCreateWindow(window_title);

	//set callbacks
	glutReshapeFunc(_gx_callback_reshape);
	glutKeyboardFunc(_gx_callback_keyboard);
	glutKeyboardUpFunc(_gx_callback_keyboard_up);
	glutMouseFunc(_gx_callback_mouseclick);
	glutMotionFunc(_gx_callback_mouseactive);
	glutPassiveMotionFunc(_gx_callback_mousepassive);
	glutDisplayFunc(_gx_callback_disp);
	glPointSize(2.0);
	

	_gx_callback_reshape( width, height);  //reshape will use defaults

	gx_setup_2d(-1.0, -1.0, 1.0, 1.0) ;  //default coords are -1,-1 to 1,1

	_gx_line_init();  //initialize line drawing functions

	if (glewInit()!=GLEW_OK)
	{
#ifdef DOPRINTF 
		printf("Cannot initialize GLEW\n");
#endif
		return GX_ERROR;
	}

	//default blending mode is alpha, but is off by default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);


	

	return GX_OK;
}

//uninitialize code
void gx_disable()
{
	if (_gx_window)
		glutDestroyWindow(_gx_window);
	_gx_window = 0;
	
	_gx_line_disable();
}


//Window event handling:  must call this function periodically to handle events

void gx_window_event()
{
	glutMainLoopEvent();
}

/* Window input-ouput */


//returns the state of a single key.  true means pressed
zbool gx_key_state(zbyte a)
{
	return _gx_keystate[ (unsigned char) a];
}

zchar gx_getkey()   //returns last key pressed
{
	char c = _keybuffer;
	_keybuffer=0;
	return c;
}


void gx_mouse_capture(zbool cap)
{
  	if (cap)
	{
		//center mouse in window

		_gx_mouse_capture_last_x=_gx_window_width/2;
		_gx_mouse_capture_last_y=_gx_window_height/2;		

		_gx_last_mouse_x = 0;
		_gx_last_mouse_y = 0;

		_gx_mouse_capture = ztrue;

		glutWarpPointer( _gx_mouse_capture_last_x, _gx_mouse_capture_last_y);	
		glutSetCursor(GLUT_CURSOR_NONE); //hide mouse pointer
	}
	else
	{
		_gx_mouse_capture = zfalse;
		glutSetCursor(GLUT_CURSOR_INHERIT); //bring back mouse pointer
	}
}

void gx_mouse_pos(zint32* x, zint32* y, zbool* rel)
{
	
		if (rel)
			*rel = _gx_mouse_capture;

		if (!_gx_mouse_capture)
		{

			*x = _gx_last_mouse_x;
			*y = _gx_last_mouse_y;
		}
		else
		{
			
			*x = _gx_last_mouse_x - _gx_mouse_capture_last_x;
			*y = _gx_last_mouse_y - _gx_mouse_capture_last_y;

			if ( *x || *y)  //if the mouse moved, re-center it
			{

				_gx_mouse_capture_last_x=_gx_window_width/2;
				_gx_mouse_capture_last_y=_gx_window_height/2;		
				glutWarpPointer( _gx_mouse_capture_last_x, _gx_mouse_capture_last_y);

			}
		}
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

zfloat32 gx_frame_get_dimensions(zuint32* width, zuint32* height)
{
	if (height)
		*height = _gx_window_height;

	if (width)
		*width = _gx_window_width;

	return ((zfloat32) _gx_window_width) / ((zfloat32)_gx_window_height);
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

	glDisable(GL_CULL_FACE); //we want face culling (for now)

	//glEnable(GL_CULL_FACE); //we want face culling (for now)
	//glCullFace(GL_BACK);
}

//depth buffer
void gx_zbuffer(zbool en)
{
	if (en)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	
}


//simple matrix based commands




static zint32 _gx_matrix_depth = 0;  //nothing pushes

static _gx_restore_camera_matrix()
{
	if (_gx_matrix_depth ==1)
	{
		glPopMatrix();
		_gx_matrix_depth = 0;
	}
	else
	{
		printf(" Invalid matrix depth state! %d\n", _gx_matrix_depth);
	}
}


static _gx_save_camera_matrix()
{
	if (_gx_matrix_depth ==0)
	{
		glPushMatrix();
		_gx_matrix_depth = 1;
	}
	else
	{
		printf(" Invalid matrix depth state! %d\n", _gx_matrix_depth);
	}
}

static _gx_reset_matrix()
{

	if (_gx_matrix_depth ==1)
	{
		glPopMatrix();
		_gx_matrix_depth = 0;
	}

	if (_gx_matrix_depth != 0)
	{
		printf(" Invalid matrix depth state! %d\n", _gx_matrix_depth);
	}

	glLoadIdentity();
}


void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis)
{

 	_gx_reset_matrix();


	if (xaxis && yaxis && zaxis)
	{
		zfloat32 matr[]={	
			xaxis->vec3x, yaxis->vec3x, -zaxis->vec3x,0,
			xaxis->vec3y, yaxis->vec3y, -zaxis->vec3y,0,
			xaxis->vec3z, yaxis->vec3z, -zaxis->vec3z,0,
			0,0,0,1};

			glLoadMatrixf((float*)&matr);			
	}

	if (position)
		glTranslatef( -position->vec3x, -position->vec3y, -position->vec3z);

	//now that we have a fresh camera matrix, lets save it
	_gx_save_camera_matrix();
	
}

void gx_home()
{  
	_gx_restore_camera_matrix();
	_gx_save_camera_matrix();
}

void gx_camera_home()
{	//reset transform AND camera
	_gx_reset_matrix();
}




void gx_move3d(vec3* amount)
{
	if (amount)
		glTranslatef(amount->named.x, amount->named.y, amount->named.z);
}

//specify 3x3 matrix
void gx_rotate_3x3(vec3* xaxis, vec3* yaxis, vec3* zaxis )
{
	if (xaxis && yaxis && zaxis)
	{
		zfloat32 matr[]={
					     xaxis->vec3x, xaxis->vec3y, xaxis->vec3z,0,
					     yaxis->vec3x, yaxis->vec3y, yaxis->vec3z,0,
					     zaxis->vec3x, zaxis->vec3y, zaxis->vec3z,0,
					     0,0,0,1};

		glMultMatrixf((float*)&matr);
	}
}

void gx_scale3d(vec3* scale)
{
	if (!scale)
		return;

	glScalef( scale->named.x, scale->named.y, scale->named.z);
}

void gx_scale(float scale)
{
	glScalef( scale, scale, scale);
}

