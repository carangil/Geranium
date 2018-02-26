// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/zmem.h"
#include "../vmath/zmath.h"
#include <stdio.h>
#include "gx_image.h"
#include "gx_buffers.h"

#include "../structures/zvector.h"
#include "../structures/zlist.h"
#include "../structures/zstring.h"

#include "gx_sys.h"
#include "gx_drawstyle.h"
#include "gx_light.h"

#include "defshader.v.h"
#include "defshader.f.h"

#include "glheaders.h"




static gx_shader_t* active_shader = NULL;


static gx_shadergroup_t* gxi_default_shader_group();


//activate a drawstyle
static gx_drawstyle_t* active_drawstyle = NULL;
static zbool active_drawstyle_dirty = ZTRUE;

void gx_drawstyle_activate(gx_drawstyle_t* style)
{
	if (active_drawstyle == style)
		return;  //do nothing if already set
		
	active_drawstyle = style;
	active_drawstyle_dirty = ZTRUE; 
}

char* gxi_drawstyle_spec(gx_drawstyle_t* style){
	char* spec ;

	if (style->spec)
		return style->spec;
	
	int i;
	
	spec = zstr_mk(100,0);
	
	for (i=0; i< zvec_count( &style->textures); i++) {
		char b[50];
	
		snprintf(b, sizeof(b), "GX_TEXTURE%d|", i);
							
		spec = zstr_cat(spec, b);
	}
	
	style->spec = spec;
	
	return spec;
	
}


gx_shader_t* gxi_select_shader(gx_vbuffer_t* vb)
{
	gx_shader_t* set = NULL;
	
	gx_drawstyle_t* style = active_drawstyle;
		
	zuint32 i=0;
	
	if (!style)
	{
#if 1
		//default style: 
		
		//no textures
		gx_set_active_textures(NULL, 0);
		glColor4f(1,1,1,1);
		glDisable(GL_BLEND);
#endif	
		//TODO: when using shaders, need to use a default drawstyle if none specified
		return NULL;
	}


	
	//set blending mode
	if (style->blending)
	{
		glEnable(GL_BLEND);
		
		if (gx_blend_alpha ==  style->blending)
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		}
		else if (gx_blend_add ==  style->blending)
		{
			glBlendFunc(GL_ONE, GL_ONE);	
		}
		else if(gx_blend_multiply == style->blending)
		{
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
		}
		
	}
	else
		glDisable(GL_BLEND);
	
	//activate all set textures
	gx_set_active_textures(zvec_elements_as(gx_image_t*, &style->textures), zvec_count(&style->textures));

		if (! gxi_fixed_function) {
		char* fullspec = zstrndup(gxi_env_spec(), 100);
		fullspec = zstr_cat(fullspec, gxi_drawstyle_spec(style));
		fullspec = zstr_cat(fullspec, gxi_vbuffer_spec(vb));
		printf(" SHADER SPEC WILL BE %s\n", fullspec);

		if (style->shadergroup) 
			set = gx_shader_variant(style->shadergroup, fullspec);
		else 
			set = gx_shader_variant(gxi_default_shader_group(), fullspec);

		ram_free(fullspec);  //TODO: should cache the fullspec
	}
	
		
	//if we are using a shader, activate it
	
	if (set) {  
		int txcount = zvec_count(&style->textures);
		char texname[20];
		int loc;
		//gx_shader_t* set = style->shader;
		active_shader = set;
		
		glUseProgram(set->program);
		
		if (txcount > set->enabled_texture_uniforms) {
			//try to enable all the textures we need

			for ( i = set->enabled_texture_uniforms ; i < txcount ; i++) {
					sprintf(texname, "gx_texture%d", i);
					loc = glGetUniformLocation(set->program , texname);
					if (loc!=-1) {
							printf(" %s is at %d, set to unit %d\n", texname, loc, i);
							glUniform1i(loc, i); //set texture uniform loc to use texture unit i
					}
				
			}
			
			set->enabled_texture_uniforms = txcount;
		}
		
		//send other material properties
		if (set->specular_exponent_uloc!=-1)
			glUniform1f(set->specular_exponent_uloc, style->specular_exponent);
		
		if (set->specular_color_uloc!=-1)
			glUniform3fv(set->specular_color_uloc, 1, style->specular_color.array);
				
		
		
	} else {
		float white[]={1,1,1,1};
		float black[]={0,0,0,1};
		float red[]={1,0,0,1};

		float f[4];
		//no shader
		active_shader = NULL;
		glUseProgram(0);
	
	
		//set FF specular color, and material parameters
		f[0]=style->specular_color.array[0];
		f[1]=style->specular_color.array[1];
		f[2]=style->specular_color.array[2];
		f[3]=1;

	//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, style->specular_exponent);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white); //setting diffuse and ambient to white will just pass-through the texture color
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
		
	}
	
	//set all other shader env parameters
	// env parameters are fog, light, etc
	
	gxi_set_shader_env_params(active_shader);

	return active_shader;
}



