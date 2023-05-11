#define GFXINTERNAL
#include "ztypes.h"
#include "zmem.h"
#include "zwindow.h"
#include "gfx_gl.h"
#include "zvector.h"
#include "zstring.h"
#include "string.h"
#include "string.h"
#include "zarray.h"
#include <stdio.h>
#include "ff.h"

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



char* gxi_builtin_properties[] = { "invalid" , "blend"        , "light_direction",   "light_color"   , "light_ambient",   
"light_position", "texture_diffuse", "specular_exponent", "specular", "light_attenuation", "fog_color", "fog_density", NULL };

zuint32 gxi_builtin_prop_id[] = { 0         , GXI_BLEND_MODE , GXI_LIGHT_DIRECTION,  GXI_LIGHT_COLOR, GXI_LIGHT_AMBIENT,  
GXI_LIGHT_POSITION, GXI_TEXTURE_DIFFUSE, GXI_SPECULAR_EXPONENT, GXI_SPECULAR_COLOR, GXI_LIGHT_ATTENUATION, GXI_FOG_COLOR, GXI_FOG_DENSITY , 0 };


zuint32 gxi_get_prop_id(char* name) {
	zuint32 i;
	for (i = 0; gxi_builtin_properties[i]; i++) {

		if (!strcmp(name, gxi_builtin_properties[i])) {
			gxdtracef(" found builtin %x for %s\n", gxi_builtin_prop_id[i], name);
			return gxi_builtin_prop_id[i];
		}
	}

	return 0;
}

zbool freestyle(void* v) {
	gfx_styleT* st = v;

	zvec_cleanup(&st->properties);
	ram_free(st->shader_group);

	return ZTRUE;
}


gfx_styleT* gfx_style_mk() {
	gfx_styleT* st = ram_alloc(sizeof(gfx_styleT), freestyle);

	zvec_mk(&st->properties, 4);

	st->style_dirty = ZTRUE; //need to regenerate the style key

	return st;
}

zbool freeprop(void* v) {

	gfx_propertyT* p = v;

	if ((p->id & GXI_TYPEMASK) == GFX_TEXTURE)
		ram_free(p->data.tex);

	ram_free(p->name);
}


void gfx_style_set_property(gfx_styleT* st, int id, char* name_in, int index, int val, void* ptr, int action) {

	char* name = name_in;
	int prop_id = gxi_get_prop_id(name);
	if (prop_id) {
		id = prop_id;
//		name = NULL; //drop name, since we have an exact integer id now
	}
	st->style_dirty = ZTRUE; //style has changed
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
		/*
		//unnamed
		if (!name && !(psearch->name)) {
			if ((id == psearch->id) && (index == psearch->index)) {
				p = psearch;
				ifound = i;
				break;
			}

		}
		*/


	}

	
	if (p)
		gxdtracef("Found existing property %x #%d for %s #%d\n", p->id, p->index, name_in, index);
		

	if (action == GFX_DELETE) {
		if (ifound != 0xFFFF) {
			p = zvec_remove_unordered(&st->properties, i);
			ram_free(p);
		}

		gxdtracef(" delete property %x \n", ifound);
		return;
	}

	if (!p) {
		p = ram_alloc(sizeof(gfx_propertyT), freeprop);
		if (name)
			p->name = zstrdup(name);
		p->uloc = -1;
		p->id = id;
		p->index = index;

		gxdtracef("New property %x #%d for %s #%d\n", p->id, p->index, name_in, index);

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

	case GFX_FLOAT:
		p->data.f = *(float*)ptr;
		break;

	case GFX_FLOAT4:
		p->data.v4 = *(vec4*)ptr;
		break;

	case GFX_TEXTURE:
		p->data.tex = ram_addref(ptr);
		break;

	case GFX_SWITCH:
		//nothing to set 
		break;
	default:
		printf(" unknown property type\n");
		getc(stdin);
	}

	if (action == GFX_TRANSFORM_DIRECTION)
		gfx_trans_dir_vec3(&p->data.v);

	if (action == GFX_TRANSFORM_POINT)
		gfx_trans_vec3(&p->data.v);

	

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

gfx_styleT* next_style=NULL;
gfx_styleT* enabled_style=NULL;
gx_shader_variantT* enabled_variant = NULL;
gfx_styleT default_style;

void gfx_style(gfx_styleT* st) {
	next_style = st;	//next draw commands will use this style
}

