uniform vec3 gfx_ambient_sum;


uniform sampler2D texture_diffuse0;
varying vec4 F_vertex_color;
varying vec2 F_texcoord;
varying float F_zcoord;

varying vec3  vertex_camspace;
varying vec3 normal_camspace;

#ifdef enable_fog_density0
	uniform float fog_density0;
	uniform vec3 fog_color0;
#endif


//Material properties 
#ifdef enable_specular0
	uniform vec3 specular0;
#endif

#ifdef enable_specular_exponent0
	uniform float specular_exponent0;
#else
	#define specular_exponent0 10.0
#endif

//Per-light properties

#ifdef enable_light_direction0
	uniform vec3 light_direction0;
#endif

#ifdef enable_light_position0
	uniform vec3 light_position0;
#endif

#ifdef enable_light_color0
	uniform vec3 light_color0;
#else
	#define light_color0 vec3(1.0, 1.0, 1.0)
#endif

#ifdef enable_light_attenuation0
	uniform vec3 light_attenuation0;
#endif

vec4 F_color =vec4(0,0,0,1);
vec4 F_specular_color = vec4(0,0,0,0);

/*Light functions */
	
/* Find direction to light source*/
vec3 light_point( vec3 vertex_camspace, vec3 light_positionX,  out float d){
	vec3 point_to_light= light_positionX - vertex_camspace;
	d = length(point_to_light);
	point_to_light = point_to_light / d;  //normally you'd use built-in 'normalize' but we need 'd' anyway if we do attenuation
	return point_to_light;
}

 /* Attenuate brightness of light based on distance */
float light_attenuationf(vec3 light_attenuationX, float d){
	#define ATTENUATION_CONSTANT	x
	#define ATTENUATION_LINEAR		y
	#define ATTENUATION_SQUARED		z

	return  1/ 
			(        light_attenuationX.ATTENUATION_CONSTANT + 
			  d *    light_attenuationX.ATTENUATION_LINEAR + 
			  d *d * light_attenuationX.ATTENUATION_SQUARED
			);

}

 /* Shade & specular highlight according to light and surface angle */
void light_shading(vec3 normal_camspace, vec3 point_to_light, vec3 light_colorX, float attenuation){

	//reflection based: (phong)
	//	vec3 reflection = reflect( point_to_light , normal_camspace );
	//float specularIntens = pow(dot(normalize(vertex_camspace), reflection),  specular_exponent0);
	
	
	//half-angle (blinn phong)
	vec3 halfa = normalize( point_to_light - vec3(0,0,-1) ); 
	float specular_bright = pow(  clamp(dot(halfa, normal_camspace),0,1),  specular_exponent0);
		
	F_specular_color += clamp(specular_bright, 0, 1)  * vec4(light_colorX,1) * attenuation;
			
	//add each light's diffuse component
	F_color +=  attenuation*vec4( clamp(dot(normal_camspace,   point_to_light ),0.0,1.0) *  light_colorX  ,1);

}





void main()
{
	float attenuation;
	float out_distance;
		
	vec3 normal_camspace = normalize(normal_camspace);

	//light 0
	#if   defined(enable_light_direction0) || defined(enable_light_position0)
		attenuation = 1.0;

		#ifdef enable_light_position0

			vec3  light_direction0 = light_point( vertex_camspace, light_position0, out_distance);	

			#ifdef enable_light_attenuation0 
				attenuation = light_attenuationf(light_attenuation0, out_distance);
			#endif

		#endif

		light_shading(normal_camspace, normalize(light_direction0), light_color0,  attenuation); //adds to F_color and F_specular_color

	#endif


	


	
	//ambient and color scaling
	/* Add ambient light to vertex's light contribution */

	F_color += vec4(gfx_ambient_sum, 0);	//add in ambient light: This is either the num of all ambient light OR is white 1,1,1 for no lighting


	/* Filter diffuse light by vertex color (if exist) */
	#ifdef enable_color
		F_color = F_color * F_vertex_color;
		F_color.w = F_vertex_color.w;	//copy transparency from the input color, if there was one
	#endif

	/*Filter specular light by specular material color */
	#ifdef enable_specular0
		F_specular_color *= vec4(specular0, 1);
	#endif

	//texture filters the diffuse color

	#ifdef enable_texture_diffuse0
		F_color *= texture2D(texture_diffuse0, vec2(F_texcoord) );
	#endif










	
	gl_FragColor = F_color + F_specular_color;

	#ifdef enable_fog_density0
		float foggy = 1-clamp(exp(F_zcoord*fog_density0), 0, 1);  //note normally is exp(-distance*denstity), but Z coordinate is already negative
		gl_FragColor = mix(gl_FragColor, vec4(fog_color0, 1), foggy);

	#endif

}



