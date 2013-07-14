// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include "../vmath.h"
#include <stdio.h>

#include "glstuff.h"

#include "gx_image.h"
#include "gx_sprite.h"

zbool _gx_sprite_t_kill(void* x)
{
	gx_sprite_t* sprite = x;
	ram_free(sprite->image);
	return ztrue;
}

gx_sprite_t* gx_sprite_mk(gx_image_t* image, zint32 left, zint32 bottom, zint32 width, zint32 height, zfloat32 sprite_width, zfloat32 sprite_height)
{
	gx_sprite_t* sprite = NULL;

	if (!image)
		return NULL; //need an image to make a sprite from!

	sprite = ram_alloc(sizeof(gx_sprite_t), _gx_sprite_t_kill);  //no special destructor needed (sprites don't free their images)

	if (!sprite)
		return NULL;

	/*
	sprite->_tx = (1+2*left) / (zfloat32) (2*image->width);
	sprite->_ty = (1+2*bottom) / (zfloat32) (2*image->height);

	sprite->_tx2 = (1+2*(left + width) ) / (zfloat32) (2*image->width);
	sprite->_ty2 = (1+2*(bottom + height) ) / (zfloat32) (2*image->width);
	*/

	sprite->_tx = (2*left) / (zfloat32) (2*image->width);
	sprite->_ty = (2*bottom) / (zfloat32) (2*image->height);

	sprite->_tx2 = (2*(left + width) ) / (zfloat32) (2*image->width);
	sprite->_ty2 = (2*(bottom + height) ) / (zfloat32) (2*image->width);

	sprite->_sprite_height = sprite_height;
	sprite->_sprite_width = sprite_width;



	sprite->image = image;
	ram_addref(sprite->image);

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
		#ifdef DOPRINTF 
		printf(" Trying to draw a sprite from in image that hasn't been sent to the graphics card!\n");
		#endif
		//todo:  just sent it?
		return;
	}
#endif
	
	glColor4f(1,1,1,1);

	//sprite renderer must go through the 'texture mananger'
	//_gx_image_enable( sprite->image);
	gx_set_active_textures(& (sprite->image) , 1);
 
	
	//NOTE:  turning on alpha blending here conflicts with any 3d 'drawstyle' thats applied
	//       after drawing the sprite the current drawstyle should be deactivated


	if (alpha_blend)
	{
		glEnable(GL_BLEND);
	
		
		
	}
	else
	{
		glDisable(GL_BLEND);
	}

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

//todo: regular sprite draw and rotozoom function should be merged
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

// simple text

static zfloat32 _gx_fontsize_w = .1;
static zfloat32 _gx_fontsize_h = .1;
static zfloat32 _gx_font_spacing_w = 0;
static zfloat32 _gx_fontr=1;
static zfloat32 _gx_fontg=1;
static zfloat32 _gx_fontb=1;
static zfloat32 _gx_fonta=1;


void gx_text_size(zfloat32 width, zfloat32 height, zfloat32 w_spacing)
{
	_gx_fontsize_w = width;
	_gx_fontsize_h = height;
	_gx_font_spacing_w = w_spacing;
}

void gx_text_color(zfloat32 r,zfloat32 g,zfloat32 b, zfloat32 a)
{
	_gx_fontr=r;
	_gx_fontg=g;
	_gx_fontb=b;
	_gx_fonta=a;

}


void gx_text_draw(gx_image_t* font,  zfloat32 x, zfloat32 y,  zfloat32 angle, zchar* string)
{
	float sx;
	float sy;
	float sx2;
	float sy2;
	int tw;
	int th;
	
	if (!string || !string[0] || !font)
		return;


	gx_set_active_textures( &font, 1);

	glColor4f(_gx_fontr, _gx_fontg,_gx_fontb, _gx_fonta);

	glPushMatrix();

	glTranslatef(x,y,0);
	glRotatef(angle, 0,0,1);
	
	x=0;
	y=0;

	tw = font->width /16;
	th = font->height /16;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBegin(GL_QUADS);

	while(*string)
	{
		unsigned char chr = *string;
	
	/*	sx = (chr % 16 )/16.0 +(1/512.0);
		sy = (chr / 16 )/16.0 +(1/512.0);
		sx2 = (chr % 16+1)/16.0 + (1/512.0);
		sy2 = (chr / 16+1)/16.0 +(1/512.0);
		*/

		
		int sxp = (chr%16) * tw;
		int syp = (chr/16) * th;

#if 0
		sx = (sxp+.5) / 256.0;
		sy = (syp+1.5) / 256.0;

		sx2 = (sxp+15.5) / 256.0;
		sy2 = (syp+15.5) / 256.0;
#else
		sx = (2*sxp) / (float)(font->width *2);
		sy = (2*syp) / (float)(font->height *2);

		sx2 = (2*(sxp+(tw-1)) +1) / (float)(font->width *2);
		sy2 = (2*(syp+(th-1)) +1) / (float)(font->height *2);


#endif
		


		glTexCoord2f( sx, 1-sy2);
		glVertex2f( x,y);

		glTexCoord2f( sx2, 1-sy2);
		glVertex2f( x+_gx_fontsize_w,y);

		
		glTexCoord2f( sx2, 1-sy);
		glVertex2f( x+_gx_fontsize_w,y+_gx_fontsize_h);

		glTexCoord2f( sx, 1-sy);
		glVertex2f( x,y+_gx_fontsize_h);

		x+= _gx_fontsize_w + _gx_font_spacing_w;

		string++;
	}

	
	glEnd();

	glPopMatrix();
}



