
uniform sampler2D gx_texture0;


varying vec4 F_DiffuseColor;
varying vec4 F_SpecularColor;

#ifdef GX_TEXTURE0
varying vec2 F_Texcoord;
#endif

void main()
{

	vec4 DiffuseColor = F_DiffuseColor;

	#ifdef GX_TEXTURE0
		DiffuseColor *= texture2D(gx_texture0, F_Texcoord);
	#endif
	
	gl_FragColor = DiffuseColor + F_SpecularColor ;

}



