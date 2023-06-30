#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "gfx_pixeltoaster.h"
#include "math.h"
#include "zbitmap.h"


typedef struct spx {
	float px[9];
	float w;

	struct spx* sub[9];

}spxT;



zvecT* spxs;


#define MAX_SPX 1000000

float distance2(spxT* a, spxT* b, int limit) {
	int i;
	float s=0;

	//if both have subs
	if (a->sub[0] && b->sub[0] && (limit>0) ) {
		for (i = 0; i < 9; i++) {
			s += distance2(a->sub[i], b->sub[i], 0);
		}
		
		return s / 9.0;  //shoud be 9, but biasing so 5x5 and 3x3 are less likely to match each other

	}

	//both missing
	//if (!a->sub[0] && !b->sub[0]) {

		//if one is missing subs, compare on 3x3 only

		for (i = 0; i < 9; i++) {
			float d = a->px[i] - b->px[i];
			s += d * d;
		}
		return s;
	//}
	//return 1; //5x5 and 3x3 can't compare

}

float avg(spxT* a) {
	int i;
	float s = 0;
	for (i = 0; i < 9 ; i++) {
		s += a->px[i];
	}
	return s / 9.0;
}

spxT* addspx(spxT* s) {


	if (zvec_count(spxs) == 0) {
		zvec_add(spxs, s);
		return s;
	}
	

	int i;
	float closestd = 999;
	int closest = 0;
	for (i = 0; i < zvec_count(spxs); i++) {
		float nd;
		if ( (nd = distance2(s, zvec_get_at(spxs, i),1)) < closestd) {
			closestd = nd;
			closest = i;
		}
	}

	if ((closestd > .001)&&(zvec_count(spxs)< MAX_SPX)){
		zvec_add(spxs, s);
		//printf(" NEW\n");
		return s;
	}

	//printf(" merge with %d\n", closest);

	spxT* closestp = zvec_get_at(spxs, closest);

	int n;
	//merge the large blocks if they are both present
	if (closestp->sub[0] && s->sub[0]) {
		for (i = 0; i < 9; i++) {
			for (n = 0; n < 9; n++) {

				closestp->sub[i]->px[n] = (closestp->sub[i]->px[n]  + s->sub[i]->px[n]) /2;
				
			}
			//closestp->sub[i]->w += s->sub[i]->w;//add weights

			closestp->px[i] = avg(closestp->sub[i]);  //adjust 3x3 after merging sub blocks
		}
	}

	//merge the pixels
	for (i = 0; i < 9; i++) {

		//merge pixels
		closestp->px[i] = (closestp->px[i] * closestp->w + s->px[i]) / (closestp->w+1);

		//merge subs
		if (!closestp->sub[i] && s->sub[i]) { //if cloests 3x3 doesn't have subdetail, just import it
			closestp->sub[i] = s->sub[i]; 
			
		} 


	}

	closestp->w++;

	return closestp;

}




