
#ifndef GXGLSLH

#define GXGLSLH

typedef struct shadergroup_s {
	char* vsource;
	char* fsource;
	zlistT	variants;
	zvecT* shader_inputs;
} gx_shadergroupT;

void gfx_shader_add_input(gx_shadergroupT* sg, char* name, int type, zbool uniform);

//Zdef type gx_shadergroupT ShaderGroup

//Zdef proc gx_shader_source ShaderSource
//gives source code for a shader group
gx_shadergroupT* gx_shader_source(char* vsource, char* fsource);


//Zdef proc gx_set_basic_shader DefaultShader

void gx_set_basic_shader(char* vsource, char* fsource);




typedef struct gx_shader_variant_s {
	zlistnodeT zlistnode;
	
	zuint64 mask; //which inputs are in use.  0 is all, otherwise 1<<n is for nth input

	zuint32 v_shader;
	zuint32 f_shader;
	zuint32 program;
		
	//for older shaders (want to deprecate these soon):
	char* key;
	zuint32* uloc;
	zuint32* aloc;
	zuint32 modelview_uloc;
	zuint32 projection_uloc;
	zuint32 camera_pos_uloc;
	zuint32 ambient_uloc;
	zuint32 matrix_version;
}gx_shader_variantT;




//get a particular version of a shader given some spec
gx_shader_variantT* gx_shader_variant(gx_shadergroupT* sg, char* key, gfx_styleT* st, gfx_vertex_bufferT* vb);

//new version
gx_shader_variantT* gx_shader_variant2(gx_shadergroupT* sg, zuint64 mask);

//vsource = vertex shader
//fsource = fragment shader
gx_shader_variantT* gx_compile_shader(char* vsource, char* fsource, zvecT* gfx_shader_inputs, zuint64 mask);

typedef struct shader_inputT {
	char*	name;
	int		type;		// such as GFX_FLOAT
	int		loc;
	zbool	uniform;	//true if uniform (false for attributes)
	//zbool   writable;	//true if will be written by compute shader
} gfx_shader_inputT;

int gfx_sizeof(int type);
void gfx_set_input(gfx_shader_inputT* input, void* data);
void gx_use_shader(gx_shader_variantT* shader);

#endif
