#define GFXINTERNAL
#include "ztypes.h"
#include "zmem.h"
#include "zwindow.h"
//#include "zlist.h"
#include "gfx_gl.h"
#include "zvector.h"
#include "zstring.h"
#include "string.h"
#include "zarray.h"

#include <stdio.h>
#include "glsl.h"




zbool gxi_delete_shadergroup(void* v) {

	gx_shadergroupT* sg = (gx_shadergroupT*)v;

	ram_free(sg->fsource);
	ram_free(sg->vsource);
	zlist_cleanup(&sg->variants);

	return ZTRUE;
}

//gx_shadergroupT* basic_shader = NULL;  //This is set to a very basic vertex-lighting shader similar to fixed function opengl
										//for quick and dirty protyping, debug drawing, etc

void gx_set_basic_shader(char* vsource, char* fsource) {
	if (gxi_current_window->basic_shader)
		ram_free(gxi_current_window->basic_shader);
	gxi_current_window->basic_shader = gx_shader_source(vsource, fsource);
}

gx_shadergroupT* gx_shader_source(char* vsource, char* fsource) {

	gx_shadergroupT* sg = ram_alloc(sizeof(gx_shadergroupT), gxi_delete_shadergroup);

	if (!sg)
		return NULL;

	if (vsource[0] == '@')
		sg->vsource = ram_loadstr(vsource + 1);
	else
		sg->vsource = ram_strdup(vsource);


	if (fsource[0] == '@')
		sg->fsource = ram_loadstr(fsource + 1);
	else
		sg->fsource = ram_strdup(fsource);


	return sg;

}

int get_shader_uniform_loc(gx_shader_variantT* shader, char* name) {
	checkGL();
	int loc = glGetUniformLocation(shader->program, name);
	gxdprintf(" UNFM %s -> %d\n", name, loc);
	checkGLNote("finding uniform uloc", ZTRUE);
	return loc;
}

int get_shader_attribute_loc(gx_shader_variantT* shader, char* name) {
	checkGL();
	int loc = glGetAttribLocation(shader->program, name);
	gxdprintf(" ATTR %s -> %d\n", name, loc);
	checkGLNote("finding uniform uloc", ZTRUE);
	return loc;
}

zbool gxi_is_property_for_shader(gfx_propertyT* p) {

	if (p->id == GXI_BLEND_MODE)
		return ZFALSE;

	if (p->id == GXI_LIGHT_AMBIENT)
		return ZFALSE;

	return ZTRUE;

}

zbool shader_active = ZFALSE;


zbool freevariant(void* v) {
	gx_shader_variantT* variant = v;
	ram_free(variant->key);
	ram_free(variant->uloc);
	ram_free(variant->aloc);

	if (variant->program)
		glDeleteProgram(variant->program);

	return ZTRUE;

}
gx_shadergroupT* current_shader_group = NULL;
gx_shader_variantT* current_variant = NULL;



