// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#define MAX_BONE 4
typedef struct bone_weights {
	zuint16	bone[MAX_BONE];
	float	weight[MAX_BONE];
}gfx_bone_weightT;



//Zdef struct gfx_meshT Mesh:style=style:Style%;vb=vb:VBuffer%;drawstart=start:N32;drawend=stop:N32;

typedef struct gx_mesh_s
{
	
	gfx_styleT*			style;
	gfx_vertex_bufferT*	vb;

	zuint32			drawstart;	//vbuffer start and end points to draw
	zuint32			drawend;


	gfx_vertex_bufferT*	vbaux;//additional vertex data that isn't rendered ( like the rest pose of a mesh or whatever)
	gfx_bone_weightT*	bone_bind;
	zuint32*			vertex_n; //from obj file

	struct gx_mesh_s* next;
	
	//gfx_vertex_bufferT* vdebug;
	//zint32* group_name;
	//zvecT* group_names;

} gfx_meshT;


//defines a mesh
//gx_mesh_t*  gx_mesh_def(gx_vbuffer_t* v, gx_drawstyle_t* s, zuint32 drawstart, zuint32 drawend, zbool indexed);
//#define Counted_gfx_meshT *Mesh


//Zdef proc gfx_mesh_load_objmm MeshLoadObjMM
gfx_meshT* gfx_mesh_load_objmm(zchar* filename, float scale, vec3* min, vec3* max);


//Zdef noproto gfx_mesh_load_obj MeshLoadObj
gfx_meshT* gfx_mesh_load_obj(zchar* filename, float scale);  //fictitous function

#define gfx_mesh_load_obj(A,B) gfx_mesh_load_objmm(A,B,NULL,NULL)


#define MAX_CHANNELS 6


/*	When importing from blender, if you have bones named like this:
	*   *=====A=====>*=====B=====>*=====C=====>
	*                 \\
	*                  \D
	*                   \\
	*                    \|
	*
	* The joints will be named like this:
	*
	*   A===========>B==========>C==========>EndSite
	*                D\\
	*                  \\
	*                   \\
	*                    \|EndSite
	*	Note bones B and D start at the same point (at the end of A), but each has their own matrix.
	*
	*/

typedef struct jointS {

	//hierarchy
	zvecT children;
	struct jointS* parent;

	//flat list of bones, only for root bone
	zvecT bones;

	char* name;	//name of the joint.
	
	//from file:
	vec3 offset; //offset relative to parent
	int numchannels;
	int channels[MAX_CHANNELS];
	float* framedata;

	//calculated

	vec4 debugcolor;
	gfx_transformT stransform; //accumulated transform of this joint's matrix for the current pose being iterated
	vec3 point;	//this joint's position, transformed by the current pose
	vec3 total_offset; // total offset relative to root.  This what needs to be subtracted from the rest pose
	zbool is_end;  //trus if 'end site'
	zbool ignored; //if true, done isn't drawn, or linked to vertices, etc
} gfx_jointT;

//bones w/ names the same as ignorelist will be flagged as ignored

gfx_jointT* load_bvh(char* filename, float scale, zvecT* ignorelist);


#define SKEL_OP_DRAW	0x00000001


//Starting with the current transform matrix, the skeleton is recursed in the pose frame, and each bone is assigned a transform
//If op & SKEL_OP_DRAW, the non-ignored bones are drawn

void recurse_skeleton(gfx_jointT* joint, int frame, int op);


extern gfx_transformT test_trans;

void debug_print_skeleton(gfx_jointT* joint, int indent);