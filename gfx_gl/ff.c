#define GFXINTERNAL
#include "ztypes.h"
#include "zmem.h"
#include "zwindow.h"
#include "gfx_gl.h"
#include "zvector.h"
#include "zstring.h"
#include "string.h"
#include "zarray.h"
#include "zarray.h"
#include <stdio.h>

extern zint32 matrix_version;

zint32 ff_matrix_version = -1;

//send our modelview & projection matrices to opengl
void ff_update_matrix(zfloat32* proj_matrix, zfloat32* matr) {

	if (ff_matrix_version == matrix_version) {
			gxdprintf("skip same ff matrix\n");
		return;

	}
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(proj_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(matr);
	//printMatrix44("ff", matr);

	ff_matrix_version = matrix_version;
}


//set texture layer blending optins

void ff_texture_env(zuint32 i) {

	if (i == 0)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //multiply againt light value
	else
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //default as alpha blending

}

//lighting
int ff_lights_used = ZFALSE;
zbool ff_lights_active[MAX_FF_LIGHTS];
vec3 ff_specular_color;
float ff_specular_exponent;

void ff_new_light_set() {
	memset(ff_lights_active, 0, sizeof(ff_lights_active));
	ff_lights_used = ZFALSE;

	//default specular settings
	ff_specular_exponent = 10.0;
	vec3set(ff_specular_color, 1, 1, 1);
}

//returns TRUE if fixed function handled the light property
zbool ff_light_parm(gfx_propertyT* p) {

	if (p->index >= MAX_FF_LIGHTS) //limit 
		return 1;  //'handled'

	switch (p->id) {


	case GXI_LIGHT_DIRECTION: //a directional light

		ff_lights_active[p->index] = ZTRUE;

		ff_lights_used = ZTRUE;
		glPushMatrix();
		glLoadIdentity();
		p->data.v4.named.w = 0.0; //direction light has position at w=0 'infinity' away
		glLightfv(GL_LIGHT0 + p->index, GL_POSITION, p->data.fa);
		glEnable(GL_LIGHT0 + p->index);
		glPopMatrix();
		return 1;


	case GXI_LIGHT_POSITION: //a positional light


		ff_lights_active[p->index] = ZTRUE;
		ff_lights_used = ZTRUE;
		glPushMatrix();
		glLoadIdentity();
		p->data.v4.named.w = 1.0; //w=1 defines an exact point
		glLightfv(GL_LIGHT0 + p->index, GL_POSITION, p->data.fa);
		glEnable(GL_LIGHT0 + p->index);
		glPopMatrix();
		return 1;

	case GXI_LIGHT_COLOR:

		glLightfv(GL_LIGHT0 + p->index, GL_DIFFUSE, p->data.fa);
		glLightfv(GL_LIGHT0 + p->index, GL_SPECULAR, p->data.fa);
		//vec4 zero = vec4const(0, 0, 0, 1);
		//glLightfv(GL_LIGHT0 + p->index, GL_SPECULAR, &zero);
		return 1;

	case GXI_SPECULAR_EXPONENT:  //specular exponent
		ff_specular_exponent = p->data.f;
		return 1;


	case GXI_SPECULAR_COLOR:  //specular color
		ff_specular_color = p->data.v;
		return 1;
	}


	return 0;

}

void ff_light_complete(vec4* ambientsum) {
	int i;

	if (ff_lights_used) {
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		ambientsum->VALPHA = 1.0;
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientsum->array);

		//have color changes change the material settings
		glColor4f(1, 1, 1, 1);  //if it happens there is no color vertex array data, use white as the color (which gets multiplied against the texture)
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ff_specular_color.array);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, ff_specular_exponent);


		//disable and ff lights that are not being used anymore
		for (i = 0; i < MAX_FF_LIGHTS; i++) {
			if (!ff_lights_active[i]) {
				printf(" dis ff light %d\n", i);
				glDisable(GL_LIGHT0 + i);
			}

		}

	}
	else {
		glDisable(GL_LIGHTING);
	}
}


