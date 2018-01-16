// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>

#include <string.h>
#include "../ztypes.h"
#include "../vmath/zmath.h"
#include "glheaders.h"
#include "gx_sys.h"
//#include "gx_line.h"
//#include "gx_trans.h"
#include <math.h>

//gl allocation count
int _gx_gl_textures_gen = 0;
int _gx_gl_vbos_gen = 0;
int _gx_gl_textures_del = 0;
int _gx_gl_vbos_del = 0;


//internal data
static zint32 _gx_window_width=0;
static zint32 _gx_window_height=0;
static zint32 _gx_auto_viewport_adjust=ZTRUE; /*true to automatically adjust viewport*/

static zfloat32 _gx_2d_top =0;
static zfloat32 _gx_2d_bottom =0;
static zfloat32 _gx_2d_left = 0;
static zfloat32 _gx_2d_right = 0;

//keyboard data

static zchar _keybuffer = 0;;
zbool _gx_keystate[256];  //up/down state of all possible chars

//mouse data
static zbool	_gx_mouse_capture = ZFALSE;
static zuint32	_gx_mouse_capture_last_x = 0;
static zuint32	_gx_mouse_capture_last_y = 0; 
static zuint32	_gx_last_mouse_x = 0;
static zuint32	_gx_last_mouse_y = 0; 
static zbool	_gx_mouse_present_state = ZTRUE;
static zbool	_gx_mouse_first_capture = ZFALSE;

// GLUT callbacks

void _gx_callback_keyboard(unsigned char key, int x, int y)
{
	_keybuffer = key;  //store last key pressed
	

	_gx_keystate[   key] = ZTRUE;  //store updated key state

}

void _gx_callback_keyboard_up(unsigned char key, int x, int y)
{
	_gx_keystate[   key] = ZFALSE;  //indicate the key is not pressed
}


int last_line=0;
char* last_file = 0;


void gx_error(char* file, int line){
	int err;
	int ec=0;
	for (err = glGetError(); err!= GL_NO_ERROR; err = glGetError()) {
		ec++;
		
	}
	
	if (ec) {
		printf(" %d OPENGL ERRORS FROM %s:%d to %s:%d\n", ec,last_file, last_line, file, line  );
	}
	
	last_file = file;
	last_line = line;
	
}


zbool gx_mousebuttons[] = {ZFALSE, ZFALSE, ZFALSE};

zbool gx_mouse_state(zuint32 button)
{
	if (button <=    (sizeof(gx_mousebuttons) / sizeof(gx_mousebuttons[0]))   )
	{
		return gx_mousebuttons[button];

	}

	return ZFALSE;
}