int main(int argc, char** args){
	zwindowT* window = pt_mkwindow("testing", 1024,768,1);
	zuint16 window_pxformat = ZBITMAP_BGRA; //todo: window pxformat will be part of the window struct returned
	
	zeventT ev = {0};
	
	zbitmapT* font = zbitmap_load_tga("../../Zcore-data/font8rle.tga", ZTGA_TOP | ZTGA_COPY_GRAY_TO_ALPHA );
	
	spxs = zvec_mk(NULL, 1000);

	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/label-rgba-norle-topleft.tga", ZTGA_TOP );
	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/label-rgba-rle-bottomleft.tga",ZTGA_TOP);
	// 
	// 
	zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/treebw.tga",ZTGA_TOP);

	zbitmapT* picbig = zbitmap_mk(pic->w * 5 / 3, pic->h * 5 / 3, pic->format); //make larger picture

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
		
		zpblit4c( bmp, 0,0, pic, 0,0, pic->w, pic->h, 0,0);
		zpblit4c(bmp, 0, pic->h, picbig, 0, 0, picbig->w, picbig->h, 0, 0);
		
	//	zpblit4c( bmp, 300,0, pic, 0,0, pic->w, pic->h, 0xc0c0c0, ZBLIT_COLORMASK|ZBLIT_ALPHATEST);
		
				
		zdrawtext4(pic, font, "Superresolution Test", 00,0, 0xffffff, 0);
		
	//	zdrawtext4(bmp, font, "TestingiilsjfsdlkfjsldfjslfjlsdjflsdjflsdkjflsdkjflsdkfjsldkfjldskfjldskfjlsdkjfldskjflsdkjflslfjsldkfjlkdsjflkjfljlkfjslkdfjlsdjflksdjflsjlfjlskfjlskdfjlsdjflsdkfjlsdkfjlsdkfjslkfjsldkfjdslkfjsldkfjlsdkfjlskdfjldkfjlsdfjlsdkfjlsdkfjlsdkfjdsflkX", 8,120, 0xff5510, ZBLIT_COLORMASK|ZBLIT_ALPHATEST);
		
		window->pixels(window,bmp->data);
		
		int ll;
		for (ll = 0; ll < 10; ll++) {

			//pick random point;

			int mx = zrand() % (pic->w - 5) + 2;
			int my = zrand() % (pic->h - 5) + 2;

			spxT* sb = ram_alloc(sizeof(spxT), NULL);

			int x;
			int y;


			//5x5 block of overlapping 3x3 blocks
			int sbn = 0;
			for (x = mx - 1; x <= mx + 1; x++) {

				for (y = my - 1; y <= my + 1; y++) {

					spxT* s = ram_alloc(sizeof(spxT), NULL);

					int i;
					s->w = 1;
					for (i = 0; i < 9; i++) {
						s->px[i] = (zpget4(pic, x - 1 + (i % 3), y - 1 + (i / 3)) & 255) / 255.0;
					}
					printf("\n");

					sb->sub[sbn] = addspx(s);
					sb->px[sbn] = avg(sb->sub[sbn]);
					sbn++;
				}
			}
			sb->w = 1;
			addspx(sb); //add the larger block

			//draw the new 5x5 into the expanded picture
			float pix[25] = { 0 };
			float pc[25] = { 0 };
			int n;
			int j;
			for (n = 0; n < 9; n++) {
				if (!sb->sub[n])
					break;
				for (j = 0; j < 9; j++) {
					int tx = (n % 3) + (j % 3);
					int ty = (n / 3) + (j / 3);
					int cn = tx + ty * 5;
					pix[cn] += sb->sub[n]->px[j];
					pc[cn]++;
					unsigned int c = 255*pix[cn] / pc[cn];

					zpset4(picbig, mx*5/3 + 4 + tx, my*5/3 + ty,(c<<16) + (c<<8) + c);
				}


			}


		}

		int i;
		//draw the atlas of low level features
		for (i = 0; i < zvec_count(spxs) ; i++) {
			spxT* s = zvec_get_at(spxs, i);
			int ii = (i% 32) *15  + pic->w *5/3+10;
			int jj = (i / 32) * 15;

			if (jj + 15 > bmp->h)
				break;

			if (!s) break;
			int j;
			int px, py;
			for (j = 0; j < 9; j++) {
				zpset4(bmp, px= ii+  j % 3, py= jj+ j/3, 65537 * s->px[j]);
			}

			/*
			//do subs
			int n;
			for (n = 0; n < 9; n++) {
				if (!s->sub[n])
					break;

				for (j = 0; j < 9; j++) {
					zpset4(bmp, px+3+  (n%3*3)  +j % 3, jj +  (n/3*3) +  j / 3, 65535 * s->sub[n]->px[j]);
				}


			}*/

			float pix[25] = { 0 };
			float pc[25] = { 0 };
			int n;
			for (n = 0; n < 9; n++) {
				if (!s->sub[n])
					break;
				for (j = 0; j < 9; j++) {
					int tx = (n % 3) + (j % 3);
					int ty = (n / 3) + (j / 3);
					int cn = tx + ty * 5;
					pix[cn] += s->sub[n]->px[j];
					pc[cn]++;
					zpset4(bmp, px + 4 + tx ,  py+ty , 65537 *  pix[cn]/  pc[cn]          );
				}


			}
			



		}

	}
	
	printf(" done with window loop\n");

}