zbool drawstyle_free(void* x)
{
	gx_drawstyle_t* ds = x;

	zvec_cleanup(&ds->textures);

	return ZTRUE;
}

gx_drawstyle_t* gx_drawstyle_mk(gx_image_t* img)
{
	gx_drawstyle_t* ds;

	ds = ram_alloc(sizeof(*ds), drawstyle_free);

	if (ds)
	{
		zvec_mk( & ds->textures,2);
		
		if(img)
		{
			ram_addref(img);
			zvec_add_or_free(&ds->textures, img);
		}
	}

	return ds;
}

#if 1

int get_shader_uniform_loc(gx_shader_t* shader, char* name) {
		int loc = glGetUniformLocation(shader->program, name);
		printf(" UNFM %s -> %d\n", name, loc);
		return loc;
}

int get_shader_attribute_loc(gx_shader_t* shader, char* name) {
		int loc = glGetAttribLocation(shader->program, name);
		printf(" ATTR %s -> %d\n", name,loc);
		return loc;
}

zbool gxi_delete_shadergroup(void* v){
	
	gx_shadergroup_t* sg = (gx_shadergroup_t*) v;
	
	ram_free(sg->fsource);
	ram_free(sg->vsource);
	zlist_cleanup(&sg->variants);
		
	return ZTRUE;
}



gx_shadergroup_t* gx_shader_source(char* vsource, char* fsource) {

	
	gx_shadergroup_t* sg = ram_alloc(sizeof(gx_shadergroup_t), gxi_delete_shadergroup); 
	
	if (!sg) 
		return NULL;
	
	if (vsource[0] == '@')
		sg->vsource = ram_loadstr(vsource+1);
	else
		sg->vsource = ram_strdup(vsource);
	
	
	if (fsource[0] == '@')
		sg->fsource = ram_loadstr(fsource+1);
	else
		sg->fsource = ram_strdup(fsource);
	
	
	return sg;
	
}

/*
char* gx_generate_drawstyle_spec(gx_drawstyle_t*  style){
	
	char* spec = "";
	int i;
	
	for (i=0;i<zvec_count(&style->textures); i++) {
		if (i!=0)
			spec = zstrcat(spec, "GX_TEXTURE0
	}
	
	
}
*/


zbool gxi_delete_shader_variant(void* v){
	gxi_shader_variant_t * var = (gxi_shader_variant_t *) v;
	
	ram_free(var->spec);
	ram_free(var->shader);  //decremenet refcount on the shader
	
	return ZTRUE;
}

static gx_shadergroup_t* builtin_shader_group = NULL;
static gx_shadergroup_t* gxi_default_shader_group(){
	
	if (builtin_shader_group)
		return builtin_shader_group;
	
	return builtin_shader_group = gx_shader_source( gxi_def_shader_v, gxi_def_shader_f);
	
}

