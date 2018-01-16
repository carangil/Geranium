#include "GL/glew.h"
#ifdef _WIN32
#include "GL/wglew.h"
#endif

#include "GL/freeglut.h"


//track how many items are generated or deleted in gl
extern int _gx_gl_textures_gen;
extern int _gx_gl_vbos_gen;
extern int _gx_gl_textures_del;
extern int _gx_gl_vbos_del;

