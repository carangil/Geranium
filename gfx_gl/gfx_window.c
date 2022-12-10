#define GFXINTERNAL
#include "ztypes.h"
#include "zmem.h"
#include "zwindow.h"
#include "gfx_gl.h"
#include "zvector.h"
#include "zstring.h"
#include "string.h"
#include "zarray.h"
#include <stdio.h>




/*opengl error checker*/
char* last_file;
int last_line;

#define checkGL()   checkGLfunc(__FILE__, __LINE__)

void checkGLfunc(char* file, int line) {
	int err;

	for (err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
		printf("OPENGL ERROR %x FROM %s:%d to %s:%d\n", err, last_file, last_line, file, line);
	}

	last_file = file;
	last_line = line;

}

/*glfw windowing and zevent interface*/
typedef struct gfx_windowS {
	zwindowT iface;	//the zevent window interface
	GLFWwindow* fwindow;
}gfx_windowT;

void errorHandler(int error, const char* message) {
	printf(" glfw e");
	printf("GLFW ERROR:%d (0x%x) %s\n", error, error, message ? message : "null");

}

zuint32 keymodstate = 0;

zbool keystatus[256];

zbool gx_keystate(zuint32 key) {

	if (key < 256)
		return keystatus[key];

	return ZFALSE;

}

void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {

	int zkey = 0;

	//printf(" Key: %d %x %c\n", key, key, key);

	//translate to ZEVENT keys


	switch (key) {
	case GLFW_KEY_ENTER:		zkey = ZKEY_ENTER;		break;
	case GLFW_KEY_BACKSPACE:	zkey = ZKEY_BACKSPACE;	break;
	case GLFW_KEY_ESCAPE:		zkey = ZKEY_ESCAPE;		break;
	case GLFW_KEY_TAB:			zkey = ZKEY_TAB;		break;

		//add others later
	case GLFW_KEY_LEFT:			zkey = ZKEY_LEFT;		break;
	case GLFW_KEY_RIGHT:		zkey = ZKEY_RIGHT;		break;
	case GLFW_KEY_UP:			zkey = ZKEY_UP;			break;
	case GLFW_KEY_DOWN:			zkey = ZKEY_DOWN;		break;
	case GLFW_KEY_LEFT_CONTROL:	zkey = ZKEY_CTRL;		break;
	case GLFW_KEY_LEFT_SHIFT:	zkey = ZKEY_SHIFT;		break;
	case GLFW_KEY_LEFT_ALT:		zkey = ZKEY_ALT;		break;
	case GLFW_KEY_RIGHT_CONTROL: zkey = ZKEY_RCTRL;		break;
	case GLFW_KEY_RIGHT_SHIFT:	zkey = ZKEY_RSHIFT;		break;
	case GLFW_KEY_RIGHT_ALT:	zkey = ZKEY_RALT;		break;

	}

	if ((key < 256) & (!zkey)) {

		//GLFW uses ascii for lots of printing characters, so does ZEVENT
		zkey = key;

		if ((zkey >= 'A') && (zkey <= 'Z'))
			zkey = zkey - 'A' + 'a'; //convert to lowercase

	}

	keymodstate = 0;

	if (mods & GLFW_MOD_SHIFT)
		keymodstate |= ZKEY_SHIFT;

	if (mods & GLFW_MOD_CONTROL)
		keymodstate |= ZKEY_CTRL;

	if (mods & GLFW_MOD_ALT)
		keymodstate |= ZKEY_ALT;


	//translate the modifiers
	int keystate = ZEVENT_KEY;

	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT) ) {
		keystate |= ZEVENT_DOWN | keymodstate;

		if (zkey < 256)
			keystatus[zkey] = 1;

	}

	if (action == GLFW_RELEASE) {
		keystate |= ZEVENT_UP;
		//zevent doesn't care about if shift/alt/ctrl are held down while releasing a key
		if (zkey < 256)
			keystatus[zkey] = 0;

	}

	gfx_windowT* win= glfwGetWindowUserPointer(window);
	
	if (win == NULL) {
		printf(" not a zevent window! serious bug?\n");
	}

	zw_enqueue(&win->iface, keystate, zkey, 0, 0);

}

