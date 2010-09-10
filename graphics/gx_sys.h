// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#define GX_OK 0
#define GX_ERROR 1


#define GX_LOOP_NOTHING 0
#define GX_LOOP_EXIT 1


int gx_window_event();
int gx_init(int width, int height, char* window_title);

void gx_clear_color(float r, float g, float b, float a);
void gx_frame_clear(zbool color, zbool depth);
void gx_frame_show();
void gx_setup_2d(float left,  float top, float right, float bottom);
void gx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);

zfloat32 gx_get_image_dimensions(zuint32* width, zuint32* height);

//todo: move from system into its own file:

void gx_camera_pos(float x, float y, float z);

void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis);


//rotates 3 vectors around each other
void gx_spin(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward);



//move this IO stuff somewhere else
zchar gx_getkey() ; //get rid of this crap!

void gx_mouse_pos(zuint32* x, zuint32* y);
void gx_mouse_capture(zbool cap);
zbool gx_key_state(zbyte a);

