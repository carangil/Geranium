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

zuint32 gx_width();
zuint32 gx_height();