void charHandler(GLFWwindow* window, unsigned int character) {

	//glfw doesn't do modifiers with character callbacks
	//todo: we could probably read or track keystate
	//also might want to generate characters for backspace, enter, tab, etc, as they do have character codes (they keydown/repeat event should do that)
	gfx_windowT* win = (gfx_windowT*)glfwGetWindowUserPointer(window); ;
	zw_enqueue(&win->iface, ZEVENT_CHAR, character, 0, 0);
}

zint32 last_mouse_x=0;
zint32 last_mouse_y=0;
zuint32 mouse_button_state=0;

zbool mouse_relative = ZFALSE;

void gfx_mouse_relative(zwindowT* zw, zbool rel) {

	if (rel) {
		mouse_relative = 1;  //turn on relative mode  .throw away first value
		glfwSetInputMode(((gfx_windowT*)zw)->fwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}
	else {
		mouse_relative = 0;  //to disable
		glfwSetInputMode(((gfx_windowT*)zw)->fwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);		
	}


}





void mousemoveHandler(GLFWwindow* window, double x, double y) {

	int dx = 0;
	int dy = 0;
	gfx_windowT* win = (gfx_windowT*)glfwGetWindowUserPointer(window); ;
	
	if (mouse_relative) {
		dx = ((zint32)x) - last_mouse_x;
		dy = ((zint32)y) - last_mouse_y;
		
	}


	last_mouse_x = (zuint32)x;
	last_mouse_y = (zuint32)y;

	if (mouse_relative) {
		zw_enqueue(&win->iface, ZEVENT_MOUSE | ZEVENT_DELTA | mouse_button_state | keymodstate, (zuint32)dx, (zuint32)dy, NULL);
		
		return;
	}

	zw_enqueue(&win->iface, ZEVENT_MOUSE | ZEVENT_MOVE | mouse_button_state | keymodstate, (zuint32) last_mouse_x, (zuint32) last_mouse_y, NULL );
}


void mousebuttonHandler(GLFWwindow* window, int button, int action, int mods) {

	gfx_windowT* win = (gfx_windowT*)glfwGetWindowUserPointer(window);
	zuint32 event = ZEVENT_MOUSE;
	zuint32 statebit = 0;
	switch (button) {

	case GLFW_MOUSE_BUTTON_LEFT:
		event |= ZEVENT_MOUSE_L;	//for the event of pressing
		statebit = ZEVENT_MOUSE_STATE_L; //for the continous state of being pressed
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		event |= ZEVENT_MOUSE_R;
		statebit = ZEVENT_MOUSE_STATE_R;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		event |= ZEVENT_MOUSE_M;
		statebit = ZEVENT_MOUSE_STATE_M;
		break;

	default:
		return; //don't sent events for things we don't know

	}

	if (action == GLFW_PRESS) {

		mouse_button_state |= statebit;  //set mouse button state bit

		event |= ZEVENT_DOWN;  

	}
	else { //release

		mouse_button_state &= (~statebit);  //set clear state bit

		event |= ZEVENT_UP;

	}

	if (mouse_relative)
		zw_enqueue(&win->iface, event | mouse_button_state | keymodstate, 0, 0, NULL);  //button clicks don't move mouse
	else
		zw_enqueue(&win->iface, event | mouse_button_state | keymodstate, (zuint32)last_mouse_x, (zuint32)last_mouse_y, NULL);
	
}

/* one-time initialization */
zbool gfxi_inited = ZFALSE;
void gfxi_init() {
	
	if (!glfwInit()) {
		printf(" Can't init glfw\n");
		return;

	}
	glfwSetErrorCallback(errorHandler);

	gfxi_inited = ZTRUE;
}

//event interface
 zbool gfx_event(zwindowT * zw, zeventT * ev) {
	
	
	glfwPollEvents(); //enqueue events


	//read from queue first
 	if (zw_event(zw, ev)) {
		printf(" RETURNING QUEUED EVENT\n");
		return ZTRUE;
	}

	gfx_windowT* win = (gfx_windowT*)zw;

	if (glfwWindowShouldClose(win->fwindow))  {
		//glfw user is trying to close window
		ev->type = ZEVENT_CLOSE;
		return ZFALSE;
	}
		

	ev->type = ZEVENT_NONE;
	return ZFALSE;
}

void gfx_pixels(zwindowT * zw, void* px) {
	gfx_windowT* win = (gfx_windowT*)zw;

	if (px == NULL)
		glfwSwapBuffers(win->fwindow);
	else {
		printf(" non-framebuffer pixels not supported ");
	}

	//get the window size and fix the viewport
	glfwGetWindowSize(win->fwindow, &win->iface.w, &win->iface.h);
	glViewport(0, 0, win->iface.w, win->iface.h);
	//the above is also updating the 'w' and 'h' coordinates, so the app can use them in drawing the next frame, if they are adapting to window size


	checkGL();//check for errors
}

void gfx_close(zwindowT * zw) {
	printf(" 'close' interface not supported in opengl.  click the 'x' manually\n");
}

//flags currently don't do anything
//creation of first window will init glfw

zwindowT* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags) {

	memset(keystatus, 0, sizeof(keystatus));

	gfx_windowT* win = ram_alloc(sizeof(gfx_windowT), NULL); //no destructor key

	if (!gfxi_inited)
		gfxi_init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 4);  //enable antialiasing buffers


	win->fwindow = glfwCreateWindow(w, h, title, NULL, NULL);
	glfwSetWindowUserPointer(win->fwindow, win); //

	glfwSetKeyCallback(win->fwindow, keyHandler);
	glfwSetCharCallback(win->fwindow, charHandler);
	glfwSetCursorPosCallback(win->fwindow, mousemoveHandler);
	glfwSetMouseButtonCallback(win->fwindow, mousebuttonHandler);
	
	glfwMakeContextCurrent(win->fwindow);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Can't init glad\n");
	}

	glEnable(GL_MULTISAMPLE); //enable antialiasing


	gfx_identity(); //clear the matrix

	win->iface.close = gfx_close;
	win->iface.pixels = gfx_pixels;
	win->iface.event = gfx_event;

	//get the window size and fix the viewport
	glfwGetWindowSize(win->fwindow, &win->iface.w, &win->iface.h);
	glViewport(0, 0, win->iface.w, win->iface.h);
	glClearColor(0, 0, 0, 1); //black window default
	
	checkGL();

	return &(win->iface);
}