gx_shader_t* gx_shader_variant(gx_shadergroup_t* sg, char* spec  ) {
	
	gxi_shader_variant_t* var = NULL;
	gx_shader_t* shader = NULL;
	
	zvec_t strings;
	int i;
	int j;
	char name[30] ;
	char log[1024] = "uninit";
	int len;
	int status;
	char* news;
	
	if (gxi_fixed_function)
		return NULL;  /* No shaders */
	
	if (!sg)
		sg = gxi_default_shader_group();

	//check if variant already exists
	var = (gxi_shader_variant_t*) zlist_head(&sg->variants);

	for ( ; var  ; var = zlist_next(var) ){
		
		if ( !strcmp(var->spec , spec)) {
			printf("FOUND VARIANT %s\n", var->spec);
			return var->shader;
			//return ram_addref(var->shader); //return the same shader again
		}
		
	}

	
	if (!sg->fsource)
		return NULL;

	if (!sg->vsource) 
		return NULL;
		
	zvec_mk(&strings, 4) ;
	
	zvec_add_or_free(&strings,ram_strdup("#version 120\n"));  //todo: multiple versions of glsl
	
	if(spec && strlen(spec)) {
		zsplit (&strings, spec, '|');
		
		for(i=1;i<zvec_count(&strings);i++) {
			
			if (!strlen(zvec_get_at(&strings, i)))
				continue;   //skip empty strings
			
			news = zstrndup( "#define ", ZSTRING_ALL);
			news = zstr_cat(news, zvec_get_at(&strings, i));
			news = zstr_cat(news, "\n" );
			
			ram_free(zvec_get_at(&strings, i));
			
			zvec_set_at(&strings, i, news);
			
		}
	}
	
	zvec_add(&strings, sg->vsource);
		
	for(i=0;i<zvec_count(&strings);i++) {
		printf("%s\n", zvec_get_x_at(&strings, char *, i));
	}
	
	
	shader = ram_alloc(sizeof(gx_shader_t), NULL);

	if (!shader)
		return NULL;

	//create v shader

	shader->v_shader = glCreateShader(GL_VERTEX_SHADER);

	//printf(" Created vshader %u\n", shader->v_shader);
	
	glShaderSource(shader->v_shader, zvec_count(&strings), zvec_elements(&strings), NULL );

	glCompileShader(shader->v_shader);
	
	status = 0;
	glGetShaderiv(shader->v_shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shader->v_shader, 1024, &len, log);
	printf("vertex:\n%s\n", log);
	if (!status) {
		//todo:cleanup
		return NULL;
	}
	
	//create f shader
	//replace the shader source with the fragment shader source
	zvec_set_at( &strings, zvec_count(&strings) -1, sg->fsource);
	
	shader->f_shader = glCreateShader(GL_FRAGMENT_SHADER);

	//printf(" Created fshader %u\n", shader->f_shader);
	
	glShaderSource(shader->f_shader, zvec_count(&strings), zvec_elements(&strings), NULL );

	glCompileShader(shader->f_shader);
	
	status = 0;
	glGetShaderiv(shader->f_shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shader->f_shader, 1024, &len, log);
	
	printf("fragment:\n%s\n", log);
	if (!status) {
		//todo:cleanup
		return NULL;
	}
	//make program

	shader->program = glCreateProgram();


	glAttachShader(shader->program, shader->v_shader);
	glAttachShader(shader->program, shader->f_shader);
	glLinkProgram(shader->program);

	
	status =0;
	glGetProgramiv(shader->program, GL_LINK_STATUS, &status);
	if (!status) {
			glGetProgramInfoLog(shader->program, 1024, &len, log);
			printf(" Link error:%s\n",log);
			//todo: cleanup
			return NULL;
	}

	
	//see if we have a uniform and attribute locations
	
	shader->vertex_loc = get_shader_attribute_loc(shader, "gx_vertex");
	shader->color_loc = get_shader_attribute_loc(shader, "gx_color");
	shader->normal_loc = get_shader_attribute_loc(shader, "gx_normal");
	
	{
		
		
		
		for (j=0;j<GX_MAX_TEXCOORD;j++) {
			sprintf(name, "gx_texcoord%d", j);
			
			shader->texcoord_loc[j] = get_shader_attribute_loc(shader, name);
			
			
		

		}		
	}
	
	//transform
	shader->modelview_uloc = get_shader_uniform_loc(shader, "gx_modelview");
	shader->projection_uloc = get_shader_uniform_loc(shader, "gx_projection");
	shader->camera_pos_uloc = get_shader_uniform_loc(shader, "gx_camera_pos");
	
	//material properties
	shader->specular_color_uloc = get_shader_uniform_loc(shader, "gx_specular_color");
	shader->specular_exponent_uloc = get_shader_uniform_loc(shader, "gx_specular_exponent");
	
	//light properties
	shader->ambient_light_uloc =  get_shader_uniform_loc(shader, "gx_ambient_light");
	
	for (j=0;j<GX_MAXLIGHTS;j++) {
	
		sprintf(name, "gx_light%d_color", j);
		shader->light_color_uloc[j] = get_shader_uniform_loc(shader, name);
		
		sprintf(name, "gx_light%d_pos_camspace", j);
		shader->light_pos_camspace_uloc[j] =  get_shader_uniform_loc(shader, name);
		
		sprintf(name, "gx_light%d_atten_const", j);
		shader->light_atten_const_uloc[j] = get_shader_uniform_loc(shader, name);
		
		sprintf(name, "gx_light%d_atten_linear", j);
		shader->light_atten_linear_uloc[j] = get_shader_uniform_loc(shader, name);
		
		sprintf(name, "gx_light%d_atten_squared", j);
		shader->light_atten_squared_uloc[j] = get_shader_uniform_loc(shader, name);
	
	}
	
		
	
	//delete the vec holding all the strings
	
	//remove the fragment shader source from the end of the list
	zvec_set_at( &strings, zvec_count(&strings) -1, NULL);
	//everything else in that vector can now be deleted
	zvec_cleanup(&strings); 
	
		
//todo: create the variant!

	var = ram_alloc(sizeof(*var), gxi_delete_shader_variant);
	if (var) {
		printf("ADD VARIANT TO LIST\n");
		var->spec = ram_strdup(spec);
		var->shader = shader;
		zlist_addhead(&sg->variants, &var->zlistnode);
	}
	
	return ram_addref(shader);  //return reference to shader (1st reference is in the linked list)
}
#endif
