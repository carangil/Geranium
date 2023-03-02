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

/* Frame clear Function */

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
/*
these are defined in gfx_gl.h
#define GFX_FLOAT		0x10000000
#define GFX_FLOAT2		0x20000000
#define GFX_FLOAT3		0x30000000
#define GFX_FLOAT4		0x40000000
#define GFX_INT			0x50000000

*/
#define GXI_TYPEMASK	0xff000000

#define GXI_BLEND_MODE	(GFX_INT  |  1)


/* light DIRECTION and POSITION for the same 'n' are mutually exclusive! */
#define GXI_LIGHT_DIRECTION	(GFX_FLOAT3 | 2)	
#define GXI_LIGHT_POSITION	(GFX_FLOAT3 | 3)
#define GXI_LIGHT_COLOR		(GFX_FLOAT4 | 4)
#define GXI_LIGHT_AMBIENT	(GFX_FLOAT4 | 5)


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

char* gxi_builtin_properties[] = { "invalid" , "blend"        , "light_direction",   "light_color"   , "light_ambient",   "light_position",  NULL };
zuint32 gxi_builtin_prop_id[] = { 0         , GXI_BLEND_MODE , GXI_LIGHT_DIRECTION,  GXI_LIGHT_COLOR, GXI_LIGHT_AMBIENT,  GXI_LIGHT_POSITION, 0 };

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




gfx_styleT* gfx_style_mk() {
	gfx_styleT* st = ram_alloc(sizeof(gfx_styleT), NULL);

	zvec_mk(&st->properties, 4);
	zvec_mk(&st->textures, 4);

	return st;
}



void gfx_style_set_property(gfx_styleT* st, int id, char* name_in, int index, int val, void* ptr, int action) {

	char* name = name_in;
	int prop_id = gxi_get_prop_id(name);
	if (prop_id) {
		id = prop_id;
		name = NULL; //drop name, since we have an exact integer id now
	}

	zuint32 i;
	zuint32 ifound = 0xFFFF; //invalid
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
				ifound = i;
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

#define MAX_FF_LIGHTS 4


void gfx_style(gfx_styleT* st) {

	zuint32 i;
	gfx_propertyT* p;

	int ff_lights_used = ZFALSE;
	zbool ff_lights_active[MAX_FF_LIGHTS];
	memset(ff_lights_active, 0, sizeof(ff_lights_active));

	vec4 ambientsum = vec4const(0, 0, 0, 1);

	for (i = 0; i < zvec_count(&st->properties); i++) {

		p = zvec_get_at(&st->properties, i);

		//builtins

		switch (p->id) {

		case GXI_BLEND_MODE:

			gxi_set_blend(p->data.i);

			break;

		case GXI_LIGHT_DIRECTION: //a directional light

			ff_lights_active[p->index] = ZTRUE;

			ff_lights_used = ZTRUE;
			glPushMatrix();
			glLoadIdentity();
			p->data.v4.named.w = 0.0; //direction light has position at w=0 'infinity' away
			glLightfv(GL_LIGHT0 + p->index, GL_POSITION, p->data.fa);
			glEnable(GL_LIGHT0 + p->index);
			glPopMatrix();
			break;


		case GXI_LIGHT_POSITION: //a positional light

			if (p->index >= MAX_FF_LIGHTS)
				continue;
			ff_lights_active[p->index] = ZTRUE;
			ff_lights_used = ZTRUE;
			glPushMatrix();
			glLoadIdentity();
			p->data.v4.named.w = 1.0; //w=1 defines an exact point
			glLightfv(GL_LIGHT0 + p->index, GL_POSITION, p->data.fa);
			glEnable(GL_LIGHT0 + p->index);
			glPopMatrix();
			break;

		case GXI_LIGHT_COLOR:

			if (p->index >= MAX_FF_LIGHTS)
				continue;

			glLightfv(GL_LIGHT0 + p->index, GL_DIFFUSE, p->data.fa);
			glLightfv(GL_LIGHT0 + p->index, GL_SPECULAR, p->data.fa);
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

		//disable and ff lights that are not being used anymore
		for (i = 0; i < MAX_FF_LIGHTS; i++) {
			if (!ff_lights_active[i]) {
				printf(" dis light %d\n", i);
				glDisable(GL_LIGHT0 + i);
			}

		}

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

zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb, zuint16 a, zuint16 b, zuint16 c) {

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
		printf(" Set ptr to %s  base+%d\n", vb->attributes[i].name, (int)(fp - vb->combined_data));
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

	if (vb->index_buffer && (zarray_count(vb->index_buffer) > 0)) {

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


/*
#define GFX_POINT	1
#define GFX_LINE	2
#define GFX_TRIANGLE	3
*/
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
			}
			else
				glDisableClientState(GL_VERTEX_ARRAY);

			if (vb->fixed_color != -1) {
				glColorPointer(4, GL_FLOAT, 4 * sizeof(float), (void*)((vb->attributes[vb->fixed_color].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_COLOR_ARRAY);
			}
			else
				glDisableClientState(GL_COLOR_ARRAY);

			if (vb->fixed_normal != -1) {
				glNormalPointer(GL_FLOAT, 3 * sizeof(float), (void*)((vb->attributes[vb->fixed_normal].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_NORMAL_ARRAY);
			}
			else
				glDisableClientState(GL_NORMAL_ARRAY);

			if (vb->fixed_texcoord != -1) {
				glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(float), (void*)((vb->attributes[vb->fixed_texcoord].data - vb->combined_data) * sizeof(zfloat32)));
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			else
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		}
		else {
			//setup arrays for shader use (all atribs)

		}

	}

	if (indexed)
		glDrawElements(gl_prims[prim], end - start, GL_UNSIGNED_SHORT, (void*)(sizeof(zuint16) * start));
	else
		glDrawArrays(gl_prims[prim], start, end - start);

	checkGL();
}