/*frame clear functions */

void gfx_background_color(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void gfx_frame_clear(zbool color, zbool depth)
{
	glClear(
		(color ? GL_COLOR_BUFFER_BIT : 0)
		|
		(depth ? GL_DEPTH_BUFFER_BIT : 0)
	);
}



/* Some simple setup functions*/

void gfx_depth_buffer(zbool test, zbool write) {

	if (test || write) {
		glEnable(GL_DEPTH_TEST);

		if (test)
			glDepthFunc(GL_LEQUAL);
		else
			glDepthFunc(GL_ALWAYS);

		if (write)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);

	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	

}

void gfx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist)
{

	//projection matrix
	gfx_projection3d(fovy, aspect, neardist, fardist);

	//by default, a full range depth buffer
	glClearDepth(1.0); //when clearing depth buffer, set to infinity
	glDepthRange(0, 1);  //set range for full depth bufer
	glDepthFunc(GL_LEQUAL);  //draw things equally far or closer
	glDepthMask(GL_TRUE); //write to depth bufer
	glEnable(GL_DEPTH_TEST);  //enable depth testing


	

}


void gfx_setup_2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom)
{

	//projection matrix (ortho 2d)
	gfx_projection2d(left, right, top, bottom);

	//depth buffer disabled
	glDepthMask(GL_FALSE);		//don't write to depth bufer
	glDisable(GL_DEPTH_TEST);	//don't enable depth testing

	gfx_identity();  //modelview matrix is reset to identity
}

/* Simple Shaders */

