// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


typedef struct
{
	gx_image_t* image;

	//texture coordinates
	zfloat32 _tx;
	zfloat32 _ty;
	zfloat32 _tx2;
	zfloat32 _ty2;
	
	//size the sprite appears on the screen:
	zfloat32 _sprite_width;
	zfloat32 _sprite_height;

} gx_sprite_t;

gx_sprite_t* gx_sprite_mk(gx_image_t*, zint32 left, zint32 top, zint32 right, zint32 bottom, zfloat32 width, zfloat32 height);


void gx_sprite_draw(gx_sprite_t* sprite, zfloat32 x, zfloat32 y);