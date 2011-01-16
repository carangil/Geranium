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

gx_sprite_t* gx_sprite_mk(gx_image_t* image, zint32 left, zint32 bottom, zint32 width, zint32 height, zfloat32 sprite_width, zfloat32 sprite_height);


void gx_sprite_draw(gx_sprite_t* sprite, zfloat32 x, zfloat32 y, zbool alpha_blend);

void gx_sprite_draw_rotozoom(gx_sprite_t* sprite, zfloat32 x, zfloat32 y, zbool alpha_blend,  zfloat32 xo, zfloat32 yo, zfloat32 zoom, zfloat32 ang);

void gx_text_draw(gx_image_t* font,  zfloat32 x, zfloat32 y,  zfloat32 angle, zchar* string);

void gx_text_size(zfloat32 width, zfloat32 height);

void gx_text_color(zfloat32 r,zfloat32 g,zfloat32 b, zfloat32 a);