//data types
#define GFX_FLOAT		0x10000000
#define GFX_FLOAT2		0x20000000
#define GFX_FLOAT3		0x30000000
#define GFX_FLOAT4		0x40000000
#define GFX_INT			0x50000000
#define GXI_TYPEMASK	0xff000000

#define GXI_BLEND_MODE	(GFX_INT  |  1)
#define GFX_BLEND_OFF	 1
#define GFX_BLEND_ALPHA	 2
#define GFX_BLEND_ADD	 3
#define GFX_BLEND_MUL	 4

#define GXI_LIGHT_DIRECTION	(GFX_FLOAT3 | 2)
#define GXI_LIGHT_COLOR		(GFX_FLOAT4 | 3)
#define GXI_LIGHT_AMBIENT	(GFX_FLOAT4 | 4)

typedef struct gfx_propertyS {
	char* name;//user can name custom properties
	int id;
	int index;  //support multiple values of same kind of data (texture 0, texture 1... etc)
	int uloc;  //if using shaders, uniform location
	union {
		float f;	//single float  
		float fa[4]; //up to 4, for color, etc
		vec3 v; //3 component vector (position)
		vec4 v4; //3 component vector (position)
		int i;
		//todo: pointer to larger data (if necessary)
	} data;
} gfx_propertyT;

char*  gxi_builtin_properties[] = { "invalid" , "blend"        , "light_direction",   "light_color"   , "light_ambient",    NULL };
zuint32 gxi_builtin_prop_id[] =	  { 0         , GXI_BLEND_MODE , GXI_LIGHT_DIRECTION,  GXI_LIGHT_COLOR, GXI_LIGHT_AMBIENT,  0    };

zuint32 gxi_get_prop_id(char* name) {
	zuint32 i;
	for (i = 0; gxi_builtin_properties[i]; i++) {

		if (!strcmp(name, gxi_builtin_properties[i])) {
			//printf(" found builtin %x for %s\n", gxi_builtin_prop_id[i], name);
			return gxi_builtin_prop_id[i];
		}
	}

	return 0;
}



typedef struct gfxstyleS {
	zvecT properties;
	zvecT textures;
} gfx_styleT;

gfx_styleT* gfx_style_mk(gfx_styleT* env) {
	gfx_styleT* st = ram_alloc(sizeof(gfx_styleT), NULL);

	zvec_mk(&st->properties, 4);
	zvec_mk(&st->textures, 4);

	return st;
}

#define GFX_DELETE 1

void gfx_style_set_property(gfx_styleT* st, int id, char* name_in , int index, int val, void* ptr, int action ) {

	char* name = name_in;
	int prop_id = gxi_get_prop_id(name);
	if (prop_id){
		id = prop_id;
		name = NULL; //drop name, since we have an exact integer id now
	}

	zuint32 i;
	zuint32 ifound=0xFFFF; //invalid
	gfx_propertyT* p = NULL;

	for (i = 0; i < zvec_count(&st->properties); i++) {

		gfx_propertyT* psearch = zvec_get_at(&st->properties, i);  
		
		//if named check index matches
		if (name && psearch->name) {
			if ((psearch->index == index) && (!strcmp(psearch->name, name))) {
				//found property
				p = psearch;
				ifound = i;
				break;
			}
		}

		//unnamed
		if (!name && !(psearch->name)) {
			if ((id == psearch->id) && (index == psearch->index)) {
				p = psearch;
				ifound = i ;
				break;
			}

		}
		
		
	}

	if (p)
		printf("Found existing property %x #%d for %s #%d\n", p->id, p->index, name_in, index);

	if (action == GFX_DELETE) {
		if (ifound != 0xFFFF)
			zvec_remove_unordered(&st->properties, i);

		printf(" delete property %x \n", ifound);
		return;  
	}

	if (!p) {
		p = ram_alloc(sizeof(gfx_propertyT), NULL);
		if (name)
			p->name = zstrdup(name);
		p->uloc = -1;
		p->id = id;
		p->index = index;

		printf("New property %x #%d for %s #%d\n", p->id, p->index, name_in, index);

		zvec_add_or_free(&st->properties, p); 
	}
	
	if (!p)
		return;


	switch (p->id & GXI_TYPEMASK) {

	case GFX_INT:
		p->data.i = val;
		break;


	case GFX_FLOAT3:
		p->data.v = *(vec3*)ptr;
		break;

	case GFX_FLOAT4:
		p->data.v4 = *(vec4*)ptr;
		break;


	default:
		printf(" unknown property type\n");

	}


}


