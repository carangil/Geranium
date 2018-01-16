// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/zmem.h"
#include <stdio.h>
#include "glheaders.h"
#include "../vmath/zmath.h"
#include "../structures/zvector.h"
#include "gx_image.h"
#include "gx_buffers.h"
#include "gx_drawstyle.h"
#include "gx_light.h"

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


//simple lighting policy:
//if _number_active_lights is zero, gl lighting is disabled


//sets active lights, but using opengl fixed function
static void ff_set_active_lights(gx_light_t** lights, zuint32 count)
{
	vec4 zero;
	vec4 ambientsum;
	zuint32 i = 0;
	printf(" setting ff lights\n");
	vec4set(zero,0,0,0,1);
	vec4set(ambientsum,0,0,0,1);

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
	}

	for (i=0;i<count;i++)
	{
		float p[4];

		//TODO: OpenGL wants 4 component vectors, but I had previously chosen to use 3 components
		//		perhaps this was a bad choice

		
		if (i>= _number_active_lights)
			glEnable(GL_LIGHT0 + i); //enable light if it hasn't been enabled before

		if (!lights[i]) 
			continue;

		//add up ambient light
		vec3add(ambientsum,lights[i]->ambient); 

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
		p[0]=lights[i]->position.named.x;
		p[1]=lights[i]->position.named.y;
		p[2]=lights[i]->position.named.z;

		if (lights[i]->light_type == gx_light_directional)
		{
			p[3]=0;
		}
		else
		{
			p[3]=1;
		}
	
		glLightfv(GL_LIGHT0+i, GL_POSITION, p);

		if (lights[i]->attenuated)
		{

			glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, 0 );
			glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, 1 / ( lights[i]->unityrange * lights[i]->unityrange ));


		}
		else
		{
			glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, 1 );
			glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, 0 );

		}
		
	}

	for (i= count; i < _number_active_lights;i++)
	{
		glDisable(GL_LIGHT0 + i);  //disable any lights we no longer want
	}

	_number_active_lights= count;
	

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientsum.array);

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

void gx_set_environment(gx_environment_t* env) {
	current_env = env;
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

void gxi_set_shader_env_params(gx_shaderset_t* set){
	gx_environment_t* env = current_env;
	
		if (set == NULL) {
			if (!env) {
				glDisable(GL_LIGHTING);
				glDisable(GL_FOG);
				return;
			}
			
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
			printf(" TODO: shader env\n");
			
		}
	
}




