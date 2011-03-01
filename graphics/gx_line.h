// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


void _gx_line_init();
void _gx_line_disable();

void gx_line(float x, float y, float x2, float y2);
void gx_line_color(zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a);
void gx_line_thickness(int pixels);
void gx_arcgon(float x, float y, float w,float  h, float start_angle, float end_angle, zuint32 sides, zbool pie);
void gx_box(float xmin, float ymin, float xmax, float ymax);

//finishes current drawing commands
void gx_line_finish();