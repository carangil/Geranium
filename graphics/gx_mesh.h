// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.



typedef struct gx_mesh_s
{
	
	gx_vbuffer_t*	data;
	gx_drawstyle_t*	style;

	zuint32			drawstart;	//vbuffer start and end points to draw
	zuint32			drawend;
	zbool			indexed;	//using indices?
	struct gx_mesh_s*	next;  //next mesh segment  (large meshes may be composed of multiple segments)
	char*			name;  //name of obj 'o' object
	gx_prim_e		prim;	//type of mesh

} gx_mesh_t;

//defines a mesh
gx_mesh_t*  gx_mesh_def(gx_vbuffer_t* v, gx_drawstyle_t* s, zuint32 drawstart, zuint32 drawend, zbool indexed);

//draws a mesh
void gx_mesh_draw(gx_mesh_t* mesh_in);
void gx_mesh_draw_at(gx_mesh_t* mesh_in, vec3* pos );

gx_mesh_t* gx_mesh_load_obj(gx_vbuffer_t* vbuf,  zchar* filename, zvec_t* drawstyles);
