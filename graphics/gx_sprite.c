// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>

#include "gl/glew.h"
#include "gl/wglew.h"
#include "gl/freeglut.h"

#include "gx_image.h"
#include "gx_sprite.h"


gx_sprite_t* gx_sprite_mk(gx_image_t* image, zint32 left, zint32 bottom, zint32 width, zint32 height, zfloat32 sprite_width, zfloat32 sprite_height)
{
	gx_sprite_t* sprite = NULL;

	if (!image)
		return NULL; //need an image to make a sprite from!

	sprite = ram_alloc(sizeof(gx_sprite_t), NULL);

	if (!sprite)
		return NULL;

	
	sprite->image = image;
	sprite->_tx = ((zfloat32) left) / ((zfloat32) image->width-1);
	sprite->_ty = ((zfloat32) bottom) / ((zfloat32) image->height-1);

	sprite->_tx2 = ((zfloat32) (left+width)) / ((zfloat32) image->width-1);
	sprite->_ty2 = ((zfloat32) (bottom+height)) / ((zfloat32) image->height-1);

	sprite->_sprite_height = sprite_height;
	sprite->_sprite_width = sprite_width;

	return sprite;

}




//crappy sprite renderer
void gx_sprite_draw(gx_sprite_t* sprite, zfloat32 x, zfloat32 y, zbool alpha_blend)
{

	if (!sprite || ! sprite->image)
		return;

	if (!sprite->image->_sent_to_gl)
	{
		printf(" Trying to draw a sprite from in image that hasn't been sent to the graphics card!\n");
		//todo:  just sent it?
		return;
	}
	
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->image->_gl_texture_number);
	
	if (alpha_blend)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glBegin(GL_QUADS);
	
		glTexCoord2f(sprite->_tx, sprite->_ty );
		glVertex2f(x,y);

		glTexCoord2f(sprite->_tx2, sprite->_ty );
		glVertex2f(x+sprite->_sprite_width, y);

		glTexCoord2f(sprite->_tx2, sprite->_ty2 );
		glVertex2f(x+sprite->_sprite_width, y + sprite->_sprite_height );

		glTexCoord2f(sprite->_tx, sprite->_ty2 );
		glVertex2f(x , y + sprite->_sprite_height );

	glEnd();
	
}