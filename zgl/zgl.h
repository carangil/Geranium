#include "ztypes.h"
#include "zmem.h"
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



typedef struct zglShaderT {
	int v;
	int f;
	int program;
    char* log;
    zvecT symbols;
}zglShaderT;


int zglBuildProgram(char* vsrc, char* fsrc, char** rlog);
