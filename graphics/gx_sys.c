// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
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
int gxi_gl_textures_gen = 0;
int gxi_gl_vbos_gen = 0;
int gxi_gl_textures_del = 0;
int gxi_gl_vbos_del = 0;


//internal data
static zint32 gxi_window_width=0;
static zint32 gxi_window_height=0;
static zint32 gxi_auto_viewport_adjust=ZTRUE; /*true to automatically adjust viewport*/

static zfloat32 gxi_2d_top =0;
static zfloat32 gxi_2d_bottom =0;
static zfloat32 gxi_2d_left = 0;
static zfloat32 gxi_2d_right = 0;

//keyboard data

static zchar _keybuffer = 0;;
zbool gxi_keystate[256];  //up/down state of all possible chars

//mouse data
static zbool	gxi_mouse_capture = ZFALSE;
static zuint32	gxi_mouse_capture_last_x = 0;
static zuint32	gxi_mouse_capture_last_y = 0; 
static zuint32	gxi_last_mouse_x = 0;
static zuint32	gxi_last_mouse_y = 0; 
static zbool	gxi_mouse_present_state = ZTRUE;
static zbool	gxi_mouse_first_capture = ZFALSE;

// GLUT callbacks

void gxi_callback_keyboard(unsigned char key, int x, int y)
{
	_keybuffer = key;  //store last key pressed
	

	gxi_keystate[   key] = ZTRUE;  //store updated key state

}

void gxi_callback_keyboard_up(unsigned char key, int x, int y)
{
	gxi_keystate[   key] = ZFALSE;  //indicate the key is not pressed
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
		while (1);
		
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

static void gxi_callback_mouseclick(int button, int state, int x, int y)
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




static void gxi_callback_mouseactive(int x, int y)
{

		gxi_last_mouse_x = x;
		gxi_last_mouse_y = y;
	
}


static void gxi_callback_mousepassive(int x, int y)
{
	//called when mouse is moved and no buttons are pressed
	//at this point, there is no reason to differentiate behavior (clicks already cause events)

	gxi_callback_mouseactive(x,y);
}

static void gxi_callback_mouse_entry(int state)
{
//	printf( "Mouse entry %d\n", state);
	if (state == GLUT_LEFT)
		gxi_mouse_present_state = ZFALSE;
	else if (state == GLUT_ENTERED)
		gxi_mouse_present_state = ZTRUE;

}

static void gxi_callback_reshape(int w, int h) //called when window is resized
{
#ifdef DOPRINTF 
	printf(" window resize %d %d\n", w, h);
#endif

	/*Remember window size*/
	gxi_window_width=w;
	gxi_window_height=h;

	if (gxi_auto_viewport_adjust)
	{
		glViewport(0,0,w,h);
	}

}

static void gxi_callback_disp(void)
{
	/*Don't do anything here, its just required to keep GLUT happy*/
}

static void gxi_callback_special(void)
{
		
	
}

//Initialization 

static int gxi_window = 0;

int gxi_no_vbos = ZFALSE; //set to true when falling back to vertex arrays
int gxi_fixed_function = ZFALSE; //set to true when falling back to fixed function

int gx_init(zint32 width, zint32 height, zchar* window_title, zuint32 options)
{
	/* Fake argc/argv fool GLUT into getting different parameters */
	int fakeargc=0;
	char *fakeargv0;
	char ** fakeargv= &fakeargv0;
	int ver[2];
	
	memset(gxi_keystate, 0, sizeof(gxi_keystate));		

	glutInit(&fakeargc, fakeargv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	//glutInitContextFlags (GLUT_CORE_PROFILE);
	//glutInitContextVersion(3,3);	
	

	if (options & GX_OPTION_NO_VBO) 
		gxi_no_vbos = ZTRUE;
	
	if (options & GX_OPTION_NO_SHADER)
		gxi_fixed_function = ZTRUE;
	
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL );
	gxi_window = glutCreateWindow(window_title);

	
	glGetIntegerv(GL_MAJOR_VERSION, &ver[0]);
	glGetIntegerv(GL_MINOR_VERSION, &ver[1]);
	printf(" OPENGL VERSION:%d/%d\n", ver[0], ver[1]);	
	

	//exit(0);
	
	//set callbacks
	glutReshapeFunc(gxi_callback_reshape);
	glutKeyboardFunc(gxi_callback_keyboard);
	glutKeyboardUpFunc(gxi_callback_keyboard_up);
	//glutSpecialInput(gxi_callback_special);
	glutMouseFunc(gxi_callback_mouseclick);
	glutMotionFunc(gxi_callback_mouseactive);
	glutPassiveMotionFunc(gxi_callback_mousepassive);
	glutDisplayFunc(gxi_callback_disp);
	glutEntryFunc(gxi_callback_mouse_entry);
	glPointSize(1.0);
	
	
	

	gxi_callback_reshape( width, height);  //reshape will use defaults

	gx_setup_2d(-1.0, -1.0, 1.0, 1.0) ;  //default coords are -1,-1 to 1,1
	//gx_setup_2d( 0.0f, 0.0f, width-1.0f, height-1.0f);

//	gxi_line_init();  //initialize line drawing functions

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
	
#if 0
			{
		int vao=0;
		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);
	}
#endif

//#define TESTOLDFALLBACK

#ifdef TESTOLDFALLBACK
		glGenBuffers = NULL;
		glDeleteBuffers = NULL;
		glBindBuffer = NULL;
		glBufferData = NULL;
#endif
	
	
	if (!glGenBuffers)
	{
		gxi_no_vbos = ZTRUE;
		printf("Cannot initialize VBO functions, using vertex arrays\n");
	}
	
	if (gxi_no_vbos) {
		gxi_fixed_function = ZTRUE; //can't used shaders without VBOs
		printf("Fallback to fixed function because VBO not available\n");
	}
		
		

	//default blending mode is alpha, but is off by default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);


	return GX_OK;
}


