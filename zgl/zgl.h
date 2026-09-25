#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"
#include "zarray.h"
#include "zbitmap.h"

#include "GL/glcorearb.h"

#define ZGL_ENABLE_INT

#ifdef ZGL_ENABLE_INT
//enable integration with interpreter
#include "int.h"
void zgl_int_init();
#endif



void zgl_test(int a, char* s);
zbool zgl_init(void* procgetter);

const char* zglErrorString(GLenum errorCode);
int zglCheckError(int spot);

/*

typedef struct zglShaderT {
	int v;
	int f;
	int program;
    char* log;
    zvecT symbols;
}zglShaderT;
*/

int zglBuildProgram(char* header, char* vsrc, char* fsrc, char** rlog);



typedef struct {
    int optionmask;  //bitmask of which inputs are used
    int prog;
}zglShaderVariantT;

typedef struct zglShaderGroup{
    char* version; //version of glsl
    char* vsource;
    char* fsource;
    zvecT options; //each char*
    zglShaderVariantT* variants;
}zglShaderGroupT;

zglShaderGroupT* zglCreateShaderGroup(char* version, char* vsrc, char* fsrc, char* optnames);

int zglFindVariant(zglShaderGroupT* sg, int optionmask, char** rlog);

typedef void* anyArray;

//mark array as dirty
void zglDirtyBuffer(void* v);

//update or create vbo from array.
#define zglBindUpdateArray(ARR,TARGET,USAGE) zglBindUpdateArrayf(ARR, sizeof((ARR)[0]) , TARGET, USAGE);
int zglBindUpdateArrayf(void* v, int elemsize, int target, int usage);

int zglUseAttrib(int attridx, void* v, int gltype, int size);

int zglBindArray(void* v, int target);

//textures


int zglBindUpdateTextureBitmap(int unit, zbitmapT* bmp);



//todo: texture arrays and such


