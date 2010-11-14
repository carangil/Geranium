// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.



#define GX_INDEX_INVALID 0xFFFFFFFF
#define GX_MAX_TEXTURES 4

typedef struct {

	//Vertex Information
	zfloat32* vertex_data;   //position
	zfloat32* color_data;    //optional color
	
	
	zfloat32* texcoord_data[GX_MAX_TEXTURES]; //texcoord data for each texture
	zuint32   num_textures;  //how many textures are applied?

	zuint32 vertex_capacity;  //number of vertices to fit
	zuint32 vertex_count;     //number of vertices here


	//index information:
	zuint32* index_data;	 //array of indices
	zuint32 index_count;     //number of indices currently stored
	zuint32 index_capacity;  //number of indices that can fit


	//opengl information:
	zbool   _sent_to_gl;
	zuint32 _vertex_vbo;
	zuint32 _index_vbo;
	zuint32 _color_vbo;
	zuint32 _texcoord_vbo[GX_MAX_TEXTURES];

//	zuint32 index_vbo;

}  gx_vbuffer_t;

typedef enum
{
	gx_points  = 0,
	gx_lines ,
	gx_triangles 
} gx_prim_e;


gx_vbuffer_t* gx_vbuffer_mk(zuint32 num_vertices, zuint32 num_indices, zbool use_color, zuint32 texture_buffer_count);

zbool gx_vbuffer_update(gx_vbuffer_t* v);

void gx_vbuffer_add_tex(gx_vbuffer_t* v, zuint32 texture, zfloat32 s, zfloat32 t);
void gx_vbuffer_add_color(gx_vbuffer_t* v, zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a);
zint32 gx_vbuffer_add_vertex(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z);


//returns how much space is in a vbuffer
zuint32 vbuffer_remaining(gx_vbuffer_t* v, zuint32* index_remaining);

void gx_vbuffer_draw(gx_vbuffer_t* v, zuint32 start, zuint32 stop, gx_prim_e prim  , zbool indexed);


void gx_test_draw_vertices(gx_vbuffer_t* v);

//returns the index of the index added
zint32 gx_vbuffer_add_index(gx_vbuffer_t* v, zuint32 i);

gx_vbuffer_t* gx_vbuffer_from_image(gx_vbuffer_t* preferred_buffer,
									gx_image_t* image, 
									zfloat32 xoff,
									zfloat32 yoff,
									zfloat32 zoff,
									zbyte xaxis,
									zbyte yaxis,
 									zbyte zaxis,
									zfloat32 xsize, 
									zfloat32 ysize, 
									zfloat32 zsize, 
									zbool use_color,
									zuint32 num_texture,
									zuint32* start,
									zuint32* end
									);