//uninitialize code
void gx_disable()
{
	
//	gxi_line_disable();

	if (gxi_window)
		glutDestroyWindow(gxi_window);
	gxi_window = 0;
	
	printf(" %d textures allocated\n", gxi_gl_textures_gen);
	printf(" %d textures deleted\n", gxi_gl_textures_del);
	printf(" %d vbos allocated\n", gxi_gl_vbos_gen);
	printf(" %d vbos deleted\n", gxi_gl_vbos_del);



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
	return gxi_keystate[ (unsigned char) a];
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

		gxi_mouse_capture_last_x=gxi_window_width/2;
		gxi_mouse_capture_last_y=gxi_window_height/2;		

		gxi_last_mouse_x = 0;
		gxi_last_mouse_y = 0;

		gxi_mouse_capture = ZTRUE;

		gxi_mouse_first_capture = ZTRUE; //we want to ignore the 1st mouse mouse event

		glutWarpPointer( gxi_mouse_capture_last_x, gxi_mouse_capture_last_y);	
		glutSetCursor(GLUT_CURSOR_NONE); //hide mouse pointer
#ifdef _WIN32
		ShowCursor(0);//windows call
#endif

	}
	else
	{
		gxi_mouse_capture = ZFALSE;
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
			*rel = gxi_mouse_capture;

		if (!gxi_mouse_capture)
		{

			*x = gxi_last_mouse_x;
			*y = gxi_last_mouse_y;
		}
		else
		{
			
			dx = gxi_last_mouse_x - gxi_mouse_capture_last_x;
			dy = gxi_last_mouse_y - gxi_mouse_capture_last_y;

			if ( dx || dy)  //if the mouse moved, re-center it
			{

				gxi_mouse_capture_last_x=gxi_window_width/2;
				gxi_mouse_capture_last_y=gxi_window_height/2;		
				glutWarpPointer( gxi_mouse_capture_last_x, gxi_mouse_capture_last_y);

			}
			if (gxi_mouse_first_capture)
			{
				dx=0;
				dy=0;
				gxi_mouse_first_capture = 0;
				
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

	if (gxi_window_width > 0)
	{
		*fx =  (x * (gxi_2d_right - gxi_2d_left) ) / gxi_window_width;
	}

	if (gxi_window_height > 0)
	{
		*fy =  (y * (gxi_2d_bottom - gxi_2d_top) ) / gxi_window_height;
	}
	
}

void gx_hide_mouse()
{
		glutSetCursor(GLUT_CURSOR_NONE); 
}

zbool gx_mouse_present()
{
	return gxi_mouse_present_state;
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
		*height = gxi_window_height;

	if (width)
		*width = gxi_window_width;

	return ((zfloat32) gxi_window_width) / ((zfloat32)gxi_window_height);
}



/* Two dimensional coord system */
void gx_setup_2d(float left,  float top, float right, float bottom)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, -1.0,1.0);
	glMatrixMode(GL_MODELVIEW);

//	gxi_reset_matrix();// reset camera matrix

	//makes most sense to disable depth:
	//glDepthMask(GL_FALSE);  //don't write to depth bufer

	glDisable(GL_DEPTH_TEST); //don't test depth buffer when drawing


	glDisable(GL_CULL_FACE);

	gxi_2d_top = top;
	gxi_2d_bottom = bottom;
	gxi_2d_left = left; 
	gxi_2d_right = right;


}

