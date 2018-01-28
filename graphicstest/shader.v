

uniform mat4 gx_projection;
uniform mat4 gx_modelview;
uniform vec3 gx_camera_pos;

uniform vec3 gx_specular_color;
uniform float gx_specular_exponent;
uniform vec3 gx_ambient_light;

//per light

#ifdef GX_LIGHT0
uniform vec3 gx_light0_color;
uniform vec3 gx_light0_pos_camspace;
uniform float gx_light0_atten_const;
uniform float gx_light0_atten_linear;
uniform float gx_light0_atten_squared;
#endif

#ifdef GX_LIGHT1
uniform vec3 gx_light1_color;
uniform vec3 gx_light1_pos_camspace;
uniform float gx_light1_atten_const;
uniform float gx_light1_atten_linear;
uniform float gx_light1_atten_squared;
#endif





//per vertex
attribute vec4 gx_color;
attribute vec2 gx_texcoord0;
attribute vec3 gx_vertex;
attribute vec3 gx_normal;

/* outputs to fragment. these are interpolated */
varying vec4 F_DiffuseColor;
varying vec4 F_SpecularColor;
varying vec2 F_Texcoord;


void main()
{
	vec3 point_to_light;
	float attenuation;

	#ifdef GX_TEXCOORD0
		F_Texcoord = gx_texcoord0;  //bydefault, we use a single texture coordinate pair, for all textures. 
	#endif
	
	//start with ambient diffuse
	F_DiffuseColor = vec4(gx_ambient_light, 1.0) ;
	
	//and no specular
	F_SpecularColor = vec4(0,0,0,0);
	
	//transform normal and vertex
	//technicaly normal needs a different matrix, but if we aren't sheering or non-uniform scaling, who cares
	//also technically a uniform scale is incorrect, but normalize fixes that
		
	//transform normal if needed
	
	#ifdef GX_NORMAL
		vec3 normal_camspace = normalize(vec3( gx_modelview * vec4(gx_normal, 0.0)));  
	#endif
	
	//transform vertex
	vec3 vertex_camspace = vec3( gx_modelview * vec4(gx_vertex, 1.0) );
		
	
	//positional light attenuation and direction
	#ifdef GX_LIGHT0POS
		point_to_light= gx_light0_pos_camspace - vertex_camspace;
		float d = length(point_to_light);
		point_to_light = point_to_light / d;  //normally you'd use built-in 'normalize' but we need 'd' anyway.
		attenuation = 1/ ( gx_light0_atten_const + d * gx_light0_atten_linear + d*d*gx_light0_atten_squared);
	#endif
	
	//directional light attenuation and direction
	#ifdef GX_LIGHT0DIR
		point_to_light = normalize(gx_light0_pos_camspace);
		attenuation= 1.0;
	#endif
	

	#ifdef GX_LIGHT0
		//reflection based: (phong)
		//vec3 reflection = reflect( point_to_light , normal_camspace );
		//float specularIntens = pow(dot(normalize(vertex_camspace), reflection),  gx_specular_exponent);
		
		//half-angle (blinn phong)
		vec3 halfa = normalize( point_to_light - vec3(0,0,-1) ); 
		float specularIntens = pow(  clamp(dot(halfa, normal_camspace),0,1),  gx_specular_exponent);
		F_SpecularColor += clamp(specularIntens, 0, 1)  * vec4(gx_light0_color,1) * attenuation;
			
		//add each light's diffuse component
		F_DiffuseColor += clamp( dot(normal_camspace, point_to_light), 0.0, 1.0) * vec4(gx_light0_color,1.0) * attenuation ;
	#endif
	
	//additional lights
	#ifdef GX_LIGHT1POS
		point_to_light= gx_light1_pos_camspace - vertex_camspace;
		float d = length(point_to_light);
		point_to_light = point_to_light / d;  //normally you'd use built-in 'normalize' but we need 'd' anyway.
		attenuation = 1/ ( gx_light1_atten_const + d * gx_light1_atten_linear + d*d*gx_light1_atten_squared); 
	#endif
	
	//directional light attenuation and direction
	#ifdef GX_LIGHT1DIR
		point_to_light = normalize(gx_light1_pos_camspace);
		attenuation= 1.0;
	#endif
	

	#ifdef GX_LIGHT1

		//half-angle (blinn phong)
		halfa = normalize( point_to_light - vec3(0,0,-1) ); 
		specularIntens = pow(  clamp(dot(halfa, normal_camspace),0,1),  gx_specular_exponent);
		F_SpecularColor += clamp(specularIntens, 0, 1)  * vec4(gx_light1_color,1) * attenuation;
			
		//add each light's diffuse component
		F_DiffuseColor += clamp( dot(normal_camspace, point_to_light), 0.0, 1.0) * vec4(gx_light1_color,1.0) * attenuation ;
	#endif

		
	//scale specular color by the material specular color
	F_SpecularColor*= vec4(gx_specular_color,1);


	//scale diffuse color by the passed in diffuse color (if we have one)
	//fixed function opengl clamps maximum illumination to 1.0 times the texture color
	F_DiffuseColor = clamp(F_DiffuseColor , 0, 1);

	#ifdef GX_COLOR
		F_DiffuseColor *= gx_color;
	#endif



	gl_Position = gx_projection * vec4(vertex_camspace, 1.0) ;

}
