
#define ZGLC
#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include "zgl.h"
//#include "zstringmap.h"




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

//Low-level helpers that work with opengl shader and program ids

#define LOGSIZE 1024

//Takes a source code string and returns a compiled opengl shader, or 0 for failure.  Returns optional log.
int zglCompileShaderSource(int stype, char** srcs, int n, char** rlog){
    char log [LOGSIZE];
    log[0]='\0';

    int sh = glCreateShader(stype);
    glShaderSource(sh, n, (void*)srcs, NULL);
    glCompileShader(sh);

    int status = 0;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(sh, 1024, NULL, log);

    debugf(" Shadertype %d number %d log:\n{%s}\n", stype, sh, log);

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

    debugf(" Program %d status %d log:{\n%s}\n", p, status, log);

    if (rlog)
        *rlog = zstrdup(log);

    if (status){
        return p;
    }

    glDeleteProgram(p);
    return 0;
}

int zglBuildProgram(char* header, char* vsrc, char* fsrc, char** rlog){

    tracef("header %s\n v %s\n f\n %s\n\n", header, vsrc, fsrc);

    char* vlog = NULL;
    char* plog = NULL;
    char* flog = NULL;

    if (! header)
        header = "";

    char* vs[] = {header, vsrc};
    char* fs[] = {header, fsrc};

    int v = zglCompileShaderSource(GL_VERTEX_SHADER, vs, 2, &vlog);

    int f = zglCompileShaderSource(GL_FRAGMENT_SHADER, fs, 2, &flog);
   // printf(" %d  %s\n", f, flog);

    //link if both shaders compiled
    int p = 0;

    if (v && f) {
        p = zglLinkProgramShaders(v, f, &plog);
    }

    //delete any compiled shaders (no longer needed after successful link)
    if (v)
        glDeleteShader(v);

    if (f)
        glDeleteShader(f);

    if (rlog){

        *rlog = zstrprintf( *rlog, "{V:%s;F:%s;Link:%s}", vlog, flog, plog);
    }

    ram_free(vlog);
    ram_free(flog);
    ram_free(plog);

    return p;
}

//higher level interface to make some things more automatic to enable/disable defines


zbool cleanShaderGroup(void* v){
    zglShaderGroupT* sg = v;
    ram_free(sg->version);
    ram_free(sg->vsource);
    ram_free(sg->fsource);
    ram_free(sg->variants);
    zvec_cleanup(&sg->options);

    return ZTRUE;
}

//creates a shader group, which is a set of shaders that share the same source, but have #define options
//lets to make attributes/uniforms optional
//Up to 32 options (probably too many!)
//optionnames are comma seperated
zglShaderGroupT* zglCreateShaderGroup(char* version, char* vsrc, char* fsrc, char* optnames){
    zglShaderGroupT* sg = ram_alloc(sizeof(zglShaderGroupT), cleanShaderGroup); //todo destructor
    zvec_mk(&sg->options, 8);
    zstrsplit(&sg->options, optnames, ',');
    sg->version = ram_strdup(version);
    sg->vsource = ram_strdup(vsrc);
    sg->fsource = ram_strdup(fsrc);
    ram_free(sg->variants);
    return sg;
}

//gets or creates a variant of a shader based on the optionmask
//bit N will refer to the Nth option, counting from LSB
//returns the opengl program number (for use with glUseProgram)
int zglFindVariant(zglShaderGroupT* sg, int optionmask, char** rlog){

    if (sg->variants){
        for (int i=0;i < zarray_count(sg->variants); i++){
            if (sg->variants[i].optionmask == optionmask){
                tracef(" FOUND EXISTING VARIANT i=%d for options %d\n",i, optionmask);
                return sg->variants[i].prog;  //return the shader
            }
        }
    }

    //not found
    if (!sg->variants)
        sg->variants = zarray_alloc(zglShaderVariantT, 4);
    else
        sg->variants = zarray_more(sg->variants, 1, NULL);  //make sure we have space

    debugf(" NEW VARIANT  for options %d\n",optionmask);

    zglShaderVariantT* vari = zarray_appendptr(sg->variants); //get the next slot

    vari->optionmask = optionmask;
    unsigned int  m = (unsigned int)optionmask;

    //start with the version string
    char* optstr = zstrdup(sg->version);

    //build header from #defines enabling options
    for (int i=0; i < zvec_count(&sg->options); i++){
        tracef(" test option %d  mask %d %s\n", i, m, zvec_get_x_at(&sg->options, char*, i));
        if (m&1){
           optstr =  zstrprintf(optstr, "\n#define OPT_%s\n", zvec_get_x_at(&sg->options, char*, i));
        }
        m=m>>1;
    }

    vari->prog = zglBuildProgram( optstr, sg->vsource, sg->fsource, rlog);

    ram_free(optstr);

    return vari->prog;
}




