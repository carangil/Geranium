#pragma once

#define GXDEBUG 1
#define gxdprintf  if(GXDEBUG>=1) printf
#define gxdtracef  if(GXDEBUG>=2) printf

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


//#define DISABLE_FIXED_FUNCTION

//creates a zwindowT object that gives UI zevents
//creates an opengl context, and makes it current
//Zdef proc gfx_mkwindow GLWindow
struct zwindow_s* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags);

/* Basic framebuffer and setup functions */
//Zdef proc gfx_background_color BackgroundColor
void gfx_background_color(float r, float g, float b, float a);

//Zdef proc gfx_frame_clear FrameClear
void gfx_frame_clear(zbool color, zbool depth);

//Zdef proc gfx_depth_buffer DepthBuffer
void gfx_depth_buffer(zbool test, zbool write);

//Zdef proc gfx_setup_3d Setup3D
void gfx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);

//Zdef proc gfx_setup_2d Setup2D
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

//Zdef opaque gfx_styleT Style

//Zdef proc gfx_style_mk NewStyle
gfx_styleT* gfx_style_mk();

//set properties.  Pass GFX_DELETE if need to remove a value
//for light positions/directions that need to be specified in camspace, passing GFX_TRANSFORM_POINT or GFX_TRANSFORM_DIRECTION will transform using the current 'matrix'
#define GFX_DELETE				1
#define GFX_TRANSFORM_POINT		2
#define GFX_TRANSFORM_DIRECTION	3
//Zdef proc gfx_style_set_property Set
void gfx_style_set_property(gfx_styleT* st, int idORtype, char* name_in, int index, int val, void* ptr, int action);

//selects a style to use for rendering
//Zdef proc _gfx_style Use
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
#define GFX_QUAD		6
/* QUAD is 6 because it is split in 2 triangles, 6 vertices: opengl wants triangles */

//Zdef type gfx_vertex_bufferT VBuffer

//Zdef proc gfx_vertex_buffer_mk NewVBuffer
gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount, char* spec);

//Zdef proc gfx_index_triangle AddTriangle
zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb, zuint16 a, zuint16 b, zuint16 c);

//Zdef proc gfx_vertex_data Data
//Zdef proc gfx_vertex_done Done

void gfx_vertex_data(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
zuint16 gfx_vertex_done(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);

#define gfx_vertex_data4	gfx_vertex_data
#define gfx_vertex_done4	gfx_vertex_done
#define gfx_vertex_data2(BUF, ATTR, S, T)		gfx_vertex_data(BUF, ATTR, S, T, 0.0f, 0.0f)
#define gfx_vertex_data3(BUF, ATTR, X, Y, Z)	gfx_vertex_data(BUF, ATTR, X, Y, Z, 0.0f)
#define gfx_vertex_done2(BUF, ATTR, S, T)		gfx_vertex_done(BUF, ATTR, S, T, 0.0f, 0.0f)
#define gfx_vertex_done3(BUF, ATTR, X, Y, Z)	gfx_vertex_done(BUF, ATTR, X, Y, Z, 0.0f)


//Zdef proc gfx_vertex_buffer_add_index AddIndexBuffer
void gfx_vertex_buffer_add_index(gfx_vertex_bufferT* vb, int num);

//Zdef proc gfx_vertex_buffer_update Update
void gfx_vertex_buffer_update(gfx_vertex_bufferT* vb);

//Zdef proc gfx_vertex_buffer_draw Draw
void gfx_vertex_buffer_draw(gfx_vertex_bufferT* vb, int prim, int start, int end, zbool indexed);


//temporary vertex buffers:
//these do not need to be freed, and are automatically recycled as needed
//Zdef proc gfx_vertex_temp TempVBuffer
gfx_vertex_bufferT* gfx_vertex_temp(struct zwindow_s* gw ,char* spec);  //returns a vertex buffer in speficed format.  CAn hold MAX_GFX_TEMP vertices
//Zdef proc gfx_vertex_buffer_draw_clear	 DrawReset
void gfx_vertex_buffer_draw_clear(gfx_vertex_bufferT* vb, zuint32 prim);  //draws the temporary vertex buffer's contents with the currently selected style.
//can draw TRIANGLE or QUAD

#define GFX_MAX_TEMP 65535

//If there is enough space to hold  count number of prims, then this doesn't do anything.
//otherwise it will draw and clear the buffer.
//If drawing a large number of immediate primitives, calling this function every so often (at least once per GFX_MAX_TEMP vertices), then you don't need to bother counting exactly
//Zdef proc gfx_vertex_buffer_continue DrawContinue
void gfx_vertex_buffer_continue(gfx_vertex_bufferT* vb, zuint32 prim, zuint32 count);



#include "objloader.h"

#include "GL/glu.h"

#include "zwindow.h"

//void gfx_gl_test();

void gfx_arrow(vec3* p1, vec3* p2, vec4* color);

/*datatypes for properties*/

#define GFX_FLOAT		0x10000000
#define GFX_FLOAT2		0x20000000
#define GFX_FLOAT3		0x30000000
#define GFX_FLOAT4		0x40000000
#define GFX_INT			0x50000000
#define GFX_TEXTURE		0x60000000
#define GFX_SWITCH		0x70000000  /* Flag creates a enable_Xn macro, with X the property name and n is the index (foo would be enable_foo0)*/

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
		vec3 v;  //3 component vector (position)
		vec4 v4; //4 component vector (position)
		int i;
		gfx_textureT* tex;
	} data;
} gfx_propertyT;

/*glfw windowing and zevent interface*/
typedef struct gfx_windowS {
	zwindowT iface;	//the zevent window interface
	GLFWwindow* fwindow;
	zvecT* tempvbufs;	//additional objects to free when window is closed
	struct shadergroup_s* basic_shader;
}gfx_windowT;


extern gfx_windowT* gxi_current_window;


#define MAX_FF_LIGHTS 4

zbool gfx_free_mesh(gfx_meshT* m);

#endif

#include "glsl.h"
