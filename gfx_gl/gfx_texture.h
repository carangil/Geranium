// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

//image data buffer
#include "zbitmap.h"

//Zdef type gfx_textureT Texture

typedef struct
{
	zbitmapT* bitmap;
	zuint32 _gl_texture_number;
	zuint32 _scaler;
	zbool   _sent_scaler;
} gfx_textureT;

#define GFX_TEXTURE_SCALER_SMOOTH 0
#define GFX_TEXTURE_SCALER_BLOCKY 1

//Zdef proc gfx_texture_mk CreateTexture:(bitmap:ZBitmap%trash->Texture%); //callee indicates the function being called adds a reference and stores the pointer, or returns without freeing the pointer.  WHen function returns, a reference will be taken away.
gfx_textureT* gfx_texture_mk(zbitmapT* bmp);

//Zdef proc gfx_texture_scaler Scaler
void gfx_texture_scaler(gfx_textureT* image, int scaler);

void gxi_new_texture_set();
zuint32 gxi_add_texture(gfx_textureT* tex);
void gxi_texture_complete();