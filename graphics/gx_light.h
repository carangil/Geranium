
typedef enum
{
	gx_light_invalid = 0,
	gx_light_point,
	gx_light_directional
} gx_light_e;


typedef struct gx_light_s
{
	gx_light_e light_type;
	vec3 color;
	vec3 ambient;
	vec3 position;

	zbool attenuated;
	float constant;
	float linear;
	float squared;
	
} gx_light_t;


typedef struct gx_environment_s{
	
	zbool usefog;
	vec3 fogcolor;
	float fogstartz;
	float fogendz;
	zvec_t lights;  //gx_light_t*
	
	//TODO: perhaps other textures can go here 'global' textures
	
} gx_environment_t;



gx_light_t* gx_light_mk(gx_light_e type, vec3* position, vec3* color, vec3* ambient);

void gx_light_set_attenuation( gx_light_t* l, zbool attenuated, float maximum, float unityrange, float falloff);



void gxi_set_shader_env_params(gx_shader_t* set);

void gx_set_environment(gx_environment_t* env);

gx_environment_t* gx_environment_mk();


void gx_debug_show_light(gx_light_t* light, zfloat32 size);



//temporary disable lights

void gx_light_tmp_off();

//restore lights to state before gx_light_restore
void gx_light_restore();
