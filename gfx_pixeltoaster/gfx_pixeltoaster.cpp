// Keyboard and Mouse Example
// How to get keyboard and mouse events from a display
// Part of the PixelToaster Framebuffer Library - http://www.pixeltoaster.com

#include <cstdio>
#include "PixelToaster.h"
#include "ztypes.h"
#define INCPPSOURCE
#include "gfx_pixeltoaster.h"

using namespace PixelToaster;



void pt_enqueue(zwindowT* zw, zuint32 type, zuint32 a, zuint32 b, void* ptr);


const zuint32 ktab_shift[] =
{

	'!', '1',
	'@', '2',
	'#', '3',
	'$', '4',
	'%', '5',
	'^', '6',
	'&', '7',
	'*', '8',
	'(', '9',
	')', '0',
	'_', '-',
	'+', '=',
	'?', '/',
	'<', ',',
	'>', '.',
	':', ';',
	'\"', '\'',
	'|', '\\',	
	0,0
};


const zuint32 ktab[] = {

	'a', Key::A,
	'b', Key::B,
	'c', Key::C,
	'd', Key::D,
	'e', Key::E,
	'f', Key::F,
	'g', Key::G,
	'h', Key::H,
	'i', Key::I,
	'j', Key::J,
	'k', Key::K,
	'l', Key::L,
	'm', Key::M,
	'n', Key::N,
	'o', Key::O,
	'p', Key::P,
	'q', Key::Q,
	'r', Key::R,
	's', Key::S,
	't', Key::T,
	'u', Key::U,
	'v', Key::V,
	'w', Key::W,
	'x', Key::X,
	'y', Key::Y,
	'z', Key::Z,
	',', Key::Comma,
	'.', Key::Period,
	'/', Key::Slash,
	'[', Key::OpenBracket,
	']', Key::CloseBracket,
	'\\', Key::BackSlash,
	'1', Key::One,
	'2', Key::Two,
	'3', Key::Three,
	'4', Key::Four,
	'5', Key::Five,
	'6', Key::Six,
	'7', Key::Seven,
	'8', Key::Eight,
	'9', Key::Nine,
	'0', Key::Zero,
	'\n', Key::Enter,
	'\b', Key::BackSpace,
	'\t', Key::Tab,
	' ', Key::Space,
	'\'', Key::Quote,
	'`', Key::BackQuote,
	27, Key::Escape,
	ZKEY_SHIFT, Key::Shift,
	ZKEY_ALT, Key::Alt,
	ZKEY_CTRL, Key::Control,
	0,0
	
};

zuint32 shift(zuint32 key, int sh){
	if (!sh) 
		return key; //return the key if there is no shift state pressed

	return key;

}


zuint32 ptkey(Key key, zbool aschar,  zbool shiftstate) {

    	int i;

	int zkey=0;

	//translate to zkey constants
	for (i=0;ktab[i]; i+=2) {
	
		if ( key == ktab[i+1])
			zkey = ktab[i];

	}

	if (!aschar)
		return zkey; //return the key

	//want a character
	
	if (zkey > ZKEY_LASTCHAR)
		return 0;

	if (shiftstate) {
		//simple case shift
		if (zkey >='a' && zkey<='z')
			return zkey - 'a' + 'A';

		//character shift:  This really depends on keyboard layout
		//This is a hack really.
		//TODO: call a system function to figure out what character to generate

		for (i=0;ktab_shift[i]; i+=2) {
	
			if ( zkey == ktab_shift[i+1])
				return ktab_shift[i];

		}

	}


	return zkey;
}


class WindowListener : public Listener
{
public:

	zwindowT* zwin = NULL;

protected:

    bool defaultKeyHandlers() const { return false; }

    int keystate=0;
    

    void onKeyDown( DisplayInterface & display, Key key )
    {
    	//values of ZKEY_SHIFT/ALT/DELETE chosen carefully so they can be used as a bitmask together
    	if (key == Key::Shift)
		keystate|= ZKEY_SHIFT;

    	if (key == Key::Alt)
		keystate|= ZKEY_ALT;

    	if (key == Key::Control)
		keystate|= ZKEY_CTRL;



    	pt_enqueue(this->zwin, ZEVENT_KEY|ZEVENT_DOWN|keystate , ptkey(key,0,0) , 0, 0);
	
	char c = ptkey(key,1, (keystate & ZKEY_SHIFT)  == ZKEY_SHIFT );

	if (c) {
	//	printf(" Generate char %c with keystate %x\n",c,keystate);
    		pt_enqueue(this->zwin, ZEVENT_KEY|ZEVENT_CHAR|keystate , c , 0, 0);
	}

    }

    void onKeyPressed( DisplayInterface & display, Key key )
    {
    	//these seem to be continuous, not needed here
    }

