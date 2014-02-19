

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

	int use_constant_alpha; // true to use below alpha value (if false, alpha value will
	zfloat32 alpha;  //alpha value to use for blending


	//specify textures to draw with
	
	//zuint32 numtextures;
	//gx_image_t** textures;  /*can allocate more than 1 */
	vec_t textures;

	//put any other parameters here too!

} gx_drawstyle_t;


//make a drawstyle, with optional 1st texture
gx_drawstyle_t* gx_drawstyle_mk(gx_image_t* img);


void gx_drawstyle_activate(gx_drawstyle_t* style);