void gxi_set_blend(int m) {


	if (m == GFX_BLEND_OFF) {
		glDisable(GL_BLEND);
		return;
	}


	glEnable(GL_BLEND);

	switch (m) {

	case GFX_BLEND_ALPHA:

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		break;
	case GFX_BLEND_ADD:
		glBlendFunc(GL_ONE, GL_ONE);
		break;

	case GFX_BLEND_MUL:
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);


	}

}


void gfx_style(gfx_styleT* st) {

	zuint32 i;
	gfx_propertyT* p;

	int ff_lights_used = ZFALSE;

	vec4 ambientsum = vec4const(0,0,0,1);

	for (i = 0; i < zvec_count(&st->properties); i++) {

		p = zvec_get_at(&st->properties, i);

		//builtins

		switch (p->id) {

		case GXI_BLEND_MODE:

			gxi_set_blend(p->data.i);

			break;

		case GXI_LIGHT_DIRECTION: //a directional light

			ff_lights_used = ZTRUE;
			glLoadIdentity();
			p->data.v4.named.w = 0.0; //direction light has position at w=0 'infinity' away
			glLightfv(GL_LIGHT0 + p->index, GL_POSITION, &p->data.v4);
			glEnable(GL_LIGHT0);
			break;
	
		case GXI_LIGHT_COLOR:
			glLightfv(GL_LIGHT0 + p->index, GL_DIFFUSE, &p->data.v4);
			glLightfv(GL_LIGHT0 + p->index, GL_SPECULAR, &p->data.v4);
			//vec4 zero = vec4const(0, 0, 0, 1);
			//glLightfv(GL_LIGHT0 + p->index, GL_SPECULAR, &zero);
			break;

		case GXI_LIGHT_AMBIENT:
			vec4add(ambientsum, p->data.v4);
			
			break;

		}
	}

	if (ff_lights_used) {
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		ambientsum.VALPHA = 1.0;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientsum.array);
				
		//have color changes change the material settings
		glColor4f(1, 1, 1, 1);  //if it happens there is no color vertex array data, use white as the color (which gets multiplied against the texture)
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		
	}
	else {
		glDisable(GL_LIGHTING);
	}

	

	gxi_texture_set_enable(&st->textures);
	
}


zuint16* gfx_vertex_buffer_add_index(gfx_vertex_bufferT* vb, int num) {

	if (vb) {
		vb->index_buffer = zarray_alloc(zuint16, num);

		return vb->index_buffer; //the index, or null
	}
	return 0;
}

zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb,  zuint16 a, zuint16 b, zuint16 c) {

	zarray_add(vb->index_buffer, a);
	zarray_add(vb->index_buffer, b);
	zarray_add(vb->index_buffer, c);
	return zarray_count(vb->index_buffer);
}

gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount_in, char* spec) {

	char* s = spec;
	zuint32 vcount = vcount_in;
	if (!s)
		return NULL;
	
	gfx_vertex_bufferT* vb = ram_alloc(sizeof(gfx_vertex_bufferT), NULL); //no destructor yet

	while (*s) {

		char* ne = strchr(s, ':');
		if (!ne)
			break;
		char* name = zstrndup(s, (ne - s));
		int size = atoi(ne + 1);	//size if number of floats.  If we ever have integer vertex attributes, instead of :2, etc can do :i2 or whatever

		ne = strchr(s, '|');

		if (!size)
			break;

		//add the attribute
		printf(" name is [%s] size is [%d]", name, size);

		vb->attributes[vb->num_attributes].name = name;
		vb->attributes[vb->num_attributes++].type = size; //simple numbers 1 to 4 are just floats.  TODO: non-float attributes?
		vb->fcount += size;

		if (!ne)
			break;

		s = ne + 1;
	}

	printf(" There are %d float components by %d vertices\n", vb->fcount, vcount);

	//allocate the buffer
	vb->combined_data = ram_alloc(sizeof(float) * vcount * vb->fcount, NULL);

	//set attribute data pointers
	vb->fixed_position = -1; //not valid
	vb->fixed_color = -1; //not valid
	vb->fixed_normal = -1; //not valid
	vb->fixed_texcoord = -1; //not valid
	int i;
	float* fp = vb->combined_data;
	for (i = 0; i < vb->num_attributes; i++) {
		vb->attributes[i].data = fp;
		printf(" Set ptr to %s  base+%d\n", vb->attributes[i].name, (int) (fp - vb->combined_data) );
		fp += vb->attributes[i].type * vcount;

		//some vertex attributes are special (can be used with fixed function pipeline.  If ever target old computers, or if I want to implement some generic default behavior with a default shader)
		if (!strcmp(vb->attributes[i].name, "color")) {
			vb->fixed_color = i;
		}
		if (!strcmp(vb->attributes[i].name, "position")) {
			vb->fixed_position = i;
		}
		if (!strcmp(vb->attributes[i].name, "normal")) {
			vb->fixed_normal = i;
		}
		if (!strcmp(vb->attributes[i].name, "texcoord")) {
			vb->fixed_texcoord = i;
		}
	}

	vb->capacity = vcount;

	return vb;

}

void gfx_vertex_data(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d) {

	int  pos = vb->count * vb->attributes[attr].type;
	//printf(" Setting to attribute %d at %d", attr, pos);
	if (vb->count > vb->capacity) {
		printf("vertex buffer overflow\n");
		exit(1);
	}

	switch (vb->attributes[attr].type) {
		case 4: vb->attributes[attr].data[pos + 3] = d;		//printf("@3");
		case 3: vb->attributes[attr].data[pos + 2] = c; // printf("@2");
		case 2: vb->attributes[attr].data[pos + 1] = b; // printf("@1");
		case 1: vb->attributes[attr].data[pos + 0] = a;	// printf("@0");
	}

//	printf("\n");
}

zuint16 gfx_vertex_done(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d) {
	gfx_vertex_data(vb, attr, a, b, c, d);
	return vb->count++;
}



//track which vbo is active
//only set to nonzery when the vertex attribs are set to this vbo as well
int gxi_current_vbo = 0;
int gxi_current_index_vbo = 0;



//Call after modifying vertex buffer data
void gfx_vertex_buffer_update(gfx_vertex_bufferT* vb) {
	if (!vb)
		return;
	checkGL();

	if (!vb->vbo) {
		//create VBO

		glGenBuffers(1, &(vb->vbo));
		printf(" Generated VBO %d\n", vb->vbo);
	
	}

	if (vb->index_buffer && (zarray_count(vb->index_buffer) >0 )) {

		if (!vb->index_vbo) {
			glGenBuffers(1, &(vb->index_vbo));
			printf(" Generated index VBO %d\n", vb->vbo);
		}
		//send index data, if we have it
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, zarray_count(vb->index_buffer) * sizeof(vb->index_buffer[0]), vb->index_buffer, GL_DYNAMIC_DRAW);
		printf("send %d index values to vbo\n", zarray_count(vb->index_buffer));
		gxi_current_index_vbo = vb->index_vbo;
	}



	//switch to buffer's vbo and send data
	glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(zfloat32) * vb->capacity * vb->fcount, vb->combined_data, GL_DYNAMIC_DRAW);
	gxi_current_vbo = 0; //set to zero, so first draw sets up the vertex arrays
}


//using fixed function or not
zbool gxi_fixed_function = ZTRUE; //set to true 



#define GFX_POINT	1
#define GFX_LINE	2
#define GFX_TRIANGLE	3
zuint32 gl_prims[] = { 0, GL_POINTS, GL_LINES, GL_TRIANGLES };

int max_attrs_active;

