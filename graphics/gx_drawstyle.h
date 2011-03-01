

typedef enum
{
	gx_blend_nothing = 0,
	gx_blend_alpha,
	gx_blend_add,
	gx_blend_multiply
} gx_blending_e;


typedef struct
{

	//specify textures to draw with
	gx_image_t** textures;  
	zuint32 numtextures;

	//use any kind of blending?
	gx_blending_e blending;


	//this controls specular reflections:
	vec3 specular_color;  //diffuse material color come from texture; specular comes from here
	zfloat32 specular_exponent; //shininess


	//put any other parameters here too!


} gx_drawstyle_t;


void gx_drawstyle_activate(gx_drawstyle_t* style);