gx_shader_variantT* gxi_enable_style_parameters(gfx_vertex_bufferT* vb){

	zuint32 i;
	gfx_propertyT* p;


	gfx_styleT* st = next_style;

	if (!st) {
		
		st = &default_style;

	}

	zbool ambient_valid = ZFALSE;
	vec4 ambientsum = vec4const(0, 0, 0, 1);

	char* key = zstr_mk(100);


	for (i = 0; i < zvec_count(&st->properties); i++) {
		char b[100];
		p = zvec_get_at(&st->properties, i);

		char* name = p->name;
			
		snprintf(b, sizeof(b) , "%s%d|", name, p->index);
		key = zstrcat(key, b);

	}
	//todo: save the key-so-far as the style_key

	key = zstrcat(key, vb->buffer_spec);
			
 	gx_shader_variantT* variant = gx_shader_variant(st->shader_group, key, st, vb);  //select 'cached' shader variant for key, or create it using st and vb
	
	ram_free(key);

	enabled_variant = variant;
		

	gxi_new_texture_set();

	if (!variant)	//fixed function light
		ff_new_light_set();

	
	checkGL();

	for (i = 0; i < zvec_count(&st->properties); i++) {

		p = zvec_get_at(&st->properties, i);

		//builtins

		if (!variant && ff_light_parm(p))  //if ff lighting can accept the value, let it take it
			continue;


		switch (p->id) {

		case GXI_LIGHT_AMBIENT:
			vec4add(ambientsum, p->data.v4);
			continue;

		case GXI_BLEND_MODE:

			gxi_set_blend(p->data.i);

			continue;

		case GXI_TEXTURE_DIFFUSE:

			if (variant)
				break;  //break down to the 'if variant' below

			//line only executed for FF textures:
			zuint32 tu = gxi_add_texture(p->data.tex);  //add texture AND get the texture unit number
				
			continue;

		case GXI_LIGHT_POSITION:
		case GXI_LIGHT_DIRECTION:
			ambient_valid = ZTRUE;
			break; //continue thru to 'variant' check, because POS and DIR also need to be passed to shader
			

		}

		if (variant) {
			char b[50];
			snprintf(b, sizeof(b), "%s%d", p->name, p->index);

			if (variant->uloc[i] != -1) {

				if ((p->id & GXI_TYPEMASK) == GFX_FLOAT)
					glUniform1f(variant->uloc[i], p->data.f);
				else if ((p->id & GXI_TYPEMASK) == GFX_FLOAT3)
					glUniform3fv(variant->uloc[i], 1, p->data.fa);
				else if ((p->id & GXI_TYPEMASK) == GFX_FLOAT4)
					glUniform4fv(variant->uloc[i], 1, p->data.fa);
				else if ((p->id & GXI_TYPEMASK) == GFX_TEXTURE) {


					zuint32 tu = gxi_add_texture(p->data.tex);  //add texture AND get the texture unit number

					
					//if using shaders, need to bind it to a sampler
					gxdtracef(" texture unit %d for loc %d\n", tu, variant->uloc[i]);
					if (variant->uloc[i] != -1) {
						glUniform1i(variant->uloc[i], tu);
					}
					


				}else{
					printf(" Unhandled variant uniform: %s %x\n", b, p->id);
					getc(stdin);
				}
			}



		}

	}
	checkGL();

	gxi_texture_complete();

	if (variant) {
		if (variant->ambient_uloc != -1) {

			if (ambient_valid == 0) {
				vec4set(ambientsum, 1, 1, 1, 1);  //white
			}

			glUniform3fv(variant->ambient_uloc, 1, &ambientsum);

		}
	}
	else {
		ff_light_complete(&ambientsum);
	}

	enabled_style = next_style;
	st->style_dirty = ZFALSE;  //no optimization yet, but in the future, changing styles when the style isn't dirty,won't change anything

	return variant;
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

zbool freevb(void* v) {

	gfx_vertex_bufferT* vb = v;

	int i;
	for (i = 0; i < vb->num_attributes;i++) {
		ram_free(vb->attributes[i].name);
	}
	ram_free(vb->buffer_spec);
	ram_free(vb->combined_data);
	ram_free(vb->index_buffer);
	if (vb->vbo)
		glDeleteBuffers(1, &vb->vbo);
	if (vb->index_vbo)
		glDeleteBuffers(1, &vb->index_vbo);

	return ZTRUE;
}

gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount_in, char* spec) {

	char* s = spec;
	zuint32 vcount = vcount_in;
	if (!s)
		return NULL;

	gfx_vertex_bufferT* vb = ram_alloc(sizeof(gfx_vertex_bufferT), freevb); //no destructor yet
	vb->buffer_spec = zstrdup(spec);

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
		gxdtracef(" name is [%s] size is [%d]", name, size);

		vb->attributes[vb->num_attributes].name = name;
		vb->attributes[vb->num_attributes++].type = size; //simple numbers 1 to 4 are just floats.  TODO: non-float attributes?
		vb->fcount += size;

		if (!ne)
			break;

		s = ne + 1;
	}

	gxdtracef(" There are %d float components by %d vertices\n", vb->fcount, vcount);

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
		gxdtracef(" Set ptr to %s  base+%d\n", vb->attributes[i].name, (int)(fp - vb->combined_data));
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
	gxdtracef(" Setting to attribute %d at %d", attr, pos);
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
		gxdtracef(" Generated VBO %d\n", vb->vbo);

	}

	if (vb->index_buffer /*&& (zarray_count(vb->index_buffer) > 0)   */) {

		if (!vb->index_vbo) {
			glGenBuffers(1, &(vb->index_vbo));
			gxdtracef(" Generated index VBO %d\n", vb->vbo);
		}
		//send index data, if we have it
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb->index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, zarray_count(vb->index_buffer) * sizeof(vb->index_buffer[0]), vb->index_buffer, GL_DYNAMIC_DRAW);
		//gxdprintf("send %d index values to vbo\n", zarray_count(vb->index_buffer));
		gxi_current_index_vbo = vb->index_vbo;
	}



	//switch to buffer's vbo and send data
	glBindBuffer(GL_ARRAY_BUFFER, vb->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(zfloat32) * vb->capacity * vb->fcount, vb->combined_data, GL_DYNAMIC_DRAW);
	gxi_current_vbo = 0; //set to zero, so first draw sets up the vertex arrays
}





