#pragma once

#define ZBITMAP_GRAY	0x0001
#define ZBITMAP_BGR		0x0003
#define ZBITMAP_BGRX	0x0104
#define ZBITMAP_BGRA	0x0004
#define ZBITMAP_PXSIZE	0x00FF
#define ZBITMAP_FAKEA	0x0100

#define zbitmap_pxsize(ZZZ) ((ZZZ)->format & ZBITMAP_PXSIZE)

typedef struct zbitmap_s{
	zuint32 size;
	zuint16 w;
	zuint16 h;
	zuint32 format;
	void* data;

}zbitmapT;

typedef zuint32 zcolor;

//cleanup a bitmap
zbool zbitmap_cleanup(zbitmapT* bmp);

//create a bitmap
zbitmapT* zbitmap_mk(zuint32 w, zuint32 h, zuint16 format);

void zpset4(zbitmapT* bmp, zuint32 x, zuint32 y, zcolor color);
void zline4(zbitmapT *bmp, zuint32 x, zuint32 y, zuint32 x2, zuint32 y2, zuint32 color);

void zpblit4(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch);
void zpblit4c(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch, zuint32 color, zuint32 flags);

#define ZBLIT_COLORMASK 0x0100
#define ZBLIT_ALPHATEST 0x0200

//copies src alpha channel into green so it is visible when testing images alpha channel without having to actually blend
void zpblit4adebug(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch);
//drawtext takes an image, divides it into 256 tiles (16x16), and selects one tile to blit per character.
//intended use is to draw text, but could be used to draw sprites or even a row of tiles for a tile-based renderer
void zdrawtext4(zbitmapT* dest, zbitmapT* font, char* text, int px, int py, zuint32 color, zuint32 flags);

zbitmapT* zbitmap_load_tga( zchar* f, zuint32 flags);

#define ZTGA_TOP			0x001
#define ZTGA_GRAY_AS_ALPHA		0x100
#define ZTGA_COPY_GRAY_TO_ALPHA		0x200

//copy gray to alpha sets both RGB and ALPHA to the incoming grey value
//gray as alpha sets RGB to white and ALPHA to the incoming grey value