//need to draw a sprite in 3d
//todo: this functino is crappy, fix it
//crappy sprite renderer
void crap_gx_sprite_draw_3d(gx_sprite_t* sprite, vec3* position, vec3* up, vec3* right , zbool alpha_blend, zbool center)
{

	vec3  sprite_upv;
	vec3  sprite_rightv;

	vec3 p;

	vec3* sprite_up = &sprite_upv;
	vec3* sprite_right = &sprite_rightv;

	vec3 t_right;
	vec3 t_up;

	vec3set (*sprite_up, 0,1,0);
	vec3set (*sprite_right, 0,0,1);

	if (!sprite || ! sprite->image || ! position || ! up || ! right)
		return;

#if 0
	if (!sprite->image->_sent_to_gl)
	{
		#ifdef DOPRINTF 
		printf(" Trying to draw a sprite from in image that hasn't been sent to the graphics card!\n");
		#endif
		//todo:  just sent it?
		return;
	}
#endif
	
	glColor4f(1,1,1,1);

	//sprite renderer must go through the 'texture mananger'
	//_gx_image_enable( sprite->image);
	gx_set_active_textures(& (sprite->image) , 1);
 	
	//NOTE:  turning on alpha blending here conflicts with any 3d 'drawstyle' thats applied
	//       after drawing the sprite the current drawstyle should be deactivated

	

	if (alpha_blend)
	{
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, .05);
	}
	else
	{
		glDisable(GL_BLEND);
	}


	//calculate sprite corner directions

	vec3mov(t_up, *sprite_up);

	vec3mov(t_right, *sprite_right);




	glBegin(GL_QUADS);
	
		glTexCoord2f(sprite->_tx, sprite->_ty );
		vec3mov(p, *position);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, t_right);
			vec3madd(p, -0.5*sprite->_sprite_height, t_up);
		}
		glVertex3fv(&p);

		glTexCoord2f(sprite->_tx2, sprite->_ty );
		vec3mov(p, *position);
		vec3madd(p, sprite->_sprite_width, t_right);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, t_right);
			vec3madd(p, -0.5*sprite->_sprite_height, t_up);
		}
		glVertex3fv(&p);

		glTexCoord2f(sprite->_tx2, sprite->_ty2 );
		vec3mov(p, *position);
		vec3madd(p, sprite->_sprite_height, t_up);
		vec3madd(p, sprite->_sprite_width, t_right);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, t_right);
			vec3madd(p, -0.5*sprite->_sprite_height, t_up);
		}
		glVertex3fv(&p);

		glTexCoord2f(sprite->_tx, sprite->_ty2 );
		vec3mov(p, *position);
		vec3madd(p, sprite->_sprite_height, t_up);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, t_right);
			vec3madd(p, -0.5*sprite->_sprite_height, t_up);
		}
		glVertex3fv(&p);

	glEnd();
}



void gx_sprite_draw_3d(gx_sprite_t* sprite, vec3* position, vec3* up, vec3* right , zbool alpha_blend, zbool center)
{

	vec3 p;

	if (!sprite || ! sprite->image || ! position || ! up || ! right)
		return;

#if 0
	if (!sprite->image->_sent_to_gl)
	{
		#ifdef DOPRINTF 
		printf(" Trying to draw a sprite from in image that hasn't been sent to the graphics card!\n");
		#endif
		//todo:  just sent it?
		return;
	}
#endif
	
	glColor4f(1,1,1,1);

	//sprite renderer must go through the 'texture mananger'
	//_gx_image_enable( sprite->image);
	gx_set_active_textures(& (sprite->image) , 1);
 	
	//NOTE:  turning on alpha blending here conflicts with any 3d 'drawstyle' thats applied
	//       after drawing the sprite the current drawstyle should be deactivated

	

	if (alpha_blend)
	{
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, .05);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	glBegin(GL_QUADS);
	
		glTexCoord2f(sprite->_tx, sprite->_ty );
		vec3mov(p, *position);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, *right);
			vec3madd(p, -0.5*sprite->_sprite_height, *up);
		}
		glVertex3fv(&p);

		glTexCoord2f(sprite->_tx2, sprite->_ty );
		vec3mov(p, *position);
		vec3madd(p, sprite->_sprite_width, *right);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, *right);
			vec3madd(p, -0.5*sprite->_sprite_height, *up);
		}
		glVertex3fv(&p);

		glTexCoord2f(sprite->_tx2, sprite->_ty2 );
		vec3mov(p, *position);
		vec3madd(p, sprite->_sprite_height, *up);
		vec3madd(p, sprite->_sprite_width, *right);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, *right);
			vec3madd(p, -0.5*sprite->_sprite_height, *up);
		}
		glVertex3fv(&p);

		glTexCoord2f(sprite->_tx, sprite->_ty2 );
		vec3mov(p, *position);
		vec3madd(p, sprite->_sprite_height, *up);
		if (center)
		{
			vec3madd(p, -0.5*sprite->_sprite_width, *right);
			vec3madd(p, -0.5*sprite->_sprite_height, *up);
		}
		glVertex3fv(&p);

	glEnd();
}
