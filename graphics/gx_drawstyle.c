// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
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
		return;

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
	gx_set_active_textures(style->textures, style->numtextures);

}