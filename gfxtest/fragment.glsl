
uniform sampler2D texture_diffuse0;

varying vec2 F_texcoord;
varying vec4 F_color;
varying vec4 F_specular_color;
varying float F_zcoord;

#ifdef enable_fog_density0
	uniform float fog_density0;
	uniform vec3 fog_color0;
#endif


void main()
{
	vec4 diffuse_color = F_color;
	vec4 specular_color = vec4(F_specular_color.xyz, 0.0);


	#ifdef enable_texture_diffuse0
		diffuse_color *= texture2D(texture_diffuse0, vec2(F_texcoord) );
	#endif

	
	
	


	
	gl_FragColor = diffuse_color +specular_color;

	#ifdef enable_fog_density0
		float foggy = 1-clamp(exp(F_zcoord*fog_density0), 0, 1);  //note normally is exp(-distance*denstity), but Z coordinate is already negative
		gl_FragColor = mix(gl_FragColor, vec4(fog_color0, 1), foggy);

	#endif

}



