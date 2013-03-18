// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include "../vmath.h"
#include <stdio.h>
#include "gx_image.h"
#include "gx_buffers.h"
#include "gx_drawstyle.h"



#include "glstuff.h"

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

	//set specular color
	{
		float white[]={1,1,1,1};
		float black[]={0,0,0,1};

		float f[4];

		if (style->use_constant_alpha) {
			white[3] = style->alpha;
		
		}else 
			white[3] = 1;

		f[0]=style->specular_color.array[0];
		f[1]=style->specular_color.array[1];
		f[2]=style->specular_color.array[2];
		f[3]=1;

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, style->specular_exponent);

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, f);

		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white); //setting diffuse and ambient to white will just pass-through the texture color
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
		
	}

	//activate all set textures
	gx_set_active_textures(style->textures, style->numtextures);

}