static void _gx_callback_mouseclick(int button, int state, int x, int y)
{


	zbool val;

	if (state==GLUT_DOWN)
		val = ZTRUE;
	else 
		val = ZFALSE;
	
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
	//printf("mouseclick %d %d %d %d", button, state, x, y);
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

static void _gx_callback_mouse_entry(int state)
{
//	printf( "Mouse entry %d\n", state);
	if (state == GLUT_LEFT)
		_gx_mouse_present_state = ZFALSE;
	else if (state == GLUT_ENTERED)
		_gx_mouse_present_state = ZTRUE;

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

static void _gx_callback_disp(void)
{
	/*Don't do anything here, its just required to keep GLUT happy*/
}


//Initialization 

static int _gx_window = 0;

int _gx_no_vbos = 0; //set to true when falling back to vertex arrays

int gx_init(zint32 width, zint32 height, zchar* window_title )
{
	/* Fake argc/argv fool GLUT into getting different parameters */
	int fakeargc=0;
	char *fakeargv0;
	char ** fakeargv= &fakeargv0;
	int ver[2];
	
	memset(_gx_keystate, 0, sizeof(_gx_keystate));		

	glutInit(&fakeargc, fakeargv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

//	glutInitContextVersion(3,1);	
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL );
	_gx_window = glutCreateWindow(window_title);


	
	glGetIntegerv(GL_MAJOR_VERSION, &ver[0]);
	glGetIntegerv(GL_MINOR_VERSION, &ver[1]);
	printf(" OPENGL VERSION:%d/%d\n", ver[0], ver[1]);

	//set callbacks
	glutReshapeFunc(_gx_callback_reshape);
	glutKeyboardFunc(_gx_callback_keyboard);
	glutKeyboardUpFunc(_gx_callback_keyboard_up);
	glutMouseFunc(_gx_callback_mouseclick);
	glutMotionFunc(_gx_callback_mouseactive);
	glutPassiveMotionFunc(_gx_callback_mousepassive);
	glutDisplayFunc(_gx_callback_disp);
	glutEntryFunc(_gx_callback_mouse_entry);
	glPointSize(1.0);
	

	_gx_callback_reshape( width, height);  //reshape will use defaults

	gx_setup_2d(-1.0, -1.0, 1.0, 1.0) ;  //default coords are -1,-1 to 1,1
	//gx_setup_2d( 0.0f, 0.0f, width-1.0f, height-1.0f);

//	_gx_line_init();  //initialize line drawing functions

	if (glewInit()!=GLEW_OK)
	{
#ifdef DOPRINTF 
		printf("Cannot initialize GLEW\n");
#endif
		return GX_ERROR;
	}

	if (! glGenBuffers)
	{
		//ARB fix
		glGenBuffers = glGenBuffersARB;
		glDeleteBuffers = glDeleteBuffersARB;
		glBindBuffer = glBindBufferARB;
		glBufferData = glBufferDataARB;
		printf("Warning:  Using ARB VBOs instead of core\n");
	}

//#define TESTOLDFALLBACK

#ifdef TESTOLDFALLBACK
		glGenBuffers = NULL;
		glDeleteBuffers = NULL;
		glBindBuffer = NULL;
		glBufferData = NULL;

#endif
	
	
	if (!glGenBuffers)
	{
		_gx_no_vbos = ZTRUE;
		printf("Cannot initialize VBO functions, using vertex arrays\n");
		//return GX_ERROR;
	}


	//default blending mode is alpha, but is off by default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);


	
	//glutFullScreen();

	return GX_OK;
}


//uninitialize code
void gx_disable()
{
	
//	_gx_line_disable();

	if (_gx_window)
		glutDestroyWindow(_gx_window);
	_gx_window = 0;
	
	printf(" %d textures allocated\n", _gx_gl_textures_gen);
	printf(" %d textures deleted\n", _gx_gl_textures_del);
	printf(" %d vbos allocated\n", _gx_gl_vbos_gen);
	printf(" %d vbos deleted\n", _gx_gl_vbos_del);



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

		_gx_mouse_capture = ZTRUE;

		_gx_mouse_first_capture = ZTRUE; //we want to ignore the 1st mouse mouse event

		glutWarpPointer( _gx_mouse_capture_last_x, _gx_mouse_capture_last_y);	
		glutSetCursor(GLUT_CURSOR_NONE); //hide mouse pointer
#ifdef _WIN32
		ShowCursor(0);//windows call
#endif

	}
	else
	{
		_gx_mouse_capture = ZFALSE;
		glutSetCursor(GLUT_CURSOR_INHERIT); //bring back mouse pointer

#ifdef _WIN32
		ShowCursor(1); //windows call
#endif
	}
}

void gx_mouse_pos(zint32* x, zint32* y, zbool* rel)
{

		int dx;
		int dy;
	
		if (rel)
			*rel = _gx_mouse_capture;

		if (!_gx_mouse_capture)
		{

			*x = _gx_last_mouse_x;
			*y = _gx_last_mouse_y;
		}
		else
		{
			
			dx = _gx_last_mouse_x - _gx_mouse_capture_last_x;
			dy = _gx_last_mouse_y - _gx_mouse_capture_last_y;

			if ( dx || dy)  //if the mouse moved, re-center it
			{

				_gx_mouse_capture_last_x=_gx_window_width/2;
				_gx_mouse_capture_last_y=_gx_window_height/2;		
				glutWarpPointer( _gx_mouse_capture_last_x, _gx_mouse_capture_last_y);

			}
			if (_gx_mouse_first_capture)
			{
				dx=0;
				dy=0;
				_gx_mouse_first_capture = 0;
				
			}
			*x = dx;
			*y = dy;
		}
}

//returns mouse pointer scaled in current 2d dimensions
void gx_mouse_posf(zfloat32* fx, zfloat32* fy, zbool* rel)
{
	int x;
	int y;
	
	//get integer coordinates
	gx_mouse_pos(&x, &y, rel);

	//translate to 

	if (_gx_window_width > 0)
	{
		*fx =  (x * (_gx_2d_right - _gx_2d_left) ) / _gx_window_width;
	}

	if (_gx_window_height > 0)
	{
		*fy =  (y * (_gx_2d_bottom - _gx_2d_top) ) / _gx_window_height;
	}
	
}

void gx_hide_mouse()
{
		glutSetCursor(GLUT_CURSOR_NONE); 
}

zbool gx_mouse_present()
{
	return _gx_mouse_present_state;
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
	gx_error("gx_frame_show",0);
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

//	_gx_reset_matrix();// reset camera matrix

	//makes most sense to disable depth:
	//glDepthMask(GL_FALSE);  //don't write to depth bufer

	glDisable(GL_DEPTH_TEST); //don't test depth buffer when drawing


	glDisable(GL_CULL_FACE);

	_gx_2d_top = top;
	_gx_2d_bottom = bottom;
	_gx_2d_left = left; 
	_gx_2d_right = right;


}

void gx_setup_2d_pixels(int* width, int *height)
{
	//give dimensions to client application
	gx_frame_get_dimensions(width, height);

	gx_setup_2d(-.375f, _gx_window_height-.375f, _gx_window_width-.375f, -.375f);
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
//	glDepthMask(GL_TRUE); //write to depth bufer
	glEnable(GL_DEPTH_TEST);  //enable depth testing

	//glDisable(GL_CULL_FACE); //we want face culling (for now)

	glEnable(GL_CULL_FACE); //we want face culling (for now)
	glCullFace(GL_BACK);


	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//	glPolygonMode( GL_BACK, GL_LINE );

}

//depth buffer
void gx_zbuffer(zbool en)
{
	if (en)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	
}











#if 0


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

//specify 3x3 camera matrix (used when rotating an object whose orientation 
//is determined by a camera-style matrix (identity = <1, 1, -1> )
void gx_rotate_3x3_cam(vec3* xaxis, vec3* yaxis, vec3* zaxis )
{
	if (xaxis && yaxis && zaxis)
	{
		zfloat32 matr[]={
					     xaxis->vec3x, xaxis->vec3y, xaxis->vec3z,0,
					     yaxis->vec3x, yaxis->vec3y, yaxis->vec3z,0,
					     -zaxis->vec3x, -zaxis->vec3y, -zaxis->vec3z,0,
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
#endif
