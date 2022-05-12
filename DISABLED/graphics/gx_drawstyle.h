
#define GX_MAXLIGHTS 4
typedef struct gx_shader_s {
	zuint32 v_shader;
	zuint32 f_shader;
	zuint32 program;	
	
	zuint32 enabled_texture_uniforms; //how many texture unit uniforms have been set
	
	//vertex attribute locations
	zuint32 vertex_loc;
	zuint32 color_loc;  //attribute location for color
	zuint32 normal_loc;
	zuint32 texcoord_loc[GX_MAX_TEXCOORD];  //attribute location for texcoord
	
	
	//uniform locations
	zuint32 modelview_uloc;
	zuint32 projection_uloc;
	zuint32 camera_pos_uloc;
	zuint32 specular_color_uloc;
	zuint32 specular_exponent_uloc;
	zuint32 ambient_light_uloc;

	zuint32 light_color_uloc[GX_MAXLIGHTS];
	zuint32 light_pos_camspace_uloc[GX_MAXLIGHTS];
	zuint32 light_atten_const_uloc[GX_MAXLIGHTS];
	zuint32 light_atten_linear_uloc[GX_MAXLIGHTS];
	zuint32 light_atten_squared_uloc[GX_MAXLIGHTS];
	
	
	zuint32 matrix_version;  //to prevent unnecessary gluniform loads

	
	
	
} gx_shader_t; 


typedef struct gxi_shader_variant_s{
	zlistnode_t zlistnode;
	char* spec;
	gx_shader_t* shader;  //actual compiled shader
} gxi_shader_variant_t;

typedef struct shadergroup_s {
	char* vsource;
	char* fsource;
	zlist_t variants;
} gx_shadergroup_t;

//gives source code for a shader group
gx_shadergroup_t* gx_shader_source(char* vsource, char* fsource);

//get a particular version of a shader given some spec
gx_shader_t* gx_shader_variant(gx_shadergroup_t* sg, char* spec  );


	
typedef enum
{
	gx_blend_nothing = 0,
	gx_blend_alpha,
	gx_blend_add,
	gx_blend_multiply
} gx_blending_e;


typedef struct
{
	//use any kind of blending?
	gx_blending_e blending;

	//this controls specular reflections:
	vec3 specular_color;  //diffuse material color come from texture; specular comes from here
	zfloat32 specular_exponent; //shininess

	//int use_constant_alpha; // true to use below alpha value (if false, alpha value will
//	zfloat32 alpha;  //alpha value to use for blending

	//specify textures to draw with
	zvec_t textures;

	//put any other parameters here too!
	//gx_shader_t* shader;
	
	gx_shadergroup_t * shadergroup;  //shadergroup for the drawstyle
	
	char* spec;  //spec for drawstyle options
	char* name;  //optional name
	//gx_shader_t *shader;  //go awasy
} gx_drawstyle_t;


//make a drawstyle, with optional 1st texture
gx_drawstyle_t* gx_drawstyle_mk(char* name, gx_image_t* img);

void gx_drawstyle_activate(gx_drawstyle_t* style);

//gx_shader_t* gxi_active_shader();
//void gxi_bind_drawstyle(gx_vbuffer_t* vb);

gx_shader_t* gxi_select_shader(gx_vbuffer_t*);

gx_shader_t* gx_shader_mk(char* vsource, char* psource) ;


//load obj mtl files

zvec_t*  gx_drawstyle_load_mtl(zvec_t* materials, char* filename, char* prefix) ;


