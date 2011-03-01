
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
} gx_light_t;


void gx_set_active_lights(gx_light_t** lights, zuint32 count);
gx_light_t* gx_light_mk(gx_light_e type, vec3* position, vec3* color, vec3* ambient);

void gx_debug_show_light(gx_light_t* light, zfloat32 size);