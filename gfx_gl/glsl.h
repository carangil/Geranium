
#ifndef GXGLSLH

#define GXGLSLH
typedef struct shadergroup_s {
	char* vsource;
	char* fsource;
	zlistT variants;
} gx_shadergroupT;
//Zdef type gx_shadergroupT ShaderGroup

//Zdef proc gx_shader_source ShaderSource
//gives source code for a shader group
gx_shadergroupT* gx_shader_source(char* vsource, char* fsource);


//Zdef proc gx_set_basic_shader DefaultShader

void gx_set_basic_shader(char* vsource, char* fsource);





typedef struct gx_shader_s {
	zlistnodeT zlistnode;


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
}gfx_shaderT;


typedef gfx_shaderT gx_shader_variantT;

//get a particular version of a shader given some spec
gx_shader_variantT* gx_shader_variant(gx_shadergroupT* sg, char* key, gfx_styleT* st, gfx_vertex_bufferT* vb);


//vsource = vertex shader
//fsource = fragment shader
gfx_shaderT* gx_compile_shader(char* vsource, char* fsource, zvecT* gfx_shader_inputs, int flags);

typedef struct shader_inputT {
	char*	name;
	int		count;		//1 for single, >1 for array of values
	int		type;		// such as GFX_FLOAT
	int		loc;
	zbool	uniform;	//true if uniform (false for attributes)
	//zbool   writable;	//true if will be written by compute shader
} gfx_shader_inputT;

int gfx_sizeof(int type);
void gfx_set_input(gfx_shader_inputT* input, void* data);
void gx_use_shader(gfx_shaderT* shader);

#endif
