//These are 'built in'
uniform mat4 gfx_projection;
uniform mat4 gfx_modelview;
uniform vec3 gfx_camera_pos;

/*********** UNIFORM INPUTS ***************/





/*********** VERTEX INPUTS ***************/
//These should match the vertex buffer attribute names

in vec3 position;
in vec4 color;
in vec2 texcoord;
in vec3 normal;

/*********** OUTPUTS ******************/
//interpolated outputs to the pixel shader

#ifdef enable_texcoord
	out vec2 F_texcoord;
#endif

#ifdef enable_color
	out vec4 F_vertex_color;
#endif

out vec3 F_vertex_camspace;
out vec3 F_normal_camspace;

/*************************/

void main()
{

	vec3 point_to_light; 
	float attenuation;
	float out_distance;


	// Transform vertex and normal
	//vec3 
	F_vertex_camspace = vec3( gfx_modelview * vec4(position, 1.0) );
	
	#ifdef enable_normal
		F_normal_camspace = normalize(vec3( gfx_modelview * vec4(normal, 0.0)));  
	#endif




	#ifdef enable_color
		F_vertex_color = color;
	#endif
		
	
	#ifdef enable_texcoord
		F_texcoord=texcoord;
	#endif

	//projection matrix
	gl_Position = gfx_projection * vec4(F_vertex_camspace, 1.0) ;

	

}
