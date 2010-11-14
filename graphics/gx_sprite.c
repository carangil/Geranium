// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>

#include "glstuff.h"

#include "gx_image.h"
#include "gx_sprite.h"



gx_sprite_t* gx_sprite_mk(gx_image_t* image, zint32 left, zint32 bottom, zint32 width, zint32 height, zfloat32 sprite_width, zfloat32 sprite_height)
{
	gx_sprite_t* sprite = NULL;

	if (!image)
		return NULL; //need an image to make a sprite from!

	sprite = ram_alloc(sizeof(gx_sprite_t), NULL);  //no special destructor needed (sprites don't free their images)

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

#if 0
	if (!sprite->image->_sent_to_gl)
	{
		printf(" Trying to draw a sprite from in image that hasn't been sent to the graphics card!\n");
		//todo:  just sent it?
		return;
	}
#endif
	
	glColor4f(1,1,1,1);

	//sprite renderer must go through the 'texture mananger'
	//_gx_image_enable( sprite->image);
	gx_set_active_textures(& (sprite->image) , 1);
 
	
	
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


void gx_sprite_draw_rotozoom(gx_sprite_t* sprite, zfloat32 x, zfloat32 y, zbool alpha_blend,  zfloat32 xo, zfloat32 yo, zfloat32 zoom, zfloat32 ang)
{


	if (!sprite || ! sprite->image)
		return;

	
	glColor4f(1,1,1,1);

	//sprite renderer must go through the 'texture mananger'
	//_gx_image_enable( sprite->image);
	gx_set_active_textures(& (sprite->image) , 1);
 

	if (alpha_blend)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glPushMatrix();

	glTranslatef(x,y,0);
	glRotatef(ang, 0,0,1);
	glScalef(zoom,zoom,1);
	glTranslatef(-xo,-yo,0);
	


	glBegin(GL_QUADS);
	
		glTexCoord2f(sprite->_tx, sprite->_ty );
		glVertex2f(0,0);

		glTexCoord2f(sprite->_tx2, sprite->_ty );
		glVertex2f(sprite->_sprite_width, 0);

		glTexCoord2f(sprite->_tx2, sprite->_ty2 );
		glVertex2f(sprite->_sprite_width,  sprite->_sprite_height );

		glTexCoord2f(sprite->_tx, sprite->_ty2 );
		glVertex2f(0 ,  sprite->_sprite_height );

	glEnd();


	glPopMatrix();

}