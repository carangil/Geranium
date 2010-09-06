// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


//image data buffer

typedef struct
{
	zuint32 width;
	zuint32 height;
	zbyte*	data;
	zuint32 bpp;
	zuint32 _gl_texture_number;
	zbool	_sent_to_gl;
} gx_image_t;



void gx_image_test(gx_image_t* image);

zbool _gx_image_enable(gx_image_t* image);

gx_image_t* gx_image_load_tga( zchar* f);

void gx_set_active_textures(gx_image_t** texes, zuint32 numtex);