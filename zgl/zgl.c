#define ZGLC
#define GLAD_GL_IMPLEMENTATION

#include "zgl.h"


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


//SHADERS


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

//Links an opengl vertex and fragment shader or compute shader and returns a program, or 0 for failure
//vc: vertex or compute shader
//vf: fragment shader, only if vc is a vertex shader

int zglLinkProgramShaders(int vc, int f, char** rlog){

    if (!vc)
        return 0;

    int p = glCreateProgram();
    glAttachShader(p, vc);

    if (f)
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
    if (f)       glDeleteShader(v);

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

int zglBuildComputeProgram(char* header, char* src, char** rlog){

    tracef("header %s\n %sn\n", header?header:"none", src);

    char* vlog = NULL;
    char* plog = NULL;


    if (!header)
        header = "";

    char* vs[] = {header, src};

    int v = zglCompileShaderSource(GL_COMPUTE_SHADER, vs, 2, &vlog);


    int p = 0;

    if (v) {
        p = zglLinkProgramShaders(v, 0, &plog);
    }

    //delete any compiled shaders (no longer needed after successful link)
    if (v)
        glDeleteShader(v);


    if (rlog){

        *rlog = zstrprintf( *rlog, "{C:%s;Link:%s}", vlog, plog);
    }

    ram_free(vlog);
    ram_free(plog);

    return p;
}


//higher level interface to make some things more automatic to enable/disable defines


zbool cleanShaderGroup(void* v){
    zglShaderGroupT* sg = v;
    ram_free(sg->version);
    ram_free(sg->vsource);
    ram_free(sg->fsource);
    ram_free(sg->csource);
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
    if (optnames)
        zstrsplit(&sg->options, optnames, ',');
    sg->version = ram_strdup(version);
    sg->vsource = ram_strdup(vsrc);
    sg->fsource = ram_strdup(fsrc);
    return sg;
}

zglShaderGroupT* zglCreateComputeShader(char* version, char* src, char* optnames){
    zglShaderGroupT* sg = ram_alloc(sizeof(zglShaderGroupT), cleanShaderGroup); //todo destructor
    zvec_mk(&sg->options, 8);
    if (optnames)
        zstrsplit(&sg->options, optnames, ',');

    if (version)
        sg->version = ram_strdup(version);

    sg->csource = ram_strdup(src);

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

    char* optstr = NULL;

    if (sg->version)
        optstr = zstrdup(sg->version);

    //build header from #defines enabling options
    for (int i=0; i < zvec_count(&sg->options); i++){
        tracef(" test option %d  mask %d %s\n", i, m, zvec_get_x_at(&sg->options, char*, i));
        if (m&1){
           optstr =  zstrprintf(optstr, "\n#define OPT_%s\n", zvec_get_x_at(&sg->options, char*, i));
        }
        m=m>>1;
    }

    if (sg->csource)
        vari->prog = zglBuildComputeProgram( optstr, sg->csource, rlog);
    else
        vari->prog = zglBuildProgram( optstr, sg->vsource, sg->fsource, rlog);

    ram_free(optstr);

    return vari->prog;
}




//integration with zarray and zmemory

//These functions use 'gpu_storage' on top of zarrays and z alloced data structures
//Arrays of data (like [Vec4] [Vec2] can be stored on the GPU as buffer objects (VBO)
//Bitmaps can be stored on the GPU as Textures



#define ZGL_DIRTY       1
#define ZGL_RESIZE      2

typedef struct {
    int usage;
    int id; //bufferid
    int flags;
} zgl_buffer_storage_infoT;

typedef struct {
    zbitmapT* bitmap;
    int id; //texture id
    int flags;
} zgl_texture_storage_infoT;

typedef union {
    zgl_texture_storage_infoT texture;
    zgl_buffer_storage_infoT buffer;
} zgl_storage_infoT;

//binds the vbo associated with the array, creating one if necessary
//does not send any data, it just assigns a place for it



zgl_storage_infoT* izglBindArray(void* v, int target){
    zgl_storage_infoT* gpu = gpu_storage(v);

    if (!gpu) {
      printf("automatic GPU storage not available\n");
      exit(1);
    }

    if (!gpu->buffer.id){
        //create the buffer
        gpu->buffer.flags = ZGL_RESIZE;
        glGenBuffers(1, &gpu->buffer.id);
        printf(" Making new buffer %d for %p\n", gpu->buffer.id, v);

    }

    glBindBuffer(target, gpu->buffer.id);

    return gpu;
}

//bind array to target, return the ID
int zglBindArray(void* v, int target){
    //bind the array
    zgl_storage_infoT* gpu = izglBindArray(v, target);
    return gpu->buffer.id;
}

//set up the vertex attrib pointer for an array.
//this will bind the array to GL_ARRAY_BUFFER

int zglUseAttrib(int attridx, void* v, int gltype, int size){

    int n = zglBindArray(v, GL_ARRAY_BUFFER); //bind and/or create buffer

    if (gltype == GL_FLOAT){
        glVertexAttribPointer(attridx, size, gltype, GL_FALSE, size *sizeof(float), 0);
    } else {
        errorf("Unknown zglSetAttrib type\n");
        exit(1);
    }

    return n;
}

//pass in 0 for usage to not change the usage

//this creates VBO (if necessary)
//binds the VBO
//uploads the array contents (if dirty or the buffer is new/resized)
//returns the id of the bound buffer (0 for error)

int zglBindUpdateArrayf(void* v, int elemsize, int target, int usage){

    zgl_storage_infoT* gpu = izglBindArray(v, target);

    if (!usage) {//take existing usage, if exists
        usage = gpu->buffer.usage;
        printf(" taking existing usage %d\n", usage);

        if (!usage){
            usage = GL_DYNAMIC_DRAW;
            printf(" using dynamic draw as default\n");
        }

    }

    if (gpu->buffer.id){

        printf(" target %x buffer %d  %x\n", target, gpu->buffer.id, GL_ARRAY_BUFFER);
        //if changing usage or is set to resize
        if ( (usage != gpu->buffer.usage) || (gpu->buffer.flags&ZGL_RESIZE)   ){
            //if changing the target, usage, or if it has been resized, needs to make it
            glBufferData( target,  elemsize * zarray_size(v), v, usage);
            printf("call glBufferData %d * %d\n", elemsize, zarray_size(v));
            gpu->buffer.flags = 0; //reset
            //update usage and target
            gpu->buffer.usage = usage;
        } else if (gpu->buffer.flags & ZGL_DIRTY) {
            printf("call glBufferSubData to update dirty buffer\n");
            glBufferSubData(target, 0, elemsize * zarray_size(v) , v);
        } else {
           // printf("Do nothing to clean buffer\n");
        }
        gpu->buffer.flags =0; //clean
    }
    return gpu->buffer.id;

}

//sets an array as dirty (needs update)
//called directly by user
void zglDirtyBuffer(void* v){
    zgl_storage_infoT* store = gpu_storage(v);
    if (store)
        store->buffer.flags |= ZGL_DIRTY;
}

//sets an array as resized, for use by the memory allocator
void zglResizedCallback(void* v, size_t  newsize){
    zgl_storage_infoT* store = v;
    store->buffer.flags |= ZGL_RESIZE;
}

//for use by the memory allocator
//this is seperate from the user-specified destructor
void zglDeleteCallback(void* v){
    zgl_storage_infoT* store = gpu_storage(v);
    if (store){
            printf(" TODO delete buffer\n");
    }
}

//TEXTURES
//these give gpu storage to an existing zbitmapT object

void zglDirtyBitmap(zbitmapT* bmp){
    zgl_storage_infoT* store = gpu_storage(bmp);
    if (store)
        store->texture.flags |= ZGL_DIRTY;
}

int zglBindUpdateTextureBitmap(int unit, zbitmapT* bmp){
    zgl_storage_infoT* gpu = gpu_storage(bmp);
    zbool init = ZFALSE;

    if (!gpu->texture.id){
        //create the texture
        glGenTextures(1, &gpu->texture.id);
        gpu->texture.flags = ZGL_DIRTY | ZGL_RESIZE;
        init=ZTRUE;
        printf(" Created texture %d\n", gpu->texture.id);

    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, gpu->texture.id);

     printf(" Bound texture %d\n", gpu->texture.id);

    if (init){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if ( gpu->texture.flags & ZGL_DIRTY){
        //need to refresh
         printf(" update texture %d\n", gpu->texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  bmp->w , bmp->h,0, GL_BGRA, GL_UNSIGNED_BYTE, bmp->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        gpu->texture.flags &= ~(ZGL_DIRTY|ZGL_RESIZE);  //clear dirty or resize bits
    }

    return gpu->texture.id;
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
    EXPORT(zglCheckError);
    EXPORT(zglBuildProgram);
    EXPORT(zglCreateShaderGroup);
    EXPORT(zglFindVariant);
    EXPORT(zglBindUpdateArrayf);
    EXPORT(zglBindArray);
    EXPORT(zglUseAttrib);
    EXPORT(zglDirtyBuffer);
    EXPORT(zglBindUpdateTextureBitmap);
}
#endif

