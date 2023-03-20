//These 3 are 'built in'
uniform mat4 gfx_projection;
uniform mat4 gfx_modelview;
uniform vec3 gfx_camera_pos;
uniform vec3 gfx_ambient_sum;


/*********** UNIFORM INPUTS ***************/

#ifdef enable_light_direction0
	uniform vec3 light_color0;
	uniform vec3 light_direction0;
#endif

//material specular color
#ifdef enable_specular0
	uniform vec3 specular0;
#endif

#ifdef enable_specular_exponent0
	uniform float specular_exponent0;
#endif

/*********** VERTEX INPUTS ***************/
//These should match the vertex buffer attribute names

attribute vec3 position;
attribute vec4 color;
attribute vec2 texcoord;
attribute vec3 normal;

/*********** OUTPUTS ******************/
//interpolated outputs to the pixel shader
varying vec4 F_color;
varying vec4 F_specular_color;

#ifdef enable_texcoord
	varying vec2 F_texcoord;
#endif

/*************************/

void main()
{

	vec3 point_to_light; 
	float attenuation;
	
	//start colors
	F_color = vec4(0,0,0,1);
	F_specular_color = vec4(0,0,0,0);
	
	vec3 ambient = vec3(1,1,1); //starting ambient color is white (unless at least one light is turned on)

	// Transform vertex and normal
	vec3 vertex_camspace = vec3( gfx_modelview * vec4(position, 1.0) );

	#ifdef enable_normal
		vec3 normal_camspace = normalize(vec3( gfx_modelview * vec4(normal, 0.0)));  
	#endif

	#ifndef 	 enable_specular_exponent0
		float specular_exponent0 = 10.0;
	#endif

	//light 0

	#ifdef enable_light_direction0
		point_to_light = normalize(light_direction0);
		attenuation = 1; 
		
	#endif
	//todo positional light

	#if   defined(enable_light_direction0) || defined(enable_light_position0)
	
		ambient = gfx_ambient_sum;	//use the pased in ambient light value as the base

		//default white light
		#ifndef enable_light_color0
			vec3 light_color0 = vec3(1,1,1);
		#endif



		//reflection based: (phong)
	//	vec3 reflection = reflect( point_to_light , normal_camspace );
		//float specularIntens = pow(dot(normalize(vertex_camspace), reflection),  specular_exponent0);
	
	
		//half-angle (blinn phong)
		vec3 halfa = normalize( point_to_light - vec3(0,0,-1) ); 
		float specularIntens = pow(  clamp(dot(halfa, normal_camspace),0,1),  specular_exponent0);
		
		
		F_specular_color += clamp(specularIntens, 0, 1)  * vec4(light_color0,1) * attenuation;
			
		//add each light's diffuse component
		F_color += vec4( clamp(dot(normal_camspace,   point_to_light ),0.0,1.0) *  light_color0  ,1);
	#endif
	


	F_color += vec4(ambient, 0);	//add in ambient light : THis is either white if no lighting, or a value passed in from a uniform


	#ifdef enable_color
		F_color = F_color * color;
		F_color.w = color.w;	//copy transparency from the input color, if there was one
	#endif

	#ifdef enable_specular0
		F_specular_color *= vec4(specular0, 1);

	#endif
	
	#ifdef enable_texcoord
		F_texcoord=texcoord;
	#endif

	//projection matrix
	gl_Position = gfx_projection * vec4(vertex_camspace, 1.0) ;

}
