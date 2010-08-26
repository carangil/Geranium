// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


//this file is for loading  (and maybe saving) image files

#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>
#include "gx_image.h"


//#include <stdio.h>
//#include <windows.h>

//#include "gl\glext.h"
//#include "../ztypes.h"

#include "gl/glew.h"
#include "gl/wglew.h"
#include "gl/freeglut.h"

//#include <gl/GL.h>

//#include "gx_sys.h"

void _gx_destruct_image(void* x)
{
	gx_image_t* i = x;
	if  (i->data)
		ram_free(i->data);

	ram_shallow_free(i);
}

//Taken from 
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

	printf(" %d by %d at %d bpp \n", image->width, image->height, image->bpp );

	if (  (image->width > 2048) || (image->height > 2048))
	{
		printf(" Probably wrong image format\n");
	}

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

zbool gx_image_enable(gx_image_t* image)
{
	if (!image)
		return zfalse;

	//if image is already loaded to opengl, say we've succeeded
	if (image->_sent_to_gl)
		return ztrue;


	//attempt to create texture object in GL
	glGenTextures(1, &(image->_gl_texture_number) );
	
	glBindTexture (GL_TEXTURE_2D,  image->_gl_texture_number );
	
	if (image->bpp == 3)
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->data);
	else if (image->bpp == 4)
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
	else if (image->bpp ==1)
		gluBuild2DMipmaps(GL_TEXTURE_2D, 1, image->width, image->height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->data);
	else
		printf(" unsupported texture format\n");

	image->_sent_to_gl = ztrue;

	return ztrue;
}

zbool gx_image_disable(gx_image_t* image)
{
	//TODO: release the texture from OPENGL
}






//garbage test function
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