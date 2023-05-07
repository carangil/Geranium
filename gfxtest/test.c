#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "gfx_pixeltoaster.h"
#include "math.h"
#include "zbitmap.h"




int main(int argc, char** args){
	zwindowT* window = pt_mkwindow("testing", 1024,512,1);
	zuint16 window_pxformat = ZBITMAP_BGRA; //todo: window pxformat will be part of the window struct returned
	
	zeventT ev = {0};
	
	zbitmapT* font = zbitmap_load_tga("../../Zcore-data/font8rle.tga", ZTGA_TOP | ZTGA_COPY_GRAY_TO_ALPHA );
	
	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/label-rgba-norle-topleft.tga", ZTGA_TOP );
	zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/label-rgba-rle-bottomleft.tga",ZTGA_TOP);
	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/brickrgb.tga",ZTGA_TOP );
	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/font8rle.tga", ZTGA_TOP | ZTGA_COPY_GRAY_TO_ALPHA );
	
	
	/*
	zbitmapT* pic = zbitmap_mk( 64, 32, window_pxformat  );
	int i,j;
	for (j=0;j<pic->h;j++){
	    for(i=0;i<pic->w;i++){
		zpset4(pic, i, j,     (j*i) | ((5*j+5*i)<<8 ) | ((5*j^5*i)<<16)   );
	    }
	}
	*/
	
	zbitmapT* bmp = zbitmap_mk( window->w, window->h, window_pxformat  );

	memset(bmp->data, 64, bmp->size);

	int c=0;	
	//int i;
	
	while(ev.type != ZEVENT_CLOSE){
		zbool b;
		
		while (b = window->event(window, &ev) ) {
			
				printf(" EVENT %x %d %d\t\t %c\n", ev.type, ev.a, ev.b, ev.a);
			
			if ( (ev.type & ZEVENT_MOVE) && ( ev.type&ZEVENT_MOUSE)){
				zpset4(bmp, ev.a, ev.b, (ev.type& ZEVENT_MOUSE_STATE_L)?0xff00:0xff0000 );
			    
			}
			
			if ( (ev.type & ZEVENT_CHAR) && (ev.a == 'q') && ((ev.type & ZKEY_CTRL) ==ZKEY_CTRL)     )
				window->close(window);
			
			
		}
			
		
		/*
		for (i=0;i<600;i+=1){
		    
			zline4(bmp, 0, i, i, 0,c);
			
			zline4(bmp, 799, i, 799-i, 0,c);
			
			
			zline4(bmp, 0, 599-i, i, 599-0,c);
			
			zline4(bmp, 799, 599-i, 799-i, 599-0,c);
			
			c+=0x030201;
		    
		}	*/
		
		zpblit4c( bmp, 200,30, pic, 0,0, pic->w, pic->h, 0xff0000, ZBLIT_COLORMASK|ZBLIT_ALPHATEST);
		
		zpblit4c( bmp, 300,0, pic, 0,0, pic->w, pic->h, 0xc0c0c0, ZBLIT_COLORMASK|ZBLIT_ALPHATEST);
		
				
		zdrawtext4(bmp, font, "Whatever", 50,100, 0xffffff, 0);
		
		zdrawtext4(bmp, font, "TestingiilsjfsdlkfjsldfjslfjlsdjflsdjflsdkjflsdkjflsdkfjsldkfjldskfjldskfjlsdkjfldskjflsdkjflslfjsldkfjlkdsjflkjfljlkfjslkdfjlsdjflksdjflsjlfjlskfjlskdfjlsdjflsdkfjlsdkfjlsdkfjslkfjsldkfjdslkfjsldkfjlsdkfjlskdfjldkfjlsdfjlsdkfjlsdkfjlsdkfjdsflkX", 8,120, 0xff5510, ZBLIT_COLORMASK|ZBLIT_ALPHATEST);
		
		window->pixels(window,bmp->data);
		
	}
	
	printf(" done with window loop\n");

}