/*
#define GFX_POINT	1
#define GFX_LINE	2
#define GFX_TRIANGLE	3
*/
zuint32 gl_prims[] = { 0, GL_POINTS, GL_LINES, GL_TRIANGLES };


int max_aloc_active=0;
zbool ff_buffers_in_use = ZFALSE;

#define MAX_ALOC 16
zuint32 last_aloc_use[MAX_ALOC] = { 0 };
zuint32 aloc_use_counter = 0;

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



	gx_shader_variantT* variant = gxi_enable_style_parameters(vb); //This function takes the vbuffer spec, combines it with the current style's spec, and attaches whatever shader to use
	
#ifdef DISABLE_FIXED_FUNCTION
	if (variant == NULL) {
		printf(" FIXED FUNCTION DISABLED\n");
		exit(1);
	}
#endif


	//TODO: check if the shader changed, if so, setup arrays
	checkGL();
	gxi_refresh_matrix(variant);
	checkGL();

	if (setup_arrays) {


		if (!variant) {
			int aloc;
			//stop any attrib use
 			for (aloc = 0; aloc < MAX_ALOC; aloc++) {

				if (last_aloc_use[aloc] ) {
					glDisableVertexAttribArray(aloc);
					last_aloc_use[aloc] = 0; //not used anymore
					gxdtracef(" Disable aloc %d not in use for ff\n", aloc);
				}

			}
			


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

			ff_buffers_in_use = ZTRUE; 

		}
		else {
			//setup arrays for shader use (all atribs)

			//first disable ff if it was used
			if (ff_buffers_in_use) {
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				ff_buffers_in_use = ZFALSE;
			}


			int i;
			int aloc;

			aloc_use_counter++;

			for (i = 0; i < vb->num_attributes; i++) {
				aloc = variant->aloc[i];

				if (aloc != -1) {

					gxdtracef(" %s is aloc %d \n", vb->attributes[i].name, variant->aloc[i]);

					glEnableVertexAttribArray(aloc);
					checkGL();

					checkGL();
					glVertexAttribPointer(aloc,
						vb->attributes[i].type,  /* 1,2,3,4 : to GL it is the number of components */
						GL_FLOAT, /*GL data type*/
						0, vb->attributes[i].type * sizeof(zfloat32), /* normalized, stride. stride 0 means densely packed */
						((char*)vb->attributes[i].data) - ((char*)vb->combined_data));
					//(void*)((vb->attributes[i].data - vb->combined_data)*sizeof(zfloat32)));

					checkGL();
					last_aloc_use[aloc] = aloc_use_counter; //track that we used this attribute location

				}//aloc


			}//end for

			//now disable any attributes we did use, but are not anymore
			for (aloc = 0; aloc < MAX_ALOC; aloc++) {

				if (last_aloc_use[aloc] && (last_aloc_use[aloc] != aloc_use_counter)) {
					glDisableVertexAttribArray(aloc);
					last_aloc_use[aloc] = 0; //not used anymore
					gxdtracef(" Disable aloc %d not in use\n", aloc);
				}

			}

		}

	}

	
	checkGL();
	if (indexed)
		glDrawElements(gl_prims[prim], end - start, GL_UNSIGNED_SHORT, (void*)(sizeof(zuint16) * start));
	else
		glDrawArrays(gl_prims[prim], start, end - start);

	checkGL();
	

}


