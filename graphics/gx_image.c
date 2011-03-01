// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.




#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>
#include "gx_image.h"
#include "glstuff.h"



void _gx_destruct_image(void* x)
{
	gx_image_t* i = x;
	if  (i->data)
		ram_free(i->data);

	//need to free it from openGL as well

	if (i->_sent_to_gl)
	{
		glDeleteTextures(1, & (i->_gl_texture_number) );
	}

	ram_shallow_free(i);
}

//Taken from 2005
gx_image_t* gx_image_load_tga( zchar* f)
{
	gx_image_t* image = NULL;
	zbyte buf[6];
	zuint32 i=0;	

	FILE* fi =fopen(f,"r+b");

	if (!fi)
		return NULL;

	image = ram_alloc(sizeof(gx_image_t), _gx_destruct_image);

	if (!image)
		return NULL;

	fseek(fi, 12, SEEK_SET);

	fread( buf  ,6, 1 , fi);
	
	image->width = buf[0] | (buf[1]<<8);
	image->height = buf[2] | ( buf[3]<<8);
	image->bpp = buf[4] / 8 ;    //we want bytes per pixel, not bits

	if ( (image->bpp==0) || (image->bpp == 2) || (image->bpp >4))
	{
		//invalid ranges
		ram_free(image);
		return NULL;
	}
#ifdef DOPRINTF 
	printf(" %d by %d at %d bpp \n", image->width, image->height, image->bpp );
#endif

#ifdef DOPRINTF 
	if (  (image->width > 2048) || (image->height > 2048))
	{
		printf(" Probably wrong image format\n");
	}
#endif

	image->data = ram_alloc(image->height * image->width * image->bpp, NULL);

	if (image->data)
	{
		/*read in all data*/
		fread( image->data, 1,image->height*image->width*image->bpp, fi);

	
		if ((image->bpp == 3) || (image->bpp == 4))
		{
			//TARGA goes BLUE GREEN RED  byte order
			for (i=0;i< image->height * image->width * image->bpp ; i+= image->bpp)
			{	
				zbyte t = image->data[i];
				image->data[i] = image->data[i+2];
				image->data[i+2] = t;
			}
		} 

	}
	else
	{
		/*Could not allocate data*/
		ram_free(image);
		return NULL;

	}

	fclose(fi);

	return image;
}


//enables an image for use in rendering (sends it to opengl for use in sprites or texture mapping)
zbool _gx_image_enable(gx_image_t* image)
{
	if (!image)
		return zfalse;

	if (! image->_sent_to_gl)
	{
		//attempt to create texture object in GL
		glGenTextures(1, &(image->_gl_texture_number) );
	}
	
	//bind the texture for the current texture unit
	glBindTexture (GL_TEXTURE_2D,  image->_gl_texture_number );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	if (! image->_sent_to_gl)
	{
		//send the texture to opengl if it hasn't been already

		if (image->bpp == 3)
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->data);
		else if (image->bpp == 4)
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->data);			
		else if (image->bpp == 1)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_INTENSITY, image->width, image->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->data);
#ifdef DOPRINTF 
		else
			printf(" unsupported texture format\n");
#endif

		image->_sent_to_gl = ztrue;
	}

	return ztrue;
}


zbool gx_image_disable(gx_image_t* i)
{
	//TODO: need this function?  Why disable an image in gl and keep the data in ram?

	if (i->_sent_to_gl)
	{
		glDeleteTextures(1, & (i->_gl_texture_number) );
	}

	i->_sent_to_gl=zfalse;
	i->_gl_texture_number=0;
}


//garbage test function: puts an image on the screen
void gx_image_test(gx_image_t* image)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture (GL_TEXTURE_2D,  image->_gl_texture_number);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glColor4f(1,1,1,1);
	glTexCoord2f(0,0);
	glVertex2f(0,0);
	
	glTexCoord2f(1,0);
	glVertex2f(1,0);
	
	glTexCoord2f(1,1);
	glVertex2f(1,1);

	glColor4f(1,1,1,0);
	glTexCoord2f(0,1);
	glVertex2f(0,1);
	
	glEnd();
}



//manage opengl texture unit state
//This turns a set of textures on/off

static zuint32 _gx_texture_enabled_count = 0;  //specified how many texture units have been turned on
void gx_set_active_textures(gx_image_t** texes, zuint32 numtex)
{
	zuint32 i = 0;

	for (i=0;i<numtex;i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);  //set active texture unit
		
		if (i>=_gx_texture_enabled_count)
		{	
			//if we haven't enabled this unit yet, enable it
			glEnable(GL_TEXTURE_2D);

			if (i==0)
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
			else
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //default as alpha blending
		}

		_gx_image_enable(texes[i]); //enable this image for use on the current texture unit
	}

	//disable any texture units we had enabled but don't need anymore
	for (i=numtex;i<_gx_texture_enabled_count;i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glDisable(GL_TEXTURE_2D);
	}

	_gx_texture_enabled_count = numtex;
}