// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>
#include "gx_image.h"
#include "gx_buffers.h"
#include "gx_line.h"
#include <math.h>


//#include "glstuff.h"

//This is a simple immediate-mode line and shape drawing API

#ifndef PI
#define PI 3.14159
#endif

#ifndef GX_LINE_BUFFER_VERTEX_COUNT
#define GX_LINE_BUFFER_VERTEX_COUNT (1024)
#endif


static zfloat32 _gx_line_next_color_r=1.0;
static zfloat32 _gx_line_next_color_g=1.0;
static zfloat32 _gx_line_next_color_b=1.0;
static zfloat32 _gx_line_next_color_a=1.0;

static gx_vbuffer_t* _gx_line_buffer = NULL;

//defines color for the next lines to be drawn
void gx_line_color(zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a)
{
	_gx_line_next_color_r=r;
	_gx_line_next_color_g=g;
	_gx_line_next_color_b=b;
	_gx_line_next_color_a=a;
}

void _gx_line_init()
{
	//create a buffer
	_gx_line_buffer = gx_vbuffer_mk(GX_LINE_BUFFER_VERTEX_COUNT, 0, ztrue, 0);
}

void _gx_line_disable()
{
	//create a buffer
	ram_free(_gx_line_buffer);
}

void gx_line(float x, float y, float x2, float y2)
{
	zuint32 rem=gx_remaining_vertices(_gx_line_buffer, NULL);

	if ( rem >=2)
	{
		//if we have at least two vertices left in the buffer, lets draw with them
		gx_vbuffer_add_color(_gx_line_buffer, _gx_line_next_color_r,_gx_line_next_color_g, _gx_line_next_color_b, _gx_line_next_color_a);
		gx_vbuffer_add_vertex(_gx_line_buffer, x,y, 0);
		gx_vbuffer_add_color(_gx_line_buffer, _gx_line_next_color_r,_gx_line_next_color_g, _gx_line_next_color_b, _gx_line_next_color_a);
		gx_vbuffer_add_vertex(_gx_line_buffer, x2,y2, 0);
	}

	if (rem<=3)
	{	//if we are exactly full
#ifdef DOPRINTF 
		printf(" LINE BUFFER FULL: need to draw\n");
#endif
		gx_line_finish();
	}

}

//draws any pending line operations, clears the buffer
void gx_line_finish()
{
	gx_vbuffer_update(_gx_line_buffer);

	gx_set_active_textures(NULL, 0);

	gx_vbuffer_draw(_gx_line_buffer, 0, _gx_line_buffer->vertex_count, gx_lines, zfalse);

	gx_vbuffer_clear(_gx_line_buffer,zfalse, ztrue); //clear out all data in this buffer

}


// Draws regular n-gons, approximates arcs, circles and 'pies'
void gx_arcgon(float x, float y, float w,float  h, float start_angle, float end_angle, zuint32 sides, zbool pie)
{
	zfloat32 angle=0;
	zfloat32 delta_theta= 2.0 * PI / sides;
		
	zfloat32 x1 = 0;
	zfloat32 y1 = 0;
	zuint32 i = 0;

	zfloat32 x0 = x;
	zfloat32 y0 = y;
	

	for( angle = start_angle; angle < end_angle ; angle += delta_theta )
	{

		x0=x1;
		y0=y1;
		x1 = w*cos(angle)+x;
		y1 = h*sin(angle)+y;
		

		if (i>0 || pie )
		{	
			gx_line( x0,y0, x1, y1);
		}
		
		i++;
	}

	x1 = w*cos(end_angle)+x;
	y1 = h*sin(end_angle)+y;

	gx_line( x0,y0, x1, y1);



	if (pie)
		gx_line(x,y,x1,y1);


}