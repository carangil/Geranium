#include "ztypes.h"
#include "zwindow.h"
// first	last
// 0		0	empty
// 1 		1       empty
// 5		6	1 item (at 5)
// 5		4	//items at 5,6 7,8,9, 0,1 ,2 3

void zw_enqueue(zwindowT* zw, zuint32 type, zuint32 a, zuint32 b, void* ptr) {

	int nlast = (zw->last + 1) % MAXEVENT;


	if (nlast == zw->first)
		return; //queue full

	zw->queue[zw->last].type = type;
	zw->queue[zw->last].a = a;
	zw->queue[zw->last].b = b;
	zw->queue[zw->last].ptr = ptr;
	zw->last = nlast;
}

zbool zw_event(zwindowT * zw, zeventT * ev) {
	

	//try to return an event
	if (zw->first != zw->last) {
		*ev = zw->queue[zw->first]; //copy event
		zw->first = (zw->first + 1) % MAXEVENT;
		return ZTRUE;
	}

	ev->type = ZEVENT_NONE;
	return ZFALSE;

}

zuint32 eventMasks[] = { ZEVENT_KEY, ZEVENT_CHAR, ZEVENT_MOUSE, ZEVENT_CLOSE, ZEVENT_DOWN, ZEVENT_UP, ZEVENT_MOVE, ZEVENT_DELTA, ZEVENT_MOUSE_STATE_L, ZEVENT_MOUSE_STATE_M,ZEVENT_MOUSE_STATE_R,ZEVENT_MOUSE_L,ZEVENT_MOUSE_M, ZEVENT_MOUSE_R,ZKEY_CTRL, ZKEY_SHIFT, ZKEY_ALT , 0 };
char*   eventNames[] ={ "ZEVENT_KEY","ZEVENT_CHAR","ZEVENT_MOUSE","ZEVENT_CLOSE","ZEVENT_DOWN","ZEVENT_UP","ZEVENT_MOVE","ZEVENT_DELTA","ZEVENT_MOUSE_STATE_L","ZEVENT_MOUSE_STATE_M","ZEVENT_MOUSE_STATE_R","ZEVENT_MOUSE_L","ZEVENT_MOUSE_M","ZEVENT_MOUSE_R","ZKEY_CTRL","ZKEY_SHIFT","ZKEY_ALT", 0 };


void zprintevent(zeventT* ev) {
	if (ev) {

		

		int i;
		
		printf("a:%d\tb:%d\t", ev->a, ev->b);

		if ((ev->type & ZEVENT_KEY) || (ev->type & ZEVENT_CHAR))
			printf(" %c ", ev->a);
		
		for (i = 0; eventMasks[i]; i++)
			if (ZEVENTIS(ev->type, eventMasks[i]))
				printf("%s ", eventNames[i]);

		
		printf("\n");



	}



}