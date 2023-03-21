#pragma once

#define GXDEBUG 1
#define gxdprintf  if(GXDEBUG) printf

#include "ztypes.h"
#include "zmem.h"
#include "zlist.h"
#include "zvector.h"		//vector array (data structure)
#include "zvectormath.h"	//3d math
#include "math.h"
#include "math.h"

#ifdef GFXINTERNAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define checkGL()   checkGLfunc(__FILE__, __LINE__, "", ZFALSE);
#define checkGLNote(NOTE,TOLERABLE)   checkGLfunc(__FILE__, __LINE__, NOTE, TOLERABLE);

void checkGLfunc(char* file, int line, char* hint, zbool tolerable );




#endif

#include "gx_trans.h"
#include "gfx_texture.h"



//creates a zwindowT object that gives UI zevents
//creates an opengl context, and makes it current
struct zwindowS* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags);


/* Basic framebuffer and setup functions */
void gfx_background_color(float r, float g, float b, float a);
void gfx_frame_clear(zbool color, zbool depth);
void gfx_depth_buffer(zbool test, zbool write);
void gfx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
void gfx_setup_2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom);



/* graphics styles (high level wrapper for shaders and their parameters) */

#define GFX_BLEND_OFF	 1
#define GFX_BLEND_ALPHA	 2
#define GFX_BLEND_ADD	 3
#define GFX_BLEND_MUL	 4

typedef struct gfxstyleS {
	zvecT properties;
	struct gx_shadergroup_s* shader_group;
	zbool style_dirty; //if true, need to push changes to opengl before rendering
} gfx_styleT;

gfx_styleT* gfx_style_mk();


//set properties.  Pass GFX_DELETE if need to remove a value
//for light positions/directions that need to be specified in camspace, passing GFX_TRANSFORM_POINT or GFX_TRANSFORM_DIRECTION will transform using the current 'matrix'
#define GFX_DELETE				1
#define GFX_TRANSFORM_POINT		2
#define GFX_TRANSFORM_DIRECTION	3
void gfx_style_set_property(gfx_styleT* st, int idORtype, char* name_in, int index, int val, void* ptr, int action);

//selects a style to use for rendering
void gfx_style(gfx_styleT* st);


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
	
	char* buffer_spec;

} gfx_vertex_bufferT;


/*draw primitives*/

#define GFX_POINT	1
#define GFX_LINE	2
#define GFX_TRIANGLE	3

gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount, char* spec);
zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb, zuint16 a, zuint16 b, zuint16 c);
void gfx_vertex_data(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
zuint16 gfx_vertex_done(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
zuint16* gfx_vertex_buffer_add_index(gfx_vertex_bufferT* vb, int num);
void gfx_vertex_buffer_update(gfx_vertex_bufferT* vb);
void gfx_vertex_buffer_draw(gfx_vertex_bufferT* vb, int prim, int start, int end, zbool indexed);


#include "objloader.h"

#include "GL/glu.h"

#include "zwindow.h"

void gfx_gl_test();


//garbage function, to redo later
void gfx_arrow(vec3* p1, vec3* p2);


/*datatypes for properties*/

#define GFX_FLOAT		0x10000000
#define GFX_FLOAT2		0x20000000
#define GFX_FLOAT3		0x30000000
#define GFX_FLOAT4		0x40000000
#define GFX_INT			0x50000000
#define GFX_TEXTURE		0x60000000

//for FLOAT3 that get passed in:  put these in the valop field
#define GFX_POINT_TRANSFORM		1
#define GFX_NORMAL_TRANSFORM	2


#ifdef GFXINTERNAL

/* More internal things */


#define GXI_TYPEMASK	0xff000000

#define GXI_BLEND_MODE	(GFX_INT  |  1)

/* light DIRECTION and POSITION for the same 'n' are mutually exclusive! */
#define GXI_LIGHT_DIRECTION		(GFX_FLOAT3  | 2 )	
#define GXI_LIGHT_POSITION		(GFX_FLOAT3  | 3 )
#define GXI_LIGHT_COLOR			(GFX_FLOAT3  | 4 )
#define GXI_LIGHT_AMBIENT		(GFX_FLOAT4  | 5 )
#define GXI_TEXTURE_DIFFUSE		(GFX_TEXTURE | 6 )
#define GXI_SPECULAR_EXPONENT	(GFX_FLOAT   | 7 )
#define GXI_SPECULAR_COLOR		(GFX_FLOAT3  | 8 )
#define GXI_LIGHT_ATTENUATION	(GFX_FLOAT3  | 9 )
#define GXI_FOG_COLOR			(GFX_FLOAT3  | 10)
#define GXI_FOG_DENSITY			(GFX_FLOAT   | 11)

typedef struct gfx_propertyS {
	char* name;//user can name custom properties
	int id;
	int index;  //support multiple values of same kind of data (texture 0, texture 1... etc)
	int uloc;  //if using shaders, uniform location
	union {
		float f;	//single float  
		float fa[4]; //up to 4, for color, etc
		vec3 v; //3 component vector (position)
		vec4 v4; //3 component vector (position)
		int i;
		gfx_textureT* tex;

	} data;
} gfx_propertyT;



#define MAX_FF_LIGHTS 4

#endif

#include "glsl.h"
