// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

//image data buffer
#include "zbitmap.h"


typedef struct
{
	zbitmapT* bitmap;
	zuint32 _gl_texture_number;
	zuint32 _scaler;
	zbool   _sent_scaler;
} gfx_textureT;

#define GFX_TEXTURE_SCALER_SMOOTH 0
#define GFX_TEXTURE_SCALER_BLOCKY 1

gfx_textureT* gfx_texture_mk(zbitmapT* bmp);

void gfx_texture_scaler(gfx_textureT* image, int scaler);