//temporary vertex buffers
//for 'immediate mode' style




void gfx_vertex_buffer_reset(gfx_vertex_bufferT* vb) {
	vb->count = 0; //reset vertices


}

gfx_vertex_bufferT* gfx_vertex_temp(zwindowT* zw, char* spec) {
	gfx_vertex_bufferT* vb = NULL;
	gfx_windowT* gw = (void*)zw;

	int i;
	for (i = 0; i < zvec_count(gw->tempvbufs); i++) {
		vb = zvec_get_at(gw->tempvbufs, i);
		if (!strcmp(vb->buffer_spec, spec)) {
		//	printf(" Returning previously used buffer\n", vb->buffer_spec);
			gfx_vertex_buffer_reset(vb);
			return vb;
		}
	}

	return zvec_add_or_free(gw->tempvbufs, gfx_vertex_buffer_mk(GFX_MAX_TEMP, spec));
}

//draws contents of vertex buffer AND resets the count to zero
//call after filling temp buffer with geometry
void gfx_vertex_buffer_draw_clear(gfx_vertex_bufferT* vb, zuint32 prim) {

	//special handling: create index buffer to make triangle pairs from the submitted vertices
	if (prim == GFX_QUAD) {
		if (vb->index_buffer == NULL) {
			gfx_vertex_buffer_add_index(vb, GFX_MAX_TEMP * GFX_QUAD); //GFX_QUAD is '6' because it takes 6 indices to draw 2 triangles to make a quad
			int i;
			for (i = 0; i < GFX_MAX_TEMP / 4; i++) {
				/*
				*  1   2
				*
				*  0   3
				* */
				gfx_index_triangle(vb, i * 4 + 0, i * 4 + 1, i * 4 + 2);
				gfx_index_triangle(vb, i * 4 + 0, i * 4 + 2, i * 4 + 3);

			}
		}
		gfx_vertex_buffer_update(vb);
		gfx_vertex_buffer_draw(vb, GFX_TRIANGLE, 0, ((zuint32)vb->count) * 6 / 4, ZTRUE);

	}
	else {
		gfx_vertex_buffer_update(vb);
		gfx_vertex_buffer_draw(vb, prim, 0, vb->count, ZFALSE);
	}

	gfx_vertex_buffer_reset(vb);
}
//calls gfx_vertex_buffer_draw_clear if there is not space to draw count more prims
//note : here a quad is only 4, not 6, because there are 4 points in the buffer.  
void gfx_vertex_buffer_continue(gfx_vertex_bufferT* vb, zuint32 prim, zuint32 count) {

	if ((vb->count + count * 4) >= vb->capacity) {
		gfx_vertex_buffer_draw_clear(vb, prim);
	}

}

gfx_vertex_bufferT* vbt = NULL;
void gfx_arrow_start() {
	vbt = gfx_vertex_temp(&gxi_current_window->iface, "position:3|color:4"); //make or recycle a temp vertex buffer
}

void gfx_arrow(vec3* p1, vec3* p2, vec4* color) {

	vec3 zero = vec3const(0, 0, 0);
	vec4 one = vec4const(1, 1, 1, 1);

	if (!p1)
		p1 = &zero;

	if (!p2)
		p2 = &zero;

	if (!color)
		color = &one;

	vec3 a = *p1;
	vec3sub(a, *p2);


	float s = sqrtf(vec3abs_sq(a)) * .05;



	gfx_vertex_data4(vbt, 1, color->named.x, color->named.y, color->named.z, color->named.w);
	gfx_vertex_done3(vbt, 0, p1->VX - s, p1->VY, p1->VZ - s);

	gfx_vertex_data4(vbt, 1, color->named.x, color->named.y, color->named.z, color->named.w);
	gfx_vertex_done3(vbt, 0, p1->VX + s, p1->VY + s, p1->VZ + s);

	gfx_vertex_data4(vbt, 1, color->named.x, color->named.y, color->named.z, color->named.w);
	gfx_vertex_done3(vbt, 0, p2->VX, p2->VY, p2->VZ);


	

}
void gfx_arrow_end() {
	gfx_vertex_buffer_draw_clear(vbt, GFX_TRIANGLE);
}




zbool gfx_free_mesh(gfx_meshT* m) {

	ram_free(m->style);
	ram_free(m->vb);
	ram_free(m->vbaux);
	ram_free(m->next); //recursive:  maybe stack overflow if too many?

	
	return ZTRUE; //don't free, we already did

}