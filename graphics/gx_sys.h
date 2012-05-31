// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define DOPRINTF

#define GX_OK 0
#define GX_ERROR 1



void gx_window_event();
int gx_init(int width, int height, char* window_title);
void gx_disable();

void gx_clear_color(float r, float g, float b, float a);
void gx_frame_clear(zbool color, zbool depth);
void gx_frame_show();

//return size of drawable window.  return value is aspect ratio
zfloat32 gx_frame_get_dimensions(zuint32* width, zuint32* height);

void gx_setup_2d(float left,  float top, float right, float bottom);
void gx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
void gx_zbuffer(zbool en);  //turns depth buffer test on/off




//read keyboard
zchar gx_getkey();
zbool gx_key_state(zbyte a);

//read mouse
void gx_mouse_pos(zuint32* x, zuint32* y, zbool* rel);
void gx_mouse_capture(zbool cap);

zbool gx_mouse_state(zuint32 button);
#define GX_MOUSE_LEFT	0
#define GX_MOUSE_MIDDLE	1
#define GX_MOUSE_RIGHT	2
#define GX_MOUSE_MAX	2

//camera control and simple transformations
void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis); //set camera position, orientation
void gx_home();  //resets any move / rotate/ scale transformations, but keeps camera orientation
void gx_camera_home();  //resets move and camera
void gx_move3d(vec3* amount); //applies translation
void gx_rotate_3x3(vec3* xaxis, vec3* yaxis, vec3* zaxis ); //applies a 3x3 matrix
void gx_rotate_3x3_cam(vec3* xaxis, vec3* yaxis, vec3* zaxis );// applies a 3x3 camera matrix

void gx_scale3d(vec3* scale); //scales
void gx_scale(float scale); //uniform scales





