#pragma once

//General window interface struct


//Zdef struct zeventT ZEvent:type=eventType:Z32;a=A:Z32;b= B:Z32;
typedef struct zevent_s {
	zuint32 type;
	zuint32 a, b;
	void* ptr;
} zeventT;

#define ZEVENT_NONE	0x0

//top 2 digits are the event type.  The 0N000000  digit is reserver for input bitmask. Must be 0 on other event types
#define ZEVENT_KEY		0x01000000
#define ZEVENT_CHAR		0x02000000
// can have another bitmask  event 0x04000000 
#define ZEVENT_MOUSE	0x08000000

//top digit isn't bitmasked, but are exact codes

#define ZEVENT_CLOSE		0x10000000
#define ZEVENT_WINDOWSIZE	0x20000000
//#define ZEVENT_WINDOWMOVE   0x30000000


//everything else is dependant on the type
//for keyboard and mouse events:

//subevent: DOWN/UP for keyboard and mouse clicks
//(ev.type & ZEVENT_UP) means a key or mouse button was released
#define ZEVENT_DOWN		0x00010000
#define ZEVENT_UP		0x00020000  
#define ZEVENT_MOVE		0x00040000
#define ZEVENT_DELTA	0x00080000

//mouse events also give the *current* mouse state as bitmask
// if (ev.type & ZEVENT_MOUSE_STATE_M ) means middle button is pressed
#define ZEVENT_MOUSE_STATE_L	0x00001000
#define ZEVENT_MOUSE_STATE_M	0x00002000
#define ZEVENT_MOUSE_STATE_R	0x00004000


//mouse events specify x and y in a and b
//(ev.type & ZEVENT_MOUSE_R) means the event concerns the left mouse button
#define ZEVENT_MOUSE_L		0x00000100
#define ZEVENT_MOUSE_M		0x00000200
#define ZEVENT_MOUSE_R		0x00000400

/* Defined in keyboard constants
#define ZKEY_CTRL		0x00000081
#define ZKEY_SHIFT		0x00000082
#define ZKEY_ALT		0x00000084
*/


#define ZEVENTIS(EVTYPE, EVMASK)      (((EVTYPE)&(EVMASK))==(EVMASK))


//named keys that are also characters
#define ZKEY_BACKSPACE	'\b'
#define ZKEY_TAB	'\t'
#define ZKEY_ENTER	'\n'
#define ZKEY_ESCAPE	0x1b

#define ZKEY_LASTCHAR	0x7f


#define ZKEY_PAUSE	0x80

//0x81 to 0x8f are for combinations of ctrl, shift, alt in modifier masks

#define ZKEY_MODFIRST	0x81
#define ZKEY_CTRL		0x81
#define ZKEY_SHIFT		0x82
#define ZKEY_ALT		0x84
#define ZKEY_MOD_FUTURE 0x88	/*something other than ctrl,alt,shift... Some future special key*/
#define ZKEY_MODLAST	0x8f

#define ZKEY_F1		0x91
#define ZKEY_F2		0x92
#define ZKEY_F3		0x93
#define ZKEY_F4		0x94
#define ZKEY_F5		0x95
#define ZKEY_F6		0x96
#define ZKEY_F7		0x97
#define ZKEY_F8		0x98
#define ZKEY_F9		0x99
#define ZKEY_F10	0x9a
#define ZKEY_F11	0x9b
#define ZKEY_F12	0x9c

#define ZKEY_LEFT	0xa0
#define ZKEY_RIGHT	0xab
#define ZKEY_UP		0xac
#define ZKEY_DOWN	0xad
#define ZKEY_PGUP	0xae
#define ZKEY_PGDN	0xaf

#define ZKEY_HOME	0xb0
#define ZKEY_END	0xb1
#define ZKEY_INS	0xb2
#define ZKEY_DEL	0xb3

/* On systems that can tell the difference between left and right control, alt, shift keys, then keyup/down events on the right keys will be RCTRL, RALT, RSHIFT*/
/* The right ALT, SHIFT and CTRL keys will still produce ZKEY_CTRL  (aka 'left') codes when used as modifiers  CTRL-B doesn't care if its left or right CTRL*/
/* On systems that can't tell between left and right, then RCTRL, etc will not be used at all */
#define ZKEY_RCTRL	0xb4
#define ZKEY_RSHIFT	0xb5
#define ZKEY_RALT	0xb6



#define ZKEY_LASTKEY	0xff

//Zdef type zwindow_s ZWindow
//Zdef type zwindowT ZWindow



#define MAXEVENT 10
typedef struct zwindow_s {
	zbool(*event) (struct zwindow_s* w, zeventT* ev);
	void (*pixels) (struct zwindow_s* w, void* pixels);
	void (*close) (struct zwindow_s* w);
	int w, h;
	zeventT queue[MAXEVENT];
	int first;
	int last;
} zwindowT;



#ifdef __cplusplus
//extern "C" {
    
#endif

void zw_enqueue(zwindowT* zw, zuint32 type, zuint32 a, zuint32 b, void* ptr);
zbool zw_queued(zwindowT* zw, zeventT* ev);

//Zdef proc zw_event GetEvent
zbool zw_event(zwindowT* zw, zeventT* ev);

//Zdef proc zw_close Close:(w:ZWindow%->);
void zw_close(zwindowT* zw);

//Zdef proc zw_pixels UpdatePixels
void zw_pixels(zwindowT* zw, void* v);

void zprintevent(zeventT* ev);  //debug function

#ifdef __cplusplus
//}
#endif
