// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>
#include "gx_buffers.h"

#include "gl/glew.h"
#include "gl/wglew.h"
#include "gl/freeglut.h"

static void _destruct_vbuffer(void* x)
{
	gx_vbuffer_t* v = x;


	//todo: free the guts of this

	ram_shallow_free(v);

}

#define VERTEX_COMPONENTS 3
#define COLOR_COMPONENTS 4

gx_vbuffer_t* gx_vbuffer_mk(zuint32 num_vertices, zuint32 num_indices, zbool use_color)
{

	gx_vbuffer_t* v = NULL;

	if (num_vertices ==0)
		return NULL;

	v = ram_alloc(sizeof(gx_vbuffer_t), _destruct_vbuffer);

	if (!v) 
		return NULL;

	v->vertex_capacity = num_vertices;
	v->vertex_data = ram_alloc(sizeof(zfloat32) * VERTEX_COMPONENTS * num_vertices, NULL);
	if (!v->vertex_data)
	{
		ram_free(v);
		return NULL;
	}

	if (use_color)  //if using color buffer, define it
	{
		v->color_data = ram_alloc(sizeof(zfloat32) * COLOR_COMPONENTS * num_vertices, NULL);
		if (!v->color_data)
		{
			ram_free(v);
			return NULL;
		}
	}
		
	return v;
}

zbool gx_vbuffer_enable(gx_vbuffer_t* v)
{
	if (!v)
		return zfalse;

	if (!v->_sent_to_gl)
	{

		glGenBuffers(1, &(v->_vertex_vbo));

		if (v->color_data)
		{
			glGenBuffers(1, &(v->_color_vbo));
		}

		v->_sent_to_gl = 1;
	}

	glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_vbo );
	glBufferData(GL_ARRAY_BUFFER, v->vertex_count * VERTEX_COMPONENTS *sizeof(float) , v->vertex_data, GL_STREAM_DRAW);


	if (v->color_data)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_color_vbo );
		glBufferData(GL_ARRAY_BUFFER, v->vertex_count * COLOR_COMPONENTS *sizeof(float) , v->color_data, GL_STREAM_DRAW);
	}


	return ztrue;
}






//adds color to a new vertex
void gx_vbuffer_add_color(gx_vbuffer_t* v, zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a)
{

	if (!v)
		return;

	if (!v->color_data)
		return;

	if (v->vertex_count == v->vertex_capacity)
		return; //we are full!

	v->color_data[COLOR_COMPONENTS * v->vertex_count] = r;
	v->color_data[COLOR_COMPONENTS * v->vertex_count + 1] = g;
	v->color_data[COLOR_COMPONENTS * v->vertex_count + 2] = b;
#if COLOR_COMPONENTS == 4
	v->color_data[COLOR_COMPONENTS * v->vertex_count + 3] = a;
#endif
}

//finalizes the current vertex, and returns a vertex index for it
zint32 gx_vbuffer_add_vertex(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z)
{
	if (!v)
		return -1;

	if (v->vertex_count == v->vertex_capacity)
		return -1; //we are full!

	v->vertex_data[VERTEX_COMPONENTS * v->vertex_count] = x;
	v->vertex_data[VERTEX_COMPONENTS * v->vertex_count + 1] = y;
	v->vertex_data[VERTEX_COMPONENTS * v->vertex_count + 2] = z;

	v->vertex_count ++;

	return v->vertex_count - 1;
}




//drawing a vbuffer

void gx_vbuffer_draw(gx_vbuffer_t* v, zuint32 start, zuint32 stop, gx_prim_e prim  , zbool indexed)
{
	if (!v)
		return;
	

	if (stop <=start)
		return;

	if (v->_vertex_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_vbo );
		glVertexPointer(VERTEX_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);	
	}

	if (v->_color_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_color_vbo);
		glColorPointer(COLOR_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	else
	{
		glDisableClientState(GL_COLOR_ARRAY);
		glColor4f(1,1,1,1);  //use white
	}

	if (indexed)
	{
		printf(" Indexed meshes not yet supported\n");
	}
	else
	{
		switch(prim)
		{
		case gx_points:
			glDrawArrays(GL_POINTS, start, stop-start);
			break;
		
		case gx_lines:
			glDrawArrays(GL_LINES, start, stop-start);
			break;
		
		case gx_triangles:
			glDrawArrays(GL_TRIANGLES, start, stop-start);
			break;
		}
	}
}



//test function to dump a set of vertices onto the screen
void gx_test_draw_vertices(gx_vbuffer_t* v)
{

	if (!v)
		return;

	if (v->_vertex_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_vbo );
		glVertexPointer(VERTEX_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);	
	}

	if (v->_color_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_color_vbo);
		glColorPointer(COLOR_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	else
	{
		glColor4f(1,1,1,1);  //use white
	}

	glDrawArrays(GL_POINTS,0, v->vertex_count);
}