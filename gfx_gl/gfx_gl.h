#pragma once

#define GXDEBUG 1
#define gxdprintf  if(GXDEBUG) printf

#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"		//vector array (data structure)
#include "zvectormath.h"	//3d math
#include "math.h"

#ifdef GFXINTERNAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "gx_trans.h"
#include "gfx_texture.h"

/* Vertex Buffer Objects */

typedef struct gfxVertexAttributeS {
	int		type;	// 1,2,3, or 4 are for float values
	char* name;
	float* data;
} gfx_vertex_attributeT;

#define MAX_ATTRIBUTE 8

typedef struct gfx_VertexBufferS {
	float* combined_data;

	gfx_vertex_attributeT attributes[MAX_ATTRIBUTE];
	int num_attributes;

	zuint16 capacity;
	zuint16 count; //number of vertices to consider valid
	int vbo;
	int fcount; //number of float fields

	zuint16* index_buffer;	//zarray
	int index_vbo;

	//positions for fixed/simple pipeline functionality
	//only valid if fixed_position != -1
	int fixed_position;
	int fixed_color;
	int fixed_texcoord;
	int fixed_normal;
} gfx_vertex_bufferT;


gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount, char* spec);
zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb, zuint16 a, zuint16 b, zuint16 c);
void gfx_vertex_data(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
zuint16 gfx_vertex_done(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
zuint16* gfx_vertex_buffer_add_index(gfx_vertex_bufferT* vb, int num);

#include "objloader.h"

#include "GL/glu.h"

#include "zwindow.h"

void gfx_gl_test();