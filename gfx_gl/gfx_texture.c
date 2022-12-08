// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2013 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.





#include <stdio.h>

#define GFXINTERNAL
#include "gfx_gl.h"




gfx_textureT* gfx_texture_mk(zbitmapT* bmp) {

	gfx_textureT* tx = ram_alloc(sizeof(gfx_textureT), NULL);

	tx->bitmap = ram_addref(bmp);
	tx->_scaler = GFX_TEXTURE_SCALER_SMOOTH;

	return tx;
}

void gfx_texture_scaler(gfx_textureT* image, int scaler)
{
	//transfers the image to hardware accelerating rendering
	image->_scaler = scaler;
	image->_sent_scaler = 0;
}



//enables an image for use in rendering (sends it to opengl for use in sprites or texture mapping)
zbool gxi_texture_enable(gfx_textureT* image)
{
	zbool tosend = ZFALSE;

	if (!image)
		return ZFALSE;

	if (!image->_gl_texture_number)
	{
		//attempt to create texture object in GL
		glGenTextures(1, &(image->_gl_texture_number));
		//gxi_gl_textures_gen++;
		tosend = ZTRUE;
	}

	//bind the texture for the current texture unit
	glBindTexture(GL_TEXTURE_2D, image->_gl_texture_number);



	if (tosend)
	{
		//send the texture to opengl if it hasn't been already
		int a = zbitmap_pxsize(image->bitmap);

			printf(" format %x %x    %dby%d\n", image->bitmap->format, a, image->bitmap->w , image->bitmap->h);

		switch (a) {

		case 3:


			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->bitmap->w, image->bitmap->h, GL_RGB, GL_UNSIGNED_BYTE, image->bitmap->data);
			break;

		case 4:

			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, image->bitmap->w, image->bitmap->h, GL_RGBA, GL_UNSIGNED_BYTE, image->bitmap->data);
			break;

		case 1:

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_INTENSITY, image->bitmap->w, image->bitmap->h, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->bitmap->data);
			break;
		default:
			printf(" unsupported texture format 0x%x\n", a);
		}



		//mess with scaler
		if (!image->_sent_scaler)
		{

			image->_sent_scaler = ZTRUE;

			if (image->_scaler == GFX_TEXTURE_SCALER_BLOCKY)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			else if (image->_scaler == GFX_TEXTURE_SCALER_SMOOTH)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

		}

	}

	glEnable(GL_TEXTURE_2D);

	return ZTRUE;
	
}


//offset is to 'stack' up multiple sets of textures
int gxi_enabled_texture_units = 0;

void gxi_texture_set_enable(zvecT* textures) {

	zuint32 i;
	int newcount = 0;

	//enable and set each texture
	if (textures != NULL){

		for (i = 0; i < zvec_count(textures); i++) {
			glActiveTexture(GL_TEXTURE0 + i);  //set texture unit
			glEnable(GL_TEXTURE_2D);

				//todo: only do this if using fixed function
				if (i == 0)
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //multiply againt light value
				else
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //default as alpha blending

				gxi_texture_enable(zvec_get_at(textures, i));
			
		}
		newcount = i;
	}
	//disable remaining textures
	for (; i < gxi_enabled_texture_units; i++) {

		glActiveTexture(GL_TEXTURE0 + i);
		glDisable(GL_TEXTURE_2D);

	}

	gxi_enabled_texture_units = newcount;
}

