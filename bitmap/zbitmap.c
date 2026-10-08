#include "ztypes.h"
#include "zbitmap.h"
#include "zmem.h"
#include "zarray.h"

 
zbool zbitmap_cleanup(zbitmapT* bmp){
	ram_free(bmp->data); //ram_free does not crash on NULL
	return ZTRUE; //free bmp when we exit
}

zbitmapT* zbitmap_mk(zuint32 w, zuint32 h, zuint32 format){
    
	zuint32 size=0;
	
	int pixelsize = (format & ZBITMAP_PXSIZE);

	switch (format){
	    
	    case ZBITMAP_GRAY:
	    case ZBITMAP_BGR:
	    case ZBITMAP_BGRA:
	    case ZBITMAP_BGRX:
		
		size = pixelsize * w * h;
		break;
		
	    //other types of formats here (TODO)  maybe palleted, etc, where 1 px needs less than 1 byte
		
	}
        
	if (size==0){
	    fprintf(stderr, "Bad bitmap format %x\n", format);    
	    return NULL;
	}
	
	printf(" size:%d pixelsize:%d\n", size, pixelsize);
	

    
	zbitmapT* bmp = ram_alloc(sizeof(zbitmapT), (ram_destructor) zbitmap_cleanup);
	if (bmp){	
	    bmp->w=w;
	    bmp->h=h;
	    bmp->size=w*h*pixelsize;



	    //bmp->data = ram_alloc( bmp->size, NULL);  //format happens to be the number of bytes per pixel... for now
		bmp->data = zarray_allocf(pixelsize, w*h, NULL, __FILE__, __LINE__);
		zarray_use(bmp->data, w*h); //all pixels used

		if (pixelsize == 4) {
			bmp->data32.address.block=bmp->data;
			bmp->data32.offset=0;
		}

		bmp->format = format;
	    if (!bmp->data) {
			ram_free(bmp);
			return NULL;
	    }
	}
	
	return bmp;
}



#define WARNRETURN(RRR,...)  {fprintf(stderr, __VA_ARGS__); return RRR;} 



void zpset4(zbitmapT*bmp, zuint32 x, zuint32 y, zuint32 color){
    if (!bmp)
	return;
    
    
    if ((x>= bmp->w) || (y>= bmp->h))
	WARNRETURN(, "zpset pixel out of range %d %d for %d by %d\n", x, y, bmp->w, bmp->h);
    
    zuint32* px = bmp->data;
    px[  (bmp->w)*y +x] = color;
    
}


zuint32  zpget4(zbitmapT* bmp, zuint32 x, zuint32 y) {
	if (!bmp)
		return 0;


	if ((x >= bmp->w) || (y >= bmp->h))
		WARNRETURN(0, "zpset pixel out of range %d %d for %d by %d\n", x, y, bmp->w, bmp->h);

	zuint32* px = bmp->data;
	return px[(bmp->w) * y + x];

}




void zline4(zbitmapT *bmp, zuint32 x, zuint32 y, zuint32 x2, zuint32 y2, zuint32 color){
    //not guarded for bounds yet...
    //pset does that... (slowly)
    
    float dx = (int)(x2-x);
    float dy = (int)(y2-y);
    float t;
    float fx=x;
    float fy=y;
    int ns;
    
 
   
    if (  abs(dy)>abs(dx))
	ns = abs(dy);
    else
	ns= abs(dx);
    
    float step =1.0/ns; 
    
    zpset4(bmp,x,y,0xff0000);
    zpset4(bmp,x2,y2,0xff0000);
 
    int i;
    zuint32 yi;
    
    for(i=0;i<=ns;i++) {
	fx+=step*dx;
	fy+=step*dy;
	yi = (zuint32)fy;
	if (yi >= bmp->h)  //sometimes rouning error
	    break;
	zpset4(bmp, fx, yi, color);
    }
  
}

void zpblit4(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch){
    int i,j;

    for(j=0;j<srch; j++){
	for (i=0;i<srcw;i++){
		((zuint32*)bmp->data)[   bmp->w * (j+y) +i+x] = ((zuint32*)src->data)[   src->w * (j+srcy) +i+srcx];
	}
    }
}

void zpblit4adebug(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch){
    int i,j;

    for(j=0;j<srch; j++){
	for (i=0;i<srcw;i++){
		zuint32 alpha = ((zuint32*)src->data)[   src->w * (j+srcy) +i+srcx] &0xff000000;
		alpha = alpha >>16;  //show in green channel;
		
		((zuint32*)bmp->data)[   bmp->w * (j+y) +i+x] = alpha;
	}
    }
}

void zpblit4c(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch, zuint32 color, zuint32 flags){
    int i,j;
    int colormask = 0xffffffff;
    
    
    if (x+srcw > bmp->w)
	srcw = bmp->w-x ;
    
    if (y+srch > bmp->h)
	srch = bmp->h-y ;
    
    if ( (flags & ZBLIT_COLORMASK) )
	colormask = color;
        
    
    for(j=0;j<srch; j++){
	for (i=0;i<srcw;i++){
		
		zuint32 scolor = ((zuint32*)src->data)[   src->w * (j+srcy) +i+srcx];
		
		if (  (flags & ZBLIT_ALPHATEST) && ( scolor < 0x80000000))
		    continue;	//skip pixels where alpha is less than half
	    
		((zuint32*)bmp->data)[   bmp->w * (j+y) +i+x] = colormask & scolor;
		
		
	}
    }
}

