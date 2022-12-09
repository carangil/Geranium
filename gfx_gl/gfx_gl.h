#pragma once

#define GXDEBUG 1
#define gxdprintf  if(GXDEBUG) printf

#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"		//vector array (data structure)
#include "zvectormath.h"	//3d math
#include "math.h"

#ifdef GFXINTERNAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "gx_trans.h"
#include "gfx_texture.h"
#include "GL/glu.h"

#include "zwindow.h"

void gfx_gl_test();