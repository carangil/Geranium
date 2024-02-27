
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





typedef struct gx_shader_variant_s {
	zlistnodeT zlistnode;

	zuint32 v_shader;
	zuint32 f_shader;
	zuint32 program;

	//zuint32 enabled_texture_uniforms; //how many texture unit uniforms have been set

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

#endif