void gx_setup_2d_pixels(int* width, int *height)
{
	//give dimensions to client application
	gx_frame_get_dimensions(width, height);

	gx_setup_2d(-.375f, gxi_window_height-.375f, gxi_window_width-.375f, -.375f);
}


/* Three dim coord system */
void gxi_trans_set_perspective_matrix (zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);


void gx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist)
{
	
	gxi_trans_set_perspective_matrix( fovy, aspect, neardist, fardist);

	//probably want depth buffer:
	glClearDepth(1.0); //when clearing depth buffer, set to infinity
	glDepthRange(0,1);  //set range for full depth bufer
	glDepthFunc(GL_LEQUAL);  //draw things equally far or closer
//	glDepthMask(GL_TRUE); //write to depth bufer
	glEnable(GL_DEPTH_TEST);  //enable depth testing

	//glDisable(GL_CULL_FACE); //we want face culling (for now)

	glEnable(GL_CULL_FACE); //we want face culling (for now)
	glCullFace(GL_BACK);


	//glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//	glPolygonMode( GL_BACK, GL_LINE );

}

void gx_zbuffer_mapping(zfloat32 min, zfloat32 max) {
	glDepthRange(min, max);
}

void gx_wireframe(zbool a){

	if (a)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


//depth buffer
void gx_zbuffer(zbool dowrite, zbool dotest)
{
	if (!dowrite && !dotest) {
		//don't want to do anything with z buffer
		glDisable(GL_DEPTH_TEST);
		//disableing depth test ALSO disables depth writes
		return;
	}

	//need to enable GL_DEPTH_TEST if we are testing or writing to the depth buffer

	glEnable(GL_DEPTH_TEST);  //enabling testing enables writing to depth buffer in opengl


	if (dotest) 
		glDepthFunc(GL_LEQUAL);  //draw things equally far or closer
	else
		glDepthFunc(GL_ALWAYS);  //dummy test, always pass
	

	if (dowrite) 
		glDepthMask(GL_TRUE); 
	else
		glDepthMask(GL_FALSE);
	
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



//reads from input until delimiter in 'delims' in  reached
//return value:  0  reached terminator
//               -1 buffer is full
//				 -2 end of file
//				 >0 the delimiter character
zint32 gxi_read_to_delim(FILE* f, zchar* buffer, zuint32 buffer_len, zchar* delims)
{
	int i = 0 ;
	int c;
	int j;
	int retval = -1;
	int br=0;

	if (!f)
		return -1;


	while (i<buffer_len)
	{
		c = fgetc(f);
		if (feof(f) || c <0)
		{
			retval = -2;
			break;
		}

		if (c==0)
		{
			retval = 0;
			break;
		}

		for (j=0; delims[j];j++)
		{
			if (c== delims[j])
			{
				retval = delims[j];
				br=1;
				break;
			}
		}
		if (br) 
			break;
		buffer[i++]=c;
	}

	buffer[i]='\0';

	return retval;
}

