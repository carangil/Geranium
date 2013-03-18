// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>
#include "glstuff.h"
#include "../vmath.h"
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


//simple lighting policy:
//if _number_active_lights is zero, gl lighting is disabled
//otherwise it is enabled

void gx_set_active_lights(gx_light_t** lights, zuint32 count)
{

	zuint32 i = 0;

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

		//set ambient light
		p[0]=lights[i]->ambient.array[0];
		p[1]=lights[i]->ambient.array[1];
		p[2]=lights[i]->ambient.array[2];
		p[3]=1;

		glLightfv(GL_LIGHT0+i, GL_AMBIENT,  p );
		

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

	//	glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, 0 );
	//	glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, 1 );

	}

	for (i= count; i < _number_active_lights;i++)
	{
		glDisable(GL_LIGHT0 + i);  //disable any lights we no longer want
	}

	_number_active_lights= count;

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



void gx_light_fog( vec3* color, float startz, float endz)
{

	float fc[4];

	if (!color)
		glDisable(GL_FOG);
	else
	{
		fc[0]=color->array[0];
		fc[1]=color->array[1];
		fc[2]=color->array[2];
		fc[3]=0.0;

		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogf(GL_FOG_START, startz);
		glFogf(GL_FOG_END, endz);
		glFogfv(GL_FOG_COLOR, fc);

	}

}