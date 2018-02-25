// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/zmem.h"
#include <stdio.h>
#include "glheaders.h"
#include "../vmath/zmath.h"
#include "../structures/zvector.h"
#include "../structures/zlist.h"
#include "gx_image.h"
#include "gx_buffers.h"
#include "gx_drawstyle.h"
#include "gx_light.h"
#include "gx_trans.h"
#include "../structures/zstring.h"

gx_light_t* gx_light_mk(gx_light_e type, vec3* position, vec3* color, vec3* ambient)
{
	gx_light_t* li = NULL;

	if (!position)
		return NULL;

	li = ram_alloc(sizeof(*li), NULL);

	li->light_type = type;

	vec3mov(li->position, *position);
	
	if (color)
	{
		li->color = *color;
	}
	else
	{
		vec3set(li->color, 1,1,1);
	}
	
	if (ambient)
	{
		li->ambient = *ambient;
	}
	else 
	{
		vec3set(li->ambient, 0,0,0);
	}
		

	return li;
}

void gx_light_set_attenuation( gx_light_t* li, zbool attenuated, float maximum, float unityrange, float falloff){
		float l,c,s;
	
		if (!li)
			return;
	
		li->attenuated = attenuated;
		if (attenuated) {
			
			/*
			 *  If d = distance, lighting equation is:
			 * 
			 *  1 / ( C + l*d + s*d*d);
			 * 
			 */
			
			
			li->constant = c = 1/maximum;
			li->squared = s =  falloff * ((1-c) / (unityrange * unityrange));
			li->linear = l = (1-c - (1-c)*falloff) / unityrange;
			
			printf("Light parameters: C=%f L=%f S=%f\n",c,l,s  );
	
			
		} else {
			li->constant=1;
			li->linear = li->squared = 0;
		}
	
}


//number of light currently enabled
static zuint32 _number_active_lights = 0;

static int gx_light_tmp_off_cnt=0;

void gx_light_tmp_off()
{
	if (!gx_light_tmp_off_cnt)
		glDisable(GL_LIGHTING);

	gx_light_tmp_off_cnt++;

}

void gx_light_restore()
{
	gx_light_tmp_off_cnt--;


	if ( (gx_light_tmp_off_cnt==0) && _number_active_lights)
		glEnable(GL_LIGHTING);
}

void gx_light_evaluate(gx_light_t* li) {
	//the position for the light is set relative to the current transformation
	
	if (!li)
		return;
	
	vec3mov(li->camspace_position, li->position);
				
	if (li->light_type == gx_light_directional) {
		gx_trans_dir_vec3(&li->camspace_position);
	} else
	{
		gx_trans_vec3(&li->camspace_position);
	}
	
}


void gx_env_evaluate_lights(gx_environment_t* env){
	int i;
	gx_light_t* li;

	for(i=0;i<zvec_count(&env->lights);i++) {
		li = zvec_get_at(&env->lights, i);
		gx_light_evaluate(li);
	}

}


//simple lighting policy:
//if _number_active_lights is zero, gl lighting is disabled


