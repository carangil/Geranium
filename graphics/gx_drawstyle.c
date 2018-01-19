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
#include "gx_drawstyle.h"
#include "glheaders.h"
#include "gx_light.h"

static gx_shaderset_t* active_shader = NULL;

 gx_shaderset_t* gxi_active_shaderset(){
		return active_shader;
}


//activate a drawstyle
//NOTE: there is not yet a constructor for styles, only functions that use them.  The user must create them
//		I may add a constructor later, but is not necessary at this point.
//
//  When shaders are implemented, they will appear here


void gx_drawstyle_activate(gx_drawstyle_t* style)
{
	
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

		return;
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


	//if we are using shaders, activate them
	
	if (style->shaderset) {
		int txcount = zvec_count(&style->textures);
		char texname[20];
		int loc;
		gx_shaderset_t* set = style->shaderset;
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

int get_shader_uniform_loc(gx_shaderset_t* shaderset, char* name) {
		int loc = glGetUniformLocation(shaderset->program, name);
		printf(" UNFM %s -> %d\n", name, loc);
		return loc;
}

int get_shader_attribute_loc(gx_shaderset_t* shaderset, char* name) {
		int loc = glGetAttribLocation(shaderset->program, name);
		printf(" ATTR %s -> %d\n", name,loc);
		return loc;
}




gx_shaderset_t* gx_shaderset_mk(char* vsource, char* psource) {

	gx_shaderset_t* shaderset;
	char* tmp = NULL;
	char* tmp2 = NULL;
	char log[1024] = "uninit";
	int len;
	int status;

	if (!vsource)
		return NULL;

	if (vsource[0] == '@') {
		tmp = ram_loadstr(vsource+1);
		vsource = tmp;
	}

	if (psource[0] == '@') {
		tmp2 = ram_loadstr(psource+1);
		psource = tmp2;
	}

	shaderset = ram_alloc(sizeof(gx_shaderset_t), NULL);

	if (!shaderset)
		return NULL;

	//create v shader

	shaderset->v_shader = glCreateShader(GL_VERTEX_SHADER);

	//printf(" Created vshader %u\n", shaderset->v_shader);
	
	glShaderSource(shaderset->v_shader, 1, &vsource, NULL );

	glCompileShader(shaderset->v_shader);
	
	status = 0;
	glGetShaderiv(shaderset->v_shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shaderset->v_shader, 1024, &len, log);
	printf("%s:\n%s\n", vsource, log);
	if (!status) {
		//todo:cleanup
		return NULL;
	}
	

	//create f shader
	shaderset->f_shader = glCreateShader(GL_FRAGMENT_SHADER);

	//printf(" Created fshader %u\n", shaderset->f_shader);
	
	glShaderSource(shaderset->f_shader, 1, &psource, NULL );

	glCompileShader(shaderset->f_shader);
	
	status = 0;
	glGetShaderiv(shaderset->f_shader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(shaderset->f_shader, 1024, &len, log);
	printf("%s:\n%s\n", psource, log);
	if (!status) {
		//todo:cleanup
		return NULL;
	}
	//make program

	shaderset->program = glCreateProgram();


		glAttachShader(shaderset->program, shaderset->v_shader);
		glAttachShader(shaderset->program, shaderset->f_shader);
		glLinkProgram(shaderset->program);

		
		status =0;
		glGetProgramiv(shaderset->program, GL_LINK_STATUS, &status);
		if (!status) {
				glGetProgramInfoLog(shaderset->program, 1024, &len, log);
				printf(" Link error:%s\n",log);
				//todo: cleanup
				return NULL;
		}

		
		//see if we have a uniform and attribute locations
		
		shaderset->vertex_loc = get_shader_attribute_loc(shaderset, "gx_vertex");
		shaderset->color_loc = get_shader_attribute_loc(shaderset, "gx_color");
		shaderset->normal_loc = get_shader_attribute_loc(shaderset, "gx_normal");
		
		{
			int j;
			char name[20] = "gx_texcoord";
			
			for (j=0;j<GX_MAX_TEXCOORD;j++) {
				if (j>0)
					sprintf(name, "gx_texcoord%d", j);
				
				shaderset->texcoord_loc[j] = get_shader_attribute_loc(shaderset, name);
				
				
			

			}		
		}
		
		//transform
		shaderset->modelview_uloc = get_shader_uniform_loc(shaderset, "gx_modelview");
		shaderset->camera_pos_uloc = get_shader_uniform_loc(shaderset, "gx_camera_pos");
		
		//material properties
		shaderset->specular_color_uloc = get_shader_uniform_loc(shaderset, "gx_specular_color");
		shaderset->specular_exponent_uloc = get_shader_uniform_loc(shaderset, "gx_specular_exponent");
		
		//light properties
		shaderset->ambient_light_uloc =  get_shader_uniform_loc(shaderset, "gx_ambient_light");
		shaderset->light0_color_uloc = get_shader_uniform_loc(shaderset, "gx_light0_color");
		shaderset->light0_pos_camspace_uloc =  get_shader_uniform_loc(shaderset, "gx_light0_pos_camspace");
		
		
		
		

	if (tmp)
		ram_free(tmp);

	if (tmp2)
		ram_free(tmp2);

	return shaderset;
}
#endif
