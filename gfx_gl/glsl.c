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

gx_shader_variantT* gx_shader_variant(gx_shadergroupT* sg, char* key, gfx_styleT* st, gfx_vertex_bufferT* vb) {

	int i;
	int status = 0;
	char log[1024];
	int len = 0;

	printf(" make shader variant for format key '%s'\n", key);
	gx_shader_variantT* shader = NULL;

	if (!sg) {
		if (shader_active) {
			glUseProgram(0);
			shader_active = ZFALSE;
		}
		return NULL;  //no shader variant, means no shader
	}

	//find variant

	for (shader = zlist_head(&(sg->variants)); shader; shader = zlist_next(shader)) {
		if (!strcmp(shader->key, key)) {
			printf(" Found existing shader variant %s\n", key);
			break;
		}

	}

	if (!shader) {

		shader = ram_alloc(sizeof(gx_shader_variantT), NULL); //TODO: destructor
	


		char* header = zstrdup("#version 120\n");

		gfx_propertyT* p;

		for (i = 0; i < zvec_count(&st->properties); i++) {
			char b[20];

			p = zvec_get_at(&st->properties, i);

			if (!gxi_is_property_for_shader(p))  //some properties are NOT actually for shader uniforms
				continue;


			snprintf(b, sizeof(b), "%s%d", p->name, p->index);
			header = zstrcat(header, "#define enable_");
			header = zstrcat(header, b);
			header = zstrcat(header, "\n");


		}

		gfx_vertex_attributeT* attr = NULL;

		for (i = 0; i < vb->num_attributes; i++) {
			header = zstrcat(header, "#define enable_");
			header = zstrcat(header, vb->attributes[i].name);
			header = zstrcat(header, "\n");
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
		gxdprintf("vertex error:\n%s\n", log);
		if (!status) {
			//todo:cleanup
			getc(stdin);
			return NULL;
		}

		//create f shader
		//replace the shader source with the fragment shader source

		shader->f_shader = glCreateShader(GL_FRAGMENT_SHADER);

		//printf(" Created fshader %u\n", shader->f_shader);

		glShaderSource(shader->f_shader, 2, (const char**)fsource, NULL);

		glCompileShader(shader->f_shader);

		status = 0;
		glGetShaderiv(shader->f_shader, GL_COMPILE_STATUS, &status);
		glGetShaderInfoLog(shader->f_shader, 1024, &len, log);

		gxdprintf("fragment error:\n%s\n", log);
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


		status = 0;
		glGetProgramiv(shader->program, GL_LINK_STATUS, &status);
		if (!status) {
			glGetProgramInfoLog(shader->program, 1024, &len, log);
			printf(" Link error:%s\n", log);
			getc(stdin);
			//todo: cleanup
			return NULL;
		}


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
	return shader;
}

