// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.



#define GX_INDEX_INVALID 0xFFFFFFFF
#define GX_MAX_TEXCOORD 4

typedef struct {

	//Vertex Information
	zfloat32* combined_vertex_data; //allocated buffer containing all the data
	
	//pointers into combined vertex data
	zfloat32* pos_data;   //position
	zfloat32* color_data;    //optional color
	zfloat32* normal_data;    //normal data	
	zfloat32* texcoord_data[GX_MAX_TEXCOORD];// note: only texcoord_data[0] is part of the combined buffer above
	//texcoord_data[1] to  texcoord_data[GX_MAX_TEXTURES-1] are to be stored seperately
    
	zuint32   num_texcoord;  //how many textures are applied?

	zuint32 vertex_capacity;  //number of vertices to fit
	zuint32 vertex_count;     //number of vertices here
	size_t	size_per_vertex;    //up to 1 texcoord

	//index information:
	zuint32* index_data;	 //array of indices
	zuint32 index_count;     //number of indices currently stored
	zuint32 index_capacity;  //number of indices that can fit

	//opengl information:
	zbool   _sent_to_gl;
	zuint32 _vertex_combined_vbo;
	zuint32 _index_vbo;
    /* TODO when dealing with multiple texture coordinates: */
    //zuint32 _texcoord_vbo[GX_MAX_TEXTURES]; //note: _texcoord_vbo[0] will not be used
    
    char* spec; //spec string for shader generation
}  gx_vbuffer_t;

typedef enum
{
	gx_points  = 0,
	gx_lines ,
	gx_triangles ,
	gx_quads
} gx_prim_e;

typedef zint32 vindex;

//gx_vbuffer_t* gx_vbuffer_mk(zuint32 num_vertices, zuint32 num_indices, zbool use_color, zuint32 texture_buffer_count);
void gx_vbuffer_add_normal(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z);

#define GX_VBUFFER_COLOR        1
#define GX_VBUFFER_NORMAL       2
#define GX_VBUFFER_TEXCOORD     4

gx_vbuffer_t* gx_vbuffer_mk(zuint32 num_vertices, 
        zuint32 num_indices, 
        zuint32 options
       );
//todo: function to add additional texcoord buffers, etc

zbool gx_vbuffer_update(gx_vbuffer_t* v);
zbool gx_vbuffer_update_indices(gx_vbuffer_t* v);

void gx_vbuffer_tex2(gx_vbuffer_t* v, zuint32 texture, zfloat32 s, zfloat32 t);
void gx_vbuffer_color4(gx_vbuffer_t* v, zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a);
zint32 gx_vbuffer_vertex3(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z);
void gx_vbuffer_normal3(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z);

#define gx_vbuffer_vertex(AAA,BBB) gx_vbuffer_vertex3(AAA, (BBB).vec3x, (BBB).vec3y, (BBB).vec3z)
#define gx_vbuffer_normal(AAA,BBB) gx_vbuffer_normal3(AAA, (BBB).vec3x, (BBB).vec3y, (BBB).vec3z)


zint32 gx_vbuffer_import_vertex(gx_vbuffer_t* dest, gx_vbuffer_t* src, zint32 vertex);

//clears data in a buffer:
void gx_vbuffer_clear(gx_vbuffer_t* v, zbool clear_index, zbool clear_vertex);

//returns how much space is in a vbuffer
zuint32 gx_remaining_vertices(gx_vbuffer_t* v);
zuint32 gx_remaining_indices(gx_vbuffer_t* v);


void gx_vbuffer_draw(gx_vbuffer_t* v, zuint32 start, zuint32 stop, gx_prim_e prim  , zbool indexed);


void gx_test_draw_vertices(gx_vbuffer_t* v);


void gx_vbuffer_add_index(gx_vbuffer_t* v, zuint32 i);



zint32 gx_vbuffer_current_index(gx_vbuffer_t* v);
zint32 gx_vbuffer_current_vertex(gx_vbuffer_t* v);


char* gxi_vbuffer_spec(gx_vbuffer_t* vb);

#if 0

gx_vbuffer_t* gx_vbuffer_from_image(gx_vbuffer_t* preferred_buffer,
									gx_image_t* image, 
									zfloat32 xoff,
									zfloat32 yoff,
									zfloat32 zoff,
									zbyte xaxis,
									zbyte yaxis,
									byte zaxis,
									float32 xsize, 
									zfloat32 ysize, 
									zfloat32 zsize, 
									zbool use_color,
									zuint32 num_texture,
									zuint32* start,
									zuint32* end,
									zbool flipnorm
									);

#endif


//access vertex data

//tell opengl only use 3 coordinats
#define VERTEX_USE_COMPONENTS 3

//we pad out 4 coordinates
#define VERTEX_COMPONENTS VEC3LEN
#define COLOR_COMPONENTS 4
#define TEXTURE_COMPONENTS 2

//internal accessors
#define gxi_vbuffer_v(vbbb, iii)   ((vec3*)(&((vbbb)->pos_data[(iii)* VERTEX_COMPONENTS])))
#define gxi_vbuffer_n(vbbb, iii)   ((vec3*)(&((vbbb)->normal_data[(iii)* VERTEX_COMPONENTS])))
#define gxi_vbuffer_s(vbbb, iii, ttt)  ((vbbb)->texcoord_data[ttt][(iii)* TEXTURE_COMPONENTS])
#define gxi_vbuffer_t(vbbb, iii, ttt)  ((vbbb)->texcoord_data[ttt][(iii)* TEXTURE_COMPONENTS+1])




//experimental accessors
#if 0
//#define gx_vbuffer_x(vbbb, iii)  ((vbbb)->vertex_data[(iii)* VERTEX_COMPONENTS])
//#define gx_vbuffer_y(vbbb, iii)  ((vbbb)->vertex_data[(iii)* VERTEX_COMPONENTS+1])
//#define gx_vbuffer_z(vbbb, iii)  ((vbbb)->vertex_data[(iii)* VERTEX_COMPONENTS+2])


//#define gx_vbuffer_c(vbbb, iii)   ((float*)(&((vbbb)->color_data[(iii)* COLOR_COMPONENTS])))




#endif
