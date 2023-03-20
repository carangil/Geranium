
uniform sampler2D texture_diffuse0;

varying vec2 F_texcoord;
varying vec4 F_color;
varying vec4 F_specular_color;

void main()
{
	vec4 diffuse_color = F_color;
	vec4 specular_color = F_specular_color;

	#ifdef enable_texture_diffuse0
		diffuse_color *= texture2D(texture_diffuse0, vec2(F_texcoord) );
	#endif
	
	gl_FragColor = diffuse_color + specular_color;

}



