//These are 'built in'
uniform mat4 gfx_projection;
uniform mat4 gfx_modelview;
uniform vec3 gfx_camera_pos;

/*********** UNIFORM INPUTS ***************/





/*********** VERTEX INPUTS ***************/
//These should match the vertex buffer attribute names

attribute vec3 position;
attribute vec4 color;
attribute vec2 texcoord;
attribute vec3 normal;

/*********** OUTPUTS ******************/
//interpolated outputs to the pixel shader

#ifdef enable_texcoord
	varying vec2 F_texcoord;
#endif

#ifdef enable_color
	varying vec4 F_vertex_color;
#endif

varying vec3 vertex_camspace;
varying vec3 normal_camspace;

/*************************/

void main()
{

	vec3 point_to_light; 
	float attenuation;
	float out_distance;


	// Transform vertex and normal
	//vec3 
	vertex_camspace = vec3( gfx_modelview * vec4(position, 1.0) );
	
	#ifdef enable_normal
		normal_camspace = normalize(vec3( gfx_modelview * vec4(normal, 0.0)));  
	#endif




	#ifdef enable_color
		F_vertex_color = color;
	#endif
		
	
	#ifdef enable_texcoord
		F_texcoord=texcoord;
	#endif

	//projection matrix
	gl_Position = gfx_projection * vec4(vertex_camspace, 1.0) ;

	

}
