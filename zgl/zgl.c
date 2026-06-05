
#define ZGLC
#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include "zgl.h"
//#include "zstringmap.h"


zbool zgl_init(void* procgetter){

    if (!procgetter){
        debugf("No procgetter. (glfwGetProcAddress or similar must be passed.)");
    }

    debugf("Opengl loader %p\n", procgetter);

    if (!gladLoadGL(procgetter)) {
        errorf(" Cannot Load GLAD\n");
        return ZFALSE;
    }

    return ZTRUE;
}


void zgl_test(int a, char* s){
        glClear(GL_COLOR_BUFFER_BIT);
        printf(" ZGL TEST %s %d\n", s, a);
}


int lastCheck = 0;

int zglCheckError(int spot){

    //returns an error, if there is one

    int e = glGetError();

    if (e!= GL_NO_ERROR){
        fprintf(stderr, "Opengl error %x '%s' between %d and %d\n", e, zglErrorString(e), lastCheck, spot);
    }
    lastCheck = spot;

    return e;
}


#define ZGL_SHADER_ARG_ATTR 1
#define ZGL_SHADER_ARG_UNIF 2

typedef struct{
    char* name;
    int category;
    int loc;
}zglShaderSymbolT;


//compiles a new shader
#define LOGSIZE 1024

//Takes a source code string and returns a compiled opengl shader, or 0 for failure.  Returns optional log.
int zglCompileShaderSource(int stype, char* src, char** rlog){
    char log [LOGSIZE];
    log[0]='\0';

    int sh = glCreateShader(stype);
    glShaderSource(sh, 1, &src, NULL);
    glCompileShader(sh);

    int status = 0;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(sh, 1024, NULL, log);

    printf(" Shadertype %d number %d log:\n{%s}\n", stype, sh, log);

    if (rlog)
        *rlog = zstrdup(log);

    if (status)
        return sh;

    glDeleteShader(sh);

    return 0;
}

//Links an opengl vertex and fragment shader and returns a program, or 0 for failure
int zglLinkProgramShaders(int v, int f, char** rlog){

    if (!v || !f)
        return 0;

    int p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);

    glLinkProgram(p);

    char log [LOGSIZE];
    log[0]='\0';
    int status=0;

    glGetProgramiv(p, GL_LINK_STATUS, &status);
	glGetProgramInfoLog(p, LOGSIZE, NULL, log);

    printf(" Program %d status %d log:{\n%s}\n", p, status, log);

    if (rlog)
        *rlog = zstrdup(log);

    if (status){
        return p;
    }

    glDeleteProgram(p);
    return 0;
}

int zglBuildProgram(char* vsrc, char* fsrc, char** rlog){

    char* vlog = NULL;
    char* plog = NULL;
    char* flog = NULL;



    int v = zglCompileShaderSource(GL_VERTEX_SHADER, vsrc, &vlog);

    printf(" %d  %s\n", v, vlog);

    int f = zglCompileShaderSource(GL_FRAGMENT_SHADER, fsrc, &flog);
    printf(" %d  %s\n", f, flog);

    int p = zglLinkProgramShaders(v, f, &plog);

    printf(" %d  %s\n", p, plog);


    if (rlog){
        *rlog = zstrprintf( NULL, "V:%s;F:%s;Link:%s\n", vlog, flog, plog);
    }

    ram_free(vlog);
    ram_free(flog);
    ram_free(plog);

    return p;
}



//end shaders

#ifdef ZGL_ENABLE_INT
#define EXPORT(XXX)  int_add_c_object( #XXX,XXX)

void zgl_int_init(){
    printf(" adding interpreter objects\n");
    EXPORT(zgl_init);
    EXPORT(zgl_test);
    EXPORT(zglCheckError);
    EXPORT(zglBuildProgram);
}
#endif

