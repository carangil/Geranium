// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define DOPRINTF

#define GX_OK 0
#define GX_ERROR 1

#include "glheaders.h"

#define GX_OPTION_NO_SHADER	1
#define GX_OPTION_NO_VBO	2

//call gx_init before any other graphics functiosn
int gx_init(zint32 width, zint32 height, zchar* window_title, zuint32 options);

//call once per frame
void gx_window_event();

//call when done
void gx_disable();

//framebuffer commands
void gx_clear_color(float r, float g, float b, float a);
void gx_frame_clear(zbool color, zbool depth);
void gx_frame_show();
//return size of drawable window.  return value is aspect ratio
zfloat32 gx_frame_get_dimensions(zuint32* width, zuint32* height);

//setup projection matrices for 2d or 3d drawing
void gx_setup_2d(float left,  float top, float right, float bottom);
void gx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);

//sets up 2d coordinate system so 1.0 per pixel. Returns the integer size of the drawing window
void gx_setup_2d_pixels(int* width, int *height);


void gx_zbuffer(zbool write, zbool test);  //turns depth buffer test on/off

void gx_zbuffer_mapping(zfloat32 min, zfloat32 max); //maps near and far z planes.  0.0 to 1.0 is the recommended value

//read keyboard
zchar gx_getkey();
zbool gx_key_state(zbyte a);

//read mouse
void gx_mouse_pos(zint32* x, zint32* y, zbool* rel);
void gx_mouse_posf(zfloat32* fx, zfloat32* fy, zbool* rel); //translated to last setup_2d coordinates
void gx_mouse_capture(zbool cap);
zbool gx_mouse_present();
void gx_hide_mouse();

zbool gx_mouse_state(zuint32 button);
#define GX_MOUSE_LEFT	0
#define GX_MOUSE_MIDDLE	1
#define GX_MOUSE_RIGHT	2


//camera control and simple transformations
void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis); //set camera position, orientation

extern int gxi_no_vbos;
extern int gxi_fixed_function;

void gx_error(char* file, int line);
#define GX_TRACE gx_error( __FILE__ , __LINE__);

#define GXDEBUG 0
#define gxdprintf  if(GXDEBUG) printf


void gx_wireframe(zbool a);

zint32 gxi_read_to_delim(FILE* f, zchar* buffer, zuint32 buffer_len, zchar* delims);

