#pragma once



#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"		//vector array (data structure)
#include "zvectormath.h"	//3d math

#ifdef GFXINTERNAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "gx_trans.h"
#include "gfx_texture.h"
#include "GL/glu.h"

void gfx_gl_test();