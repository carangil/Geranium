#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "gfx_pixeltoaster.h"
#include "math.h"
#include "ztime.h"


#define ZBITMAP_GRAY	1
#define ZBITMAP_BGR	3
#define ZBITMAP_BGRA	4



typedef struct zbitmap_s{
	zuint32 size;
	zuint16 w;
	zuint16 h;
	zuint16 format;
	void* data;
}zbitmapT;

#define WARNRETURN(RRR,...)  {fprintf(stderr, __VA_ARGS__); return RRR;} 

/*These functions are slow junk, but just to show other things are working.  Will be optimized someday if its determined they have to be */

void zpset4(zbitmapT*bmp, zuint32 x, zuint32 y, zuint32 color){
    if (!bmp)
	return;
    
    if ((x>= bmp->w) || (y>= bmp->h))
	WARNRETURN(, "zpset pixel out of range %d %d for %d by %d\n", x, y, bmp->w, bmp->h);
    
    zuint32* px = bmp->data;
    px[  (bmp->w)*y +x] = color;
    
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
  
    
    for(i=0;i<=ns;i++) {
	fx+=step*dx;
	fy+=step*dy;
	zpset4(bmp, (int)fx, (int)fy, color);
    }
  
}

zbool zbitmap_cleanup(zbitmapT* bmp){
	ram_free(bmp->data); //ram_free does not crash on NULL
	return ZTRUE; //free bmp when we exit
}

zbitmapT* zbitmap_mk(zuint32 w, zuint32 h, zuint16 format){
    
	switch (format){
	    case ZBITMAP_GRAY:
	    case ZBITMAP_BGR:
	    case ZBITMAP_BGRA:
		break;
		
	    default: 
		fprintf(stderr, "Bad bitmap format %x\n", format);
		return NULL;
	    
	}
    
	zbitmapT* bmp = ram_alloc(sizeof(zbitmapT), zbitmap_cleanup);
	if (bmp){	
	    bmp->w=w;
	    bmp->h=h;
	    bmp->size=w*h*format;
	    bmp->data = ram_alloc( bmp->size, NULL);  //format happens to be the number of bytes per pixel... for now
	    
	    if (!bmp->data) {
		ram_free(bmp);
		return NULL;
	    }
	}
	
	return bmp;
    
}




int main(int argc, char** args){
	zwindowT* window = pt_mkwindow("testing", 800,600,0);
	zuint16 window_pxformat = ZBITMAP_BGRA; //todo: window pxformat will be part of the window struct returned
	
	zeventT ev = {0};

	//char* px = ram_alloc(800*600*4,NULL);
	zbitmapT* bmp = zbitmap_mk( window->w, window->h, window_pxformat  );

	memset(bmp->data, 64, bmp->size);

	
//	zline4(bmp, 300,300,100,30,0xff00ff);
	
//	float a=M_PI;
	float a;
	for (a=0;a< M_PI*2; a+=.1)
	
	{
	    //a=5;
		zline4(bmp, 400,300, 400 + 100*cos(a), 300+100*sin(a), 0xff0000 );
		window->pixels(window,bmp->data);
	//	tm_msleep(50);
	}

	
	
	while(ev.type != ZEVENT_CLOSE){
		zbool b;
		
		while (b = window->event(window, &ev) ) {
			if (b)
				printf(" EVENT %x %d %d\t\t %c\n", ev.type, ev.a, ev.b, ev.a);
			
			if ( (ev.type & ZEVENT_MOVE) && ( ev.type&ZEVENT_MOUSE)){
				zpset4(bmp, ev.a, ev.b, (ev.type& ZEVENT_MOUSE_STATE_L)?0xff00:0xff0000 );
			    
			}
			
		}
				
		window->pixels(window,bmp->data);
	}

}



