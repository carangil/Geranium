// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.



typedef struct {
	//Vertex information:

	zfloat32* vertex_data;
	zfloat32* color_data;

	zuint32 vertex_capacity;  //number of vertices to fit
	zuint32 vertex_count;     //number of vertices here



	//index information:
//	zuint32* indices;		//array of indices
//	zuint32 index_count;     //number of indices currently stored
//	zuint32 index_capacity;  //number of indices that can fit


	//opengl information:
	zbool   _sent_to_gl;
	zuint32 _vertex_vbo;
	zuint32 _color_vbo;
//	zuint32 index_vbo;

}  gx_vbuffer_t;

typedef enum
{
	gx_points  = 0,
	gx_lines ,
	gx_triangles 
} gx_prim_e;


gx_vbuffer_t* gx_vbuffer_mk(zuint32 num_vertices, zuint32 num_indices, zbool use_color);

zbool gx_vbuffer_enable(gx_vbuffer_t* v);

void gx_vbuffer_add_color(gx_vbuffer_t* v, zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a);
zint32 gx_vbuffer_add_vertex(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z);

void gx_vbuffer_draw(gx_vbuffer_t* v, zuint32 start, zuint32 stop, gx_prim_e prim  , zbool indexed);


void gx_test_draw_vertices(gx_vbuffer_t* v);