//sets active lights, but using opengl fixed function
static void ff_set_active_lights(gx_light_t** lights, zuint32 count)
{
	vec4 zero;
	vec4 ambientsum;
	zuint32 i = 0;
	
	
	vec4set(zero,0,0,0,1);
	vec4set(ambientsum,0,0,0,1);

	gxi_refresh_matrix(NULL);  //fixed function needs modelview matrix loaded from our transform
	

	
	//printf(" GO\n");
	// user specified zero lights no lights are null
	if (count == 0 || !lights)
	{
		for (i=0;i<_number_active_lights;i++)
		{
			//disable all active lights
			glDisable(GL_LIGHT0 + i);  
		}

		//if lighting was on, turn it off
		if (_number_active_lights)
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_NORMALIZE);
			glDisable(GL_COLOR_MATERIAL);
		}

		//note for above:  instead of just disabling lighting, it was necessary to disable all the enabled lights,
		//because if later we turn lights back on, but fewer than currently, some higher number lights will be stuck on

		_number_active_lights = 0;

		return;
	}

	if (! _number_active_lights)
	{
		//if lighting was off, turn it on
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		//glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
		glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER,  0);
	}

	printf(" setting %d ff lights\n", count);
	glPushMatrix();
	glLoadIdentity();
	/* When in fixed function mode, opengl transforms the light position
	 * passed in using the modelview matrix.  I already did this when
	 * I calculated position_camspace value, so we load identity here
	 * so opengl doesn't transformt the light again.
	 * In traditional FF opengl, we wouldn't calculate the camspace position,
	 * but instead just pass in the light position, with the matrix already
	 * set.  But when we use shaders we use our own matrix stack and can
	 * do our own matrix transforms, so when we do fixed function, we still
	 * use our own matrix stack
	 * */


	for (i=0;i<count;i++)
	{
		float p[4];

		//TODO: OpenGL wants 4 component vectors, but I had previously chosen to use 3 components
		//		perhaps this was a bad choice

		//add up ambient light
		vec3add(ambientsum,lights[i]->ambient); 
		
			
		if (i<GX_MAXLIGHTS) {
		
			if (i>= _number_active_lights)
				glEnable(GL_LIGHT0 + i); //enable light if it hasn't been enabled before

			if (!lights[i]) 
				continue;

			//set the light object's ambient to zero, so we don't
			// attenuate by distance	
			//instead we will set the global ambient
			//printf(" set ambient ZERO\n");
			glLightfv(GL_LIGHT0+i, GL_AMBIENT,  zero.array );
			

			//set diffuse and specular color
			p[0]=lights[i]->color.array[0];
			p[1]=lights[i]->color.array[1];
			p[2]=lights[i]->color.array[2];

			glLightfv(GL_LIGHT0+i, GL_SPECULAR, p);
			glLightfv(GL_LIGHT0+i, GL_DIFFUSE,  p);
			
			
			//set light position
			p[0]=lights[i]->camspace_position.named.x;
			p[1]=lights[i]->camspace_position.named.y;
			p[2]=lights[i]->camspace_position.named.z;

			if (lights[i]->light_type == gx_light_directional)
			{
				p[3]=0;
			}
			else
			{
				p[3]=1;
			}
	
				
			glLightfv(GL_LIGHT0+i, GL_POSITION, p);
			
	#if 0
			if (i==0){
			
				//This abuses the opengl transform function to check
				//my version of the gx_trans_vec3 function.
				
				float v[4];
			
				printf("Originalp: %f %f %f\n", p[0], p[1], p[2]);
				glLightfv(GL_LIGHT0+i, GL_POSITION, p);
				glGetLightfv(GL_LIGHT0+i, GL_POSITION,v);
				printf("GL Light0: %f %f %f %f\n", v[0] ,v[1], v[2]);
				gx_trans_vec3(&p);
				printf("Trans   p: %f %f %f\n", p[0], p[1], p[2]);
				
				
			}
	#endif 

			if (lights[i]->attenuated) {
			
				glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, lights[i]->constant );
				glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION,  lights[i]->squared  );
				glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, lights[i]->linear  ); 
			} else {
				glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, 1.0);
				glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, 0.0);
				glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, 0.0);
			}
		}
				
	}
	glPopMatrix();

	for (i= count; i < _number_active_lights;i++)
	{
		glDisable(GL_LIGHT0 + i);  //disable any lights we no longer want
	}

	_number_active_lights= count;
	

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientsum.array);

}


static void shader_set_active_lights( gx_shader_t* set, gx_light_t** lights, int lightcount){
	vec3 ambientsum;
	vec3 p;
	float l, c, s;
	int i;

	
	
	vec3set (ambientsum, 0,0,0);

	for (i = 0;i < lightcount ;i++) {
		vec3add(ambientsum, lights[i]->ambient);
	}

	if (lightcount > GX_MAXLIGHTS)
		lightcount = GX_MAXLIGHTS;
	
	for (i=0;i<lightcount;i++) {
	
		
		if (set->light_color_uloc[i] != -1) {
			glUniform3fv(set->light_color_uloc[i], 1, lights[i]->color.array);
		}
		if (set->light_pos_camspace_uloc[i] != -1) {
			
			

			glUniform3fv(set->light_pos_camspace_uloc[i], 1, lights[i]->camspace_position.array);
								
		}
			
		if (lights[i]->attenuated) {
		
			if (set->light_atten_const_uloc[i] != -1)
				glUniform1f(set->light_atten_const_uloc[i],  lights[i]->constant);
				
			if (set->light_atten_linear_uloc[i] != -1)
				glUniform1f(set->light_atten_linear_uloc[i],  lights[i]->linear);
				
			if (set->light_atten_squared_uloc[i] != -1)
				glUniform1f(set->light_atten_squared_uloc[i],  lights[i]->squared);
				
		} else {
			if (set->light_atten_const_uloc[i] != -1)
				glUniform1f(set->light_atten_const_uloc[i],  1.0 );
				
			if (set->light_atten_linear_uloc[i] != -1)
				glUniform1f(set->light_atten_linear_uloc[i],  0.0 );
				
			if (set->light_atten_squared_uloc[i] != -1)
				glUniform1f(set->light_atten_squared_uloc[i],  0.0);
		}
			
	}

	printf("Ambient sum: %f %f %f\n", ambientsum.VX, ambientsum.VY, ambientsum.VZ);

	
	
	if (set->ambient_light_uloc!=-1) 
		glUniform3fv(set->ambient_light_uloc, 1, ambientsum.array);
	
				
}


