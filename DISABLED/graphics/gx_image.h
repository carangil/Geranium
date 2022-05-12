// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
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
	zuint32 _scaler;
	zbool   _sent_scaler;
} gx_image_t;

#define GX_IMAGE_SCALER_SMOOTH 0
#define GX_IMAGE_SCALER_BLOCKY 1

void gx_image_test(gx_image_t* image);

zbool gxi_image_enable(gx_image_t* image);

gx_image_t* gx_image_load_tga( zchar* f);
gx_image_t* gx_image_mk(zuint32 w, zuint32 h, zuint32 bpp);

void gx_image_set_scaler(gx_image_t* image, int scaler);
//

//BPP values
#define GX_IMAGE_GRAY		 1
#define GX_IMAGE_COLOR		 3
#define GX_IMAGE_COLOR_ALPHA 4

void gx_set_active_textures(gx_image_t** texes, zuint32 numtex);
int gxi_num_texture_units();
