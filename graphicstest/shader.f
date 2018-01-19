uniform sampler2D gx_texture0;
uniform sampler2D gx_texture1;

varying vec4 F_DiffuseColor;
varying vec4 F_SpecularColor;

varying vec2 F_Texcoord;

void main()
{

	
	
	gl_FragColor = F_DiffuseColor + F_SpecularColor ;

}