//integration with zarray and zmemory

#define ZGL_DIRTY   1
#define ZGL_RESIZE  2

typedef struct {
    int usage;
    int buffer;
    int flags;
} zgl_storage_infoT;


//binds the vbo associated with the array, creating one if necessary
//does not send any data, it just assigns a place for it

int zglBindArrayf(void* v, int target){
    zgl_storage_infoT* gpu = gpu_storage(v);

    if (!gpu) {
      printf("automatic GPU storage not available\n");
      exit(1);
    }

    if (!gpu->buffer){
        //create the buffer
        gpu->flags = ZGL_RESIZE;
        glGenBuffers(1, &gpu->buffer);
        printf(" Making new buffer %d for %p\n", gpu->buffer, v);
    }

    glBindBuffer(target, gpu->buffer);

    return gpu->buffer;

}


//pass in 0 for usage to not change the usage
//returns the id of the bound buffer (0 for error)
int zglBindUpdateArrayf(void* v, int elemsize, int target, int usage){
    zgl_storage_infoT* gpu = gpu_storage(v);

    if (!gpu) {
      printf("automatic GPU storage not available\n");
      exit(1);
    }

    if (!gpu->buffer){
        //create the buffer
        gpu->flags = ZGL_RESIZE;
        glGenBuffers(1, &gpu->buffer);
        printf(" Making new buffer %d for %p\n", gpu->buffer, v);
    }

    if (!usage) {//take existing usage, if exists
        usage = gpu->usage;
        printf(" taking existing usage %d\n", usage);

        if (!usage){
            usage = GL_DYNAMIC_DRAW;
            printf(" using dynamic draw as default\n");
        }

    }

    if (gpu->buffer){

        glBindBuffer(target, gpu->buffer);
        printf(" target %x buffer %d  %x\n", target, gpu->buffer, GL_ARRAY_BUFFER);
        //if changing usage or is set to resize
        if ( (usage != gpu->usage) || (gpu->flags&ZGL_RESIZE)   ){
            //if changing the target, usage, or if it has been resized, needs to make it
            glBufferData( target,  elemsize * zarray_size(v), v, usage);
            printf("call glBufferData %d * %d\n", elemsize, zarray_size(v));
            gpu->flags = 0; //reset
            //update usage and target
            gpu->usage = usage;
        } else if (gpu->flags & ZGL_DIRTY) {
            printf("call glBufferSubData to update dirty buffer\n");
            glBufferSubData(target, 0, elemsize * zarray_size(v) , v);
        } else {
           // printf("Do nothing to clean buffer\n");
        }
        gpu->flags =0; //clean
    }
    return gpu->buffer;

}


//sets an array as dirty (needs update)
//can be called directly by user
void zglDirty(void* v){
    zgl_storage_infoT* store = gpu_storage(v);
    if (store)
        store ->flags |= ZGL_DIRTY;
}



//sets an array as resized, for use by the memory allocator
void zglResizedCallback(void* v, size_t  newsize){
    zgl_storage_infoT* store = v;
    store->flags |= ZGL_RESIZE;
}

//for use by the memory allocator
//this is seperate from the user-specified destructor
void zglDeleteCallback(void* v){
    zgl_storage_infoT* store = gpu_storage(v);
    if (store){
            printf(" TODO delete buffer\n");
    }
}




//initializes this.  procgetter is glfwGetProcAddress or equivalent
zbool zgl_init(void* procgetter){

    if (!procgetter){
        debugf("No procgetter. (glfwGetProcAddress or similar must be passed.)");
    }

    debugf("Opengl loader %p\n", procgetter);

    if (!gladLoadGL(procgetter)) {
        errorf(" Cannot Load GLAD\n");
        return ZFALSE;
    }

    gpu_storage_interface.set_resize_flag = zglResizedCallback;
    //gpu_storage_interface.set_dirty_flag = zglDirty;
    gpu_storage_interface.delete_buffer = zglDeleteCallback;
    gpu_storage_interface.storeinfosize = sizeof(zgl_storage_infoT);

    return ZTRUE;
}


#ifdef ZGL_ENABLE_INT
#define EXPORT(XXX)  int_add_c_object( #XXX,XXX)

void zgl_int_init(){
    debugf(" adding interpreter C objects for zgl\n");

    EXPORT(zgl_init);
    EXPORT(zgl_test);
    EXPORT(zglCheckError);
    EXPORT(zglBuildProgram);
    EXPORT(zglCreateShaderGroup);
    EXPORT(zglFindVariant);
    EXPORT(zglBindUpdateArrayf);
    EXPORT(zglBindArrayf);
    EXPORT(zglDirty);
}
#endif