    void onKeyUp( DisplayInterface & display, Key key )
    {

    	if (key == Key::Shift)
		keystate&= ~ZKEY_SHIFT;

    	if (key == Key::Alt)
		keystate&= ~ZKEY_ALT;

    	if (key == Key::Control)
		keystate&= ~ZKEY_CTRL;

	keystate |= 0x80;  //don't clear the top bit


    	pt_enqueue(this->zwin, ZEVENT_KEY|ZEVENT_UP , ptkey(key,0,0) , 0, 0);

    }

   int mbstatus=0;

   int mouseButtons(Mouse mouse, int buttondelta){
   	int ev=0;

	if (mouse.buttons.left)
		ev |= ZEVENT_MOUSE_L;

	if (mouse.buttons.middle)
		ev |= ZEVENT_MOUSE_M;

	if (mouse.buttons.right)
		ev |= ZEVENT_MOUSE_R;


	if (!buttondelta) {
		//not delta, this is the full state of the mouse buttons
		ev = ev << 4;
		mbstatus = ev; //save the state
	}
	
	return ev;

    }

    void onMouseButtonDown( DisplayInterface & display, Mouse mouse )
    {
    	pt_enqueue(this->zwin, ZEVENT_MOUSE | ZEVENT_DOWN | mouseButtons(mouse, ZEVENT_DOWN) |keystate, mouse.x, mouse.y, 0);
    }

    void onMouseButtonUp( DisplayInterface & display, Mouse mouse )
    {
    	pt_enqueue(this->zwin, ZEVENT_MOUSE | ZEVENT_UP | mouseButtons(mouse, ZEVENT_UP) |keystate, mouse.x, mouse.y, 0);
    }

    void onMouseMove( DisplayInterface & display, Mouse mouse )
    {
    	pt_enqueue(this->zwin, ZEVENT_MOUSE | ZEVENT_MOVE | mouseButtons(mouse, 0) |keystate, mouse.x, mouse.y, 0);
    }

    void onActivate( DisplayInterface & display, bool active )
    {
        //printf( "onActivate: active=%d\n", active );
    }

    void onOpen( DisplayInterface & display )
	{
	    /*
		printf( "onOpen: \"%s\", %d x %d ", display.title(), display.width(), display.height() );
		switch ( display.mode() )
		{
			case Mode::TrueColor: printf( "truecolor" ); break;
			case Mode::FloatingPoint: printf( "floating point" ); break;
		}
		switch ( display.output() )
		{
			case Output::Windowed: printf( " (windowed)\n" ); break;
			case Output::Fullscreen: printf( " (fullscreen)\n" ); break;
			default: break;
		}
		*/
    }

    bool onClose( DisplayInterface & display )
    {
	/*
		printf( "onClose" );
	    */
        return true;
    }

    


};



typedef struct ptWindow_s{
	zwindowT interface;
	WindowListener* wl;
	Display* display;
} ptWindowT;

// first	last
// 0		0	empty
// 1 		1       empty
// 5		6	1 item (at 5)
// 5		4	//items at 5,6 7,8,9, 0,1 ,2 3

void pt_enqueue(zwindowT* zw, zuint32 type, zuint32 a, zuint32 b, void* ptr){

	int nlast = (zw->last + 1 ) % MAXEVENT;


	if (nlast == zw->first)
		return; //queue full

	zw->queue[zw->last].type=type;
	zw->queue[zw->last].a=a;
	zw->queue[zw->last].b=b;
	zw->queue[zw->last].ptr=ptr;
	zw->last = nlast;
}


extern "C" zbool pt_event(zwindowT* zw, zeventT* ev){
	ptWindowT* ptw = (ptWindowT*) zw;

	int op = ptw->display->open();

	if (!op) {
		ev->type=ZEVENT_CLOSE;
		return ZFALSE;
	}

	//try to return an event
	if (zw->first != zw->last){
		*ev = zw->queue[zw->first]; //copy event
		zw->first = (zw->first+1) % MAXEVENT;
		return ZTRUE;
	}

	ev->type = ZEVENT_NONE;
	return ZFALSE;

}

extern "C" void pt_pixels(zwindowT* zw, void* px){
	ptWindowT* ptw = (ptWindowT*) zw;

	ptw->display->update((const PixelToaster::TrueColorPixel*)px);
}

extern "C" void pt_close(zwindowT* zw){
	ptWindowT* ptw = (ptWindowT*) zw;

	ptw->display->close();
}

extern "C" zwindowT* pt_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags){

	ptWindowT* ptw = (ptWindowT*) malloc(sizeof(ptWindowT));

	ptw->wl = new WindowListener();
	ptw->wl->zwin = &ptw->interface;
	ptw->display = new Display();	
	ptw->display->listener(ptw->wl);
	ptw->display->open(title, w, h, Output::Windowed, Mode::TrueColor);
	ptw->interface.event = pt_event;  //function to get events
	ptw->interface.pixels = pt_pixels;
	ptw->interface.close= pt_close;
	ptw->interface.w=w;
	ptw->interface.h=h;
	return &ptw->interface;
}