//junk function to draw a triangle at the light source (it is crap)
void gx_debug_show_light(gx_light_t* light, zfloat32 size)
{
	
	glBegin(GL_TRIANGLES);

		glColor4f(light->color.named.x,light->color.named.y,light->color.named.z,1);
		glVertex3f(light->position.named.x, light->position.named.y, light->position.named.z);
		glVertex3f(light->position.named.x , light->position.named.y + size, light->position.named.z);
		glVertex3f(light->position.named.x + size , light->position.named.y , light->position.named.z);

	glEnd();

}




static gx_environment_t* current_env = NULL;

char* gxi_env_spec() {
	
	char* spec ;
	gx_environment_t* env = current_env;
	
	if (env->spec)
		return env->spec;
	
	int i;
	
	gx_light_t* li;
	
	spec = zstr_mk(100,0);
	
	if (env->usefog)
		spec = zstr_cat(spec, "GX_FOG|");
	
	for (i=0; i< zvec_count( &env->lights); i++) {
		char b[50];
		li = zvec_get_at(&env->lights, i);
		
		if (li->light_type == gx_light_directional)
			snprintf(b, sizeof(b), "GX_LIGHT%d|GX_LIGHT%dDIR|", i,i);
		else
			snprintf(b, sizeof(b), "GX_LIGHT%d|GX_LIGHT%dPOS|",i,i );
			
		spec = zstr_cat(spec, b);
	}


	
	env->spec = spec;
	
	return spec;
}



void gx_set_environment(gx_environment_t* env) {
	current_env = env;
	//printf(" env spec: %s\n", gxi_env_spec(env));
	//getc(stdin);
}

zbool _gx_environment_cleanup(void* ve) {
		gx_environment_t* env = ve;
		
		
	zvec_cleanup( & env->lights);
		
	return ZTRUE;
}

gx_environment_t*  gx_environment_mk(){
		gx_environment_t* env = ram_alloc( sizeof(*env), _gx_environment_cleanup);
		
		zvec_mk( & env->lights, 4);
		
		return env;
	
}


//set fog, lighting and other atrributes shaders might need
//if not using shaders, use the built in light/fog

static int ff_enabled_env= ZFALSE;  


void gxi_set_shader_env_params(gx_shader_t* set){
	int i;
	gx_environment_t* env = current_env;
	
	
	
	
	if (set && ff_enabled_env) {
			ff_enabled_env = ZFALSE;
			ff_set_active_lights(NULL, 0);
			glDisable(GL_FOG);
			printf(" Clear out FF environment settings\n");
	}
	
	
	if (set == NULL) {
		
		if (!env) {
			//if no env set, don't enable anything.
			glDisable(GL_LIGHTING);
			glDisable(GL_FOG);
			return;
		}
		ff_enabled_env = ZTRUE;  //if we switch to shaders, we should disable this'
		
		//set FF lighting parameters
		ff_set_active_lights( zvec_elements_as(gx_light_t*,&env->lights), zvec_count(&env->lights));
		
		//set ff fog parameters
		if (env->usefog) {
			float fc[4];
			glEnable(GL_FOG);
			glFogi(GL_FOG_MODE, GL_LINEAR);
			glFogf(GL_FOG_START, env->fogstartz);
			glFogf(GL_FOG_END, env->fogendz);
			fc[0]= env->fogcolor.array[0];
			fc[1]= env->fogcolor.array[1];		
			fc[2]= env->fogcolor.array[2];
			fc[3]=0;

			glFogfv(GL_FOG_COLOR,fc);

		} else
			glDisable(GL_FOG);
		
	} else {

		shader_set_active_lights(set, zvec_elements_as(gx_light_t*,&env->lights), zvec_count(&env->lights) );
		
		
		
	}

}




