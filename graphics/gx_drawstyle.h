
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
	zuint32 camera_pos_uloc;
	
	
	zuint32 matrix_version;  //to prevent unnecessary gluniform loads

	
} gx_shaderset_t;

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
	gx_shaderset_t* shaderset;
	
} gx_drawstyle_t;


//make a drawstyle, with optional 1st texture
gx_drawstyle_t* gx_drawstyle_mk(gx_image_t* img);

void gx_drawstyle_activate(gx_drawstyle_t* style);

gx_shaderset_t* gxi_active_shaderset();


gx_shaderset_t* gx_shaderset_mk(char* vsource, char* psource) ;

