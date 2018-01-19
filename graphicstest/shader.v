#version 120

#define GX_MAX_LIGHT

uniform mat4 gx_modelview;
uniform vec3 gx_camera_pos;


uniform vec3 gx_specular_color;
uniform float gx_specular_exponent;
uniform vec3 gx_ambient_light;

//per light
uniform vec3 gx_light0_color;
uniform vec3 gx_light0_pos_camspace;


//per vertex
attribute vec4 gx_color;
attribute vec2 gx_texcoord;
attribute vec3 gx_vertex;
attribute vec3 gx_normal;

/* outputs to fragment. these are interpolated */
varying vec4 F_DiffuseColor;
varying vec4 F_SpecularColor;
varying vec2 F_Texcoord;


void main()
{
	//texcoords unmodified
	F_Texcoord = gx_texcoord;  //bydefault, we use a single texture coordinate pair, for all textures. 
	
	//start with ambient diffuse
	F_DiffuseColor = vec4(gx_ambient_light, 1.0);
	
	//and no specular
	F_SpecularColor = vec4(0,0,0,0);

	
	//transform normal and vertex
	//technicaly normal needs a different matrix, but if we aren't sheering or non-uniform scaling, who cares
	//also technically a uniform scale is incorrect, but normalize fixes that
		
	vec3 normal_camspace = normalize(vec3( gx_modelview * vec4(gx_normal, 0.0)));  
	vec3 vertex_camspace = vec3( gx_modelview * vec4(gx_vertex, 1.0) );
		
	
	
	//per light specular :
	
	vec3 point_to_light = normalize( gx_light0_pos_camspace - vertex_camspace );
	
	//reflection based: (phong)
	//vec3 reflection = reflect( point_to_light , normal_camspace );
	//float specularIntens = pow(dot(normalize(vertex_camspace), reflection),  gx_specular_exponent);
	
	//half-angle (blinn phong)
	vec3 halfa = normalize( point_to_light - vec3(0,0,-1) ); 
	float specularIntens = pow(  clamp(dot(halfa, normal_camspace),0,1),  gx_specular_exponent);
	F_SpecularColor += clamp(specularIntens, 0, 1)  * vec4(gx_light0_color,1);
	
	
	//scale specular color by the material specular color
	F_SpecularColor*= vec4(gx_specular_color,1);
	
	
	
	//add each light's component
	F_DiffuseColor += clamp( dot(normal_camspace, point_to_light), 0.0, 1.0) * vec4(gx_light0_color,1.0) ;
		
	//multiply by gx_color, if we want
	//F_DiffuseColor *= gx_color;
	
	gl_Position = gl_ProjectionMatrix * vec4(vertex_camspace, 1.0) ;
	

}