#if 0
//drawing text using a bitmap font and the simple blit routines
//this isn't very effecient, but this tests those routines, plus this doesn't exactly require a supercomputer to draw a few characters the slow way
void zdrawtext4(zbitmapT* dest, zbitmapT* font, char* text, int px, int py, zuint32 color, zuint32 flags){
    
    int cw = font->w/16;
    int ch = font->h/16;
    int c;
    
    while(c=*text){
	
	if (px+cw > dest->w){
	    px =0;
	    py+=ch;
	}
	
	if (py+ch>dest->h)
	    break;
		
	int sx= cw * (c%16);
	int sy=ch * (c/16);
	
	zpblit4c( dest, px,py, font, sx,sy, cw, ch, color, flags);
	
	text++;
	px+=cw;
    }
}
#endif

void zfread(void*v, size_t s, int n, FILE* f){
		int nr = fread(v,s,n,f);
		if (nr != n){
			fprintf(stderr, "fread did not read enough\n");
			exit(1);
		}

}


//taken from gx_image_load_tga
//the original version of this function I wrote in 2005
zbitmapT* zbitmap_load_tga( zchar* f, zuint32 flags)
{
	zbyte* temp = NULL;
	zbitmapT*  image = NULL;
	zbyte buf[6];
	zuint32 i=0;	
	zbool topleft = ZFALSE;
	zbool rle_compressed=ZFALSE;
	zbool flip = flags & 0xf;
	
	
	FILE* fi =fopen(f,"r+b");

	if (!fi) {
		printf(" can't open {%s}\n", f);
		return NULL;
	}

	printf(" opened {%s}\n", f);

	zfread( buf  ,4, 1 , fi);
	printf(" IMAGE TYPE %d\n", buf[2]);
	
	if ((buf[2] == 10) || (buf[2]==11))
		rle_compressed=1;
	
	fseek(fi, 12, SEEK_SET);

	zfread( buf  ,6, 1 , fi);

	int width = buf[0] | (buf[1]<<8);
	int height = buf[2] | ( buf[3]<<8);
	int bpp = buf[4] / 8 ;    //we want bytes per pixel, not bits
	
	if (buf[5] & 32)
	    flip ^=1;	//if file is topleft OR user wants topleft, we have to flip.  if neither, or both, we don't
	
	//at this point, bpp is conveniently 1 ZFORMAT_GRAY, 3 ZFORMAT,BGR or 4 ZFORMAT BGRA
	
	printf(" %d by %d at %d bpp  compressed:%s origin:%s\n", width, height, bpp, rle_compressed?"yes":"no" , topleft? "top":"bottom" );
	
	if (bpp == 2)
	    printf(" Warning bpp=2 (grayscale w/ alpha ?) is not fully supported\n");
	
	if ((bpp == 0)|| (bpp>4)){
	    return NULL; //bad format
	}
	
	    
	//always load to GBRA for now
	
	image = zbitmap_mk(width, height, ZBITMAP_BGRA);

	if (  ((ZBITMAP_BGRA & ZBITMAP_PXSIZE) != bpp) || flip  ){
	    //need to convert image
	    temp = ram_alloc( bpp * width* height, NULL);
	    printf(" converting\n");
	}
	
	if (image->data)
	{
		
		if (rle_compressed) {
			zuint32 dcount=0;
			zuint32 i;
			zbyte* dptr = temp ? temp : image->data; //decompress into temp or directly into bitmap
			zbyte header;
			while (dcount < width*height) {
				header = fgetc(fi);
				if (header & 128) {
					header = header -128;  //header is number of repeats minus one

			//		printf(" rle for %d  %p %p\n",header, image->data, dptr);

					zfread(dptr, bpp, 1, fi); //read 1 pixel

					for (i=0;i<header*bpp;i++) {  //repeat it i times
						dptr[i+bpp] = dptr[i];
						
					}
					dptr += bpp * header;

					dptr+= bpp;

					dcount += header+1;

//					rle section;
				} else {
					header++;
					// raw section
//					printf(" raw for %d  %p %p\n",header, image->data, dptr);
					zfread(dptr, bpp, header, fi);  //read pixel
					dptr += bpp*header;
					dcount += header;
					
				}

			}

		}
		else {
			/*read in all data*/
			zfread( temp? temp : image->data, 1,height * width * bpp , fi);
		}

		
		//now need to convert 
		if (temp){
			char* from = temp;
			char* to = image->data;
			
			int i,y,j;
			
			for (y=0;y<height;y++){
				
			    	    
			    i= y*width;
			    
			    if (flip) 
				j = width* (height-y-1);
			    else
				j = i;
			    
			    
			    for (   ; i< (y+1)*width;i++,j++){
				
				
				
				if (flags & ZTGA_GRAY_AS_ALPHA)
				    to[i*4] = 255;
				else
				    to[i*4] = from[j*bpp];  //blue or gray
				
				
				
				//GB to GB or grey to GB
				if (bpp > 2) {
				    to[i*4+1]= from[j*bpp+1];  //green
				    to[i*4+2]= from[j*bpp+2];  //red
				} else {
				    to[i*4+2]=to[i*4+1]=to[i*4]; //copy gray to g and r
				}
				
				//alpha to alpha, or 255 to alpha
				if (bpp ==4)
				    to[i*4+3] = from[j*bpp+3]; //copy alpha
				else if ( flags& (ZTGA_COPY_GRAY_TO_ALPHA|ZTGA_GRAY_AS_ALPHA))
				    to[i*4+3] = from[j*bpp];  //copy gray value to alpha channel
				else
				    to[i*4+3] = 255;
				
				
			    }
			}
		    ram_free(temp);
		}

	}
	

	fclose(fi);
	

	return image;
}