gx_shader_variantT* gx_shader_variant(gx_shadergroupT* sg, char* key, gfx_styleT* st, gfx_vertex_bufferT* vb) {

	int i;
	int status = 0;
	char log[1024];
	int len = 0;


	gx_shader_variantT* shader = NULL;



	if (!sg)
		sg = gxi_current_window->basic_shader;

	if (!sg) {
		if (shader_active) {
			glUseProgram(0);
			shader_active = ZFALSE;
			
			current_variant = NULL;
			current_shader_group = NULL;
		}
		return NULL;  //no shader variant, means no shader
	}
	//skip the current variant is from the same group and has the same key, just return it. No need to search
	if (sg == current_shader_group) {
		if (!strcmp(current_variant->key, key)) {
			//gxdprintf("Still using same variant %s\n", key);
			return current_variant;
		}
	}

	//find variant

	for (shader = zlist_head(&(sg->variants)); shader; shader = zlist_next(shader)) {
		if (!strcmp(shader->key, key)) {
		//	gxdprintf(" Found existing shader variant %s\n", key);
			break;
		}

	}

	if (!shader) {

		gxdprintf(" make shader variant for format key '%s'\n", key);
		shader = ram_alloc(sizeof(gx_shader_variantT), freevariant); //TODO: destructor
	

		int hline = 1;
		char* header = zstrdup("#version 130\n");

		gfx_propertyT* p;

		for (i = 0; i < zvec_count(&st->properties); i++) {
			char b[50];

			p = zvec_get_at(&st->properties, i);

 			if (!gxi_is_property_for_shader(p))  //some properties are NOT actually for shader uniforms
				continue;


			snprintf(b, sizeof(b), "%s%d", p->name, p->index);
			header = zstrcat(header, "#define enable_");
			header = zstrcat(header, b);
			header = zstrcat(header, "\n");
			hline++;


		}

		gfx_vertex_attributeT* attr = NULL;

		for (i = 0; i < vb->num_attributes; i++) {
			header = zstrcat(header, "#define enable_");
			header = zstrcat(header, vb->attributes[i].name);
			header = zstrcat(header, "\n");
			hline++;
		}

		char* vsource[] = { header, sg->vsource };
		char* fsource[] = { header, sg->fsource };

		printf(" vertex:\n %s %s  fragment:\n  %s %s\n", vsource[0], vsource[1], fsource[0], fsource[1]);



		shader->v_shader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(shader->v_shader, 2, (const char**)vsource, NULL);

		glCompileShader(shader->v_shader);

		status = 0;
		glGetShaderiv(shader->v_shader, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(shader->v_shader, 1024, &len, log);
		printf("vertex error:\n%s\n +%dlines\n", log, hline);
		if (!status) {
			//todo:cleanup
			getc(stdin);
			return NULL;
		}

		//create f shader
		//replace the shader source with the fragment shader source

		shader->f_shader = glCreateShader(GL_FRAGMENT_SHADER);

		gxdprintf(" Created fshader %u\n", shader->f_shader);

		glShaderSource(shader->f_shader, 2, (const char**)fsource, NULL);

		glCompileShader(shader->f_shader);

		status = 0;
		glGetShaderiv(shader->f_shader, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(shader->f_shader, 1024, &len, log);

		printf("fragment error:\n%s\n+%dlines\n", log, hline);
		if (!status) {
			//todo:cleanup
			getc(stdin);
			return NULL;
		}
		//make program

		shader->program = glCreateProgram();


		glAttachShader(shader->program, shader->v_shader);
		glAttachShader(shader->program, shader->f_shader);
		glLinkProgram(shader->program);

		ram_free(header);

		status = 0;
		glGetProgramiv(shader->program, GL_LINK_STATUS, &status);
		
		glGetProgramInfoLog(shader->program, 1024, &len, log);
		gxdprintf(" Link error:%s\n", log);
		
		if (!status) {
			printf(" Link unsuccessful\n");
			//todo: cleanup
			return NULL;
		}
		printf(" SHADER COMPILE OUTPUT ^^\n");
		//getc(stdin);

		//find all the uniform and attribute locations

		shader->modelview_uloc = get_shader_uniform_loc(shader, "gfx_modelview");
		shader->projection_uloc = get_shader_uniform_loc(shader, "gfx_projection");
		shader->camera_pos_uloc = get_shader_uniform_loc(shader, "gfx_camera_pos");
		shader->ambient_uloc = get_shader_uniform_loc(shader, "gfx_ambient_sum");

		shader->uloc = zarray_alloc(zuint32, zvec_count(&st->properties));

		for (i = 0; i < zvec_count(&st->properties); i++) {
			char b[50];

			p = zvec_get_at(&st->properties, i);

			if (!gxi_is_property_for_shader(p))  //some properties are NOT actually for shader uniforms
				continue;

			snprintf(b, sizeof(b), "%s%d", p->name, p->index);
			shader->uloc[i] = get_shader_uniform_loc(shader, b);

		}


		shader->aloc = zarray_alloc(zuint32, vb->num_attributes);

		for (i = 0; i < vb->num_attributes; i++) {
			shader->aloc[i] = get_shader_attribute_loc(shader, vb->attributes[i].name);
		}

		//getc(stdin);
		shader->key = zstrdup(key);
		zlist_addhead(&sg->variants, &shader->zlistnode);

	}
	checkGL();

	glUseProgram(shader->program);
	checkGL();
	shader_active = ZTRUE;
	current_shader_group = sg;
	current_variant = shader;
	return shader;
}




int gfx_sizeof(int type) {

	switch ( (type&GXI_BASETYPEMASK) ) {
		case GFX_FLOAT: return  (int)sizeof(zfloat32) * 1;
		case GFX_FLOAT2: return (int)sizeof(zfloat32) * 2;
		case GFX_FLOAT3: return (int)sizeof(zfloat32) * 3;
		case GFX_FLOAT4: return (int)sizeof(zfloat32) * 4;
		case GFX_MAT33: return  (int)sizeof(zfloat32) * 9;
		case GFX_MAT44: return  (int)sizeof(zfloat32) * 16;
		case GFX_TEXTURE: return sizeof(int);  //texture integer
	}
	
	printf("unknown type size:%d\n", type);
	return 0;
}



void gfx_set_input(gfx_shader_inputT* si,  void* data) {
	float* f = data; //for debugger
	int tu = 0;

	if (si->uniform) {	//uniforms
		int count = si->count;

		if (si->type & GFX_ARRAY) {
			count = zarray_size(data);
		}

		switch ( (si->type)&(GXI_BASETYPEMASK)) {

		case GFX_FLOAT:
			glUniform1fv(si->loc, count, data);
			break;
		case GFX_FLOAT2:
			glUniform2fv(si->loc,count, data);
			break;
		case GFX_FLOAT3:
			glUniform3fv(si->loc, count, data);
			break;
		case GFX_FLOAT4:
			glUniform4fv(si->loc, count, data);
			break;
		case GFX_MAT33:
			glUniformMatrix3fv(si->loc, count,0, data);
		case GFX_MAT44:
			glUniformMatrix4fv(si->loc, count,0, data);
			break;
		case GFX_TEXTURE:
			
			 tu = gxi_add_texture(data, ZFALSE);
			glUniform1i(si->loc, tu);
			
			break;
		default:
			printf(" Unknown uniform type %d\n", si->type);
		return;
		}
	} else {	//attributes have count of 0
		checkGL();
		gpu_storageT* gs = gpu_storage(data);
		if (!gs) {
			printf("can't create gpu buffer (data array has no gpu_storage structure) \n");
			return;
		}

		int size = gfx_sizeof(si->type);
		int count = zarray_size(data);
		
		zbool toupdate = ZFALSE;

		if (!gs->buffer) {
			glGenBuffers(1, &(gs->buffer));
			checkGL();
			toupdate = ZTRUE;
		}

		glBindBuffer(GL_ARRAY_BUFFER, gs->buffer);
		checkGL();
		if (toupdate) {	//TODO: maintain a 'dirty' flag in the buffer so we can re-do this if needed
			
			glBufferData(GL_ARRAY_BUFFER, size * count, data, GL_DYNAMIC_DRAW);
			checkGL();
		}
		checkGL();

		if (last_aloc_use[si->loc] != aloc_use_counter) {
			glEnableVertexAttribArray(si->loc);  //enable it
			last_aloc_use[si->loc] = aloc_use_counter;
		}

		checkGL();
		switch (si->type) {

			case (GFX_FLOAT2 | GFX_ARRAY):

			glVertexAttribPointer(si->loc,
				2,  /*number of components */
				GL_FLOAT, /*GL data type*/
				0, 0, /* normalized, stride. stride 0 means densely packed */
				0);//offset is zero

			break;

			case (GFX_FLOAT4 | GFX_ARRAY):

			glVertexAttribPointer(si->loc,
				4,  /*number of components */
				GL_FLOAT, /*GL data type*/
				0, 0, /* normalized, stride. stride 0 means densely packed */
				0);//offset is zero


			break;

			case (GFX_FLOAT3|GFX_ARRAY):

				glVertexAttribPointer(si->loc,
					3,  /*number of components */
					GL_FLOAT, /*GL data type*/
					0, 0, /* normalized, stride. stride 0 means densely packed */
					0);//offset is zero
								

				break;
			default:
				printf(" Unknown attribute type %d\n", count);
				return;
		}
		checkGL();
	}


}


void gx_use_shader(gfx_shaderT* shader) {
	glUseProgram(shader->program);
	gxi_new_texture_set();
}

gfx_shaderT* gx_compile_shader(char* vsource, char* fsource, zvecT * inputs, int flags){

	int i;
	int status = 0;
	char log[1024];
	int len = 0;

	gx_shader_variantT* shader = ram_alloc(sizeof(gx_shader_variantT), freevariant);

	int hline = 1;
	char* header = zstrdup("#version 330\n");

	char* vs[] = { header, vsource };
	char* fs[] = { header, fsource };

	printf(" vertex:\n %s %s  fragment:\n  %s %s\n", vs[0], vs[1], fs[0], fs[1]);

	//add parameters that mask off some areas
				
	shader->v_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(shader->v_shader, 2, (const char**)vs, NULL);

	glCompileShader(shader->v_shader);

	status = 0;
	glGetShaderiv(shader->v_shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shader->v_shader, 1024, &len, log);
	printf("vertex error:\n%s\n +%dlines\n", log, hline);
	if (!status) {
		//todo:cleanup
		getc(stdin);
		return NULL;
	}

	//create f shader
	//replace the shader source with the fragment shader source

	shader->f_shader = glCreateShader(GL_FRAGMENT_SHADER);

	gxdprintf(" Created fshader %u\n", shader->f_shader);

	glShaderSource(shader->f_shader, 2, (const char**)fs, NULL);

	glCompileShader(shader->f_shader);

	status = 0;
	glGetShaderiv(shader->f_shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shader->f_shader, 1024, &len, log);

	printf("fragment error:\n%s\n+%dlines\n", log, hline);
	if (!status) {
		//todo:cleanup
		getc(stdin);
		return NULL;
	}
	//make program
	
	shader->program = glCreateProgram();


	glAttachShader(shader->program, shader->v_shader);
	glAttachShader(shader->program, shader->f_shader);
	glLinkProgram(shader->program);

	ram_free(header);

	status = 0;
	glGetProgramiv(shader->program, GL_LINK_STATUS, &status);

	glGetProgramInfoLog(shader->program, 1024, &len, log);
	gxdprintf(" Link error:%s\n", log);

	if (!status) {
		printf(" Link unsuccessful\n");
		//todo: cleanup
		return NULL;
	}
	printf(" SHADER COMPILE OUTPUT ^^\n");

	checkGL();

	//set all the uniform/attribute positions

	for (i = 0; i < zvec_count(inputs); i++) {
		gfx_shader_inputT* si = zvec_get_at(inputs, i);
		if (si->uniform)
			si->loc = glGetUniformLocation(shader->program, si->name);
		else
			si->loc = glGetAttribLocation(shader->program, si->name);

		printf(" %d  %d %s\n", si->uniform, si->loc, si->name);
	}


	return shader;
}












char* cshCode =
"#version 430  \n"
"layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;	"
" layout (std430, binding=3) buffer data{ vec4 d; };"
"void main() {"
	"d*=vec4(11.0,11.1,111.0,1111.0);"
	
"}"
"";




void testComputeShader() {
	char log[1024];
	log[0] = 0;
	int status = 0;
	int len = 0;

	int ssbo = 0;
	checkGL();

	float data[4];
	data[0] = 10;
	data[1] = 9;
	data[2] = 8;
	data[3] = 7;

	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,  sizeof(data), data, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	checkGL();


	checkGL();
	int csh = glCreateShader(GL_COMPUTE_SHADER);
	checkGL();
	glShaderSource(csh, 1, &cshCode, NULL);
	checkGL();
	glCompileShader(csh);

	glGetShaderiv(csh, GL_COMPILE_STATUS, &status);
	checkGL();

	glGetShaderInfoLog(csh, 1024, NULL, log);
	checkGL();

	printf("res:%d %s\n",status, log);
	if (!status)
		exit(1);

	int prog = glCreateProgram();
	checkGL();

	glAttachShader(prog, csh);
	checkGL();

	glLinkProgram(prog);
	checkGL();

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	checkGL();

	glGetShaderInfoLog(csh, 1024, NULL, log);
	printf("res:%d %s\n", status, log);
	if (!status)
		exit(1);

	checkGL();
	glUseProgram(prog);
	checkGL();

	int idx = glGetProgramResourceIndex(prog, GL_SHADER_STORAGE_BLOCK, "data");
	int idx2 = glGetProgramResourceIndex(prog, GL_SHADER_STORAGE_BLOCK, "data2");
	printf(" %d %d\n", idx, idx2);
	checkGL();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	checkGL();
	//run it
	glDispatchCompute(1, 1, 1); 

	checkGL();
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	checkGL();

	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(data), data);
	
	printf(" %f %f %f %f\n", data[0], data[1], data[2], data[3]);

	exit(1);

}