void gfx_vertex_buffer_draw(gfx_vertex_bufferT* vb, int prim, int start, int end, zbool indexed) {

	zbool setup_arrays = ZFALSE;

	if (!vb)
		return;

	if (!vb->vbo)
		gfx_vertex_buffer_update(vb);	//update if a vbo was never made for this object


	if (gxi_current_vbo != vb->vbo) {
		glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
		gxi_current_vbo = vb->vbo;
		setup_arrays = ZTRUE;  //need to setup vertex arrays
	}

	if (gxi_current_index_vbo != vb->index_vbo) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->index_vbo);
		gxi_current_index_vbo = vb->index_vbo;
	}

	//TODO: check if the shader changed, if so, setup arrays
	gxi_refresh_matrix();
	
	if (setup_arrays) {


		if (gxi_fixed_function) {

			//set each attribute - fixed function
			if (vb->fixed_position != -1) {

				glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), (void*)((vb->attributes[vb->fixed_position].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_VERTEX_ARRAY);
			} else
				glDisableClientState(GL_VERTEX_ARRAY);
						
			if (vb->fixed_color != -1) {
				glColorPointer(4, GL_FLOAT, 4 * sizeof(float), (void*)((vb->attributes[vb->fixed_color].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_COLOR_ARRAY);
			} else
				glDisableClientState(GL_COLOR_ARRAY);
							
			if (vb->fixed_normal != -1) {
				glNormalPointer(GL_FLOAT, 3 * sizeof(float), (void*)((vb->attributes[vb->fixed_normal].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_NORMAL_ARRAY);
			} else 
				glDisableClientState(GL_NORMAL_ARRAY);
			
			if (vb->fixed_texcoord != -1) {
				glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(float), (void*)((vb->attributes[vb->fixed_texcoord].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			} else
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		}
		else {
			//setup arrays for shader use (all atribs)

		}

	}

	if (indexed)
 		glDrawElements(gl_prims[prim], end - start, GL_UNSIGNED_SHORT, (void*) (sizeof(zuint16) * start));
	else
		glDrawArrays(gl_prims[prim], start, end - start);

	checkGL();
}

/* higher level meshes*/

typedef struct gfx_mesh_sectionS {
	
	gfx_styleT* style;
	gfx_vertex_bufferT* buffer;
	zbool indexed;
	zuint32 startVertex;
	zuint32 endVertex;

} gfx_mesh_sectionT;


/* test program */

void gfx_gl_test() {
	
	zwindowT* zwin = gfx_mkwindow("internal test", 1024, 768, 0);

	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/label.tga", ZTGA_TOP);
	//zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/earth-cylindrical-alpha-holes.tga", ZTGA_TOP);
	zbitmapT* pic = zbitmap_load_tga("../../Zcore-data/web/strawberry/Texture/Strawberry_basecolor.tga", 0*ZTGA_TOP);
	
	
	gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/web/strawberry/Strawberry_obj.obj");
	//gfx_meshT* strawberry_mesh = gfx_mesh_load_obj("../../Zcore-data/cube.obj");

	printf(" loaded %x %d %d %d\n", pic->format, pic->w, pic->h, pic->size);

	gfx_windowT* gfx_window = (gfx_windowT*)zwin; //cast to our own specific type
		
	gfx_textureT* tex = gfx_texture_mk(pic);
		
	gfx_styleT* st = gfx_style_mk(NULL);
	
	gfx_style_set_property(st, 0, "blend", 0, GFX_BLEND_ALPHA, NULL, 0);

	vec3 lpcam = vec3const(1, 1, 1);
	vec4 lcol = vec4const(1, 1, .8, 1.0);
	vec4 lam = vec4const(.2, .2, .2, 1.0);

	gfx_style_set_property(st, GFX_FLOAT3, "light_direction", 0, 0, &lpcam, 0);
	gfx_style_set_property(st, GFX_FLOAT4, "light_color", 0, 0, &lcol, 0);
	gfx_style_set_property(st, GFX_FLOAT4, "light_ambient", 0, 0, &lam, 0);

	//GLFWwindow* window = gfx_window->fwindow;


	zvec_add(&st->textures, tex);

 	gfx_style(st);

	gfx_vertex_bufferT* vb = gfx_vertex_buffer_mk(10000, "color:4|position:3|texcoord:2|normal:3");
	gfx_vertex_buffer_add_index(vb, 10000);


	gfx_cameraT cam;
 	gfx_camera_init(&cam);


	//junky sphere

	int j, k;
	int v=0;
	int vc = 0;
	for (j = -10; j <= 10; j++) {
		for (k = -10; k <= 10; k++) {

			float fy = j / 10.0;

			float s = sqrtf(1- fy*fy);

			float fx = s*sinf(k /10.0 *3.141);
			float fz = s*cosf(k /10.0 * 3.141);

			gfx_vertex_data(vb, 0, 1, 1, 1, 1);
			gfx_vertex_data(vb, 2,  k/20.0  , -(j + 10) / 20.0, 0, 1);
 			gfx_vertex_data(vb, 3, fx, fy, fz, 1);
			gfx_vertex_done(vb, 1, fx, fy, fz, 0);
			

			if (k < 10 && j < 10) {
				v=gfx_index_triangle(vb, vc, vc + 1, vc + 21);
				v = gfx_index_triangle(vb, vc+1, vc + 21, vc + 22);
			}
			vc++;
			
		}
	}
		
	zeventT ev;
	
	float ang = 0;
	zbool mr = ZFALSE;

	for (;;) {
		 
		float yaw = 0;
		float pitch = 0;
		float roll = 0;

		while (zwin->event(zwin, &ev)) {

			//printf(" ZEVENT %x %x %x %c     %x\n", ev.type, ev.a, ev.b, ev.a, ZKEY_CTRL);
			zprintevent(&ev);
			

			if (ZEVENTIS(ev.type ,ZEVENT_CHAR)) {

			//	printf(" ZEVENT CHAR %x %x %x %c     %x\n", ev.type, ev.a, ev.b, ev.a, ZKEY_CTRL);
				
				if (ev.a == 'm') 
					gfx_mouse_relative(zwin, mr ^= 1);
								
			}


			if (ZEVENTIS(ev.type,  ZEVENT_KEY|ZEVENT_DOWN|ZKEY_CTRL   ) &&(ev.a=='q') ) {
				printf("CLOSE\n");
				exit(1);
				break;
			}

			if (ZEVENTIS(ev.type, ZEVENT_DELTA)) {
			
				pitch -= ((zint32)ev.b ) / 200.0;
				yaw   -=  ((zint32)ev.a ) / 200.0;

			}
		}

		if (ev.type == ZEVENT_CLOSE)
			break;
	
		float forward = 0.0;
		float right = 0.0;
		float up = 0.0;
		float speed = .01;

		if (gx_keystate('a')) right -= speed;
		if (gx_keystate('d')) right += speed;
		if (gx_keystate('w')) forward += speed;
		if (gx_keystate('s')) forward -= speed;
		if (gx_keystate('r')) up += speed;
		if (gx_keystate('f')) up -= speed;

		if (gx_keystate('q'))  roll -= .01;
		if (gx_keystate('e'))  roll += .01;
				
		gfx_camera_motion_6dof(&cam, forward, right, up, yaw, pitch, roll);

		//printf(" Window size is %d %d\n", w, h);
		gfx_background_color(.3, .2, .1, 0);
		gfx_frame_clear(ZTRUE, ZTRUE);
		
		gfx_setup_3d(80,  (float)zwin->w / (float) zwin->h , .1, 1000);
	//	gfx_depth_buffer(ZFALSE, ZFALSE);
		
		gfx_camera_view(&cam);

		gfx_translate3(0, 0, -2);
		//gfx_rotate_y(ang);
		
		
	//	gfx_vertex_buffer_draw(vb, GFX_TRIANGLE, 0, v, ZTRUE);
	
		gfx_meshT* m = strawberry_mesh;

		while (m) {

			gfx_vertex_buffer_draw(m->vb, GFX_TRIANGLE, 0, zarray_count(m->vb->index_buffer), ZTRUE);
			m = m->next_piece;
		}
		
		zwin->pixels(zwin, NULL);	//display the framebuffer
		ang += .01;
	}

	printf(" window close button was pressed\n");
	

}