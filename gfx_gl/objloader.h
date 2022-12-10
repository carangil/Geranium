// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.



typedef struct gx_mesh_s
{

	gfx_vertex_bufferT* vb;
	
	zuint32			drawstart;	//vbuffer start and end points to draw
	zuint32			drawend;
	zbool			indexed;	

	struct gx_mesh_s* next_piece;
	
} gfx_meshT;

//defines a mesh
//gx_mesh_t*  gx_mesh_def(gx_vbuffer_t* v, gx_drawstyle_t* s, zuint32 drawstart, zuint32 drawend, zbool indexed);

gfx_meshT* gfx_mesh_load_obj(zchar* filename);


