// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.
#include "ztypes.h"
#include <malloc.h>
#include <string.h>

extern zsize	z_global_ram_header_size;

//destructor returns true if the ram should be freed
typedef zbool (*ram_destructor)(void* block);

#ifdef RAM_DEBUG

#ifndef RAM_C
#define ram_alloc(SSS,DDD) ram_alloc_debug (SSS,DDD,__FILE__, __LINE__)
#define ram_alloc_shadow(SSS,DDD,SHSH) ram_alloc_shadow_debug (SSS,DDD,SHSH,__FILE__, __LINE__)
#endif 


void* ram_alloc_debug(zsize size, ram_destructor destructor, char* file, int line);
void* ram_alloc_shadow_debug(zsize size, ram_destructor destructor, zuint32 shadowsize, char* file, int line);
#else
void* ram_alloc(zsize size, ram_destructor destructor);
void* ram_alloc_shadow(zsize size, ram_destructor destructor, zuint32 shadowsize);
#endif

/* If multiple threads will be allocating memory, ram_init must be called before creating those threads to prevent a race condition. */
void ram_init();

//returns true if resize successful. Replaces pointer with new one
void* ram_resize(void* ram, zsize size, zbool* successful);
void* ram_addref(void* thing);
void  ram_free(void* thing);  //call on an object to dec refcount, ultimately calling its destructor when refcount==0

/* experimental ideas for recursive destructors */
void ram_delref(void* thing); //decrements reference to object.  returns true if object should be deleted
void ram_deallocate(void* v); //call only inside a destructor.  it will deallocate an item without calling its destructor.  
/* end experiment */

//char* ram_strdup(char* in);
char* ram_strdup_func(char* in, char* file,  int line);
#define ram_strdup(xx) ram_strdup_func(xx,__FILE__,  __LINE__)

char* ram_strdup_cat(char* in1, char* in2); //cat two string together into a new buffer


char* ram_loadstr(char* filename);  //load contents of filename into a buffer

//void* ram_clear(void* v, zsize size);
#define ram_clear(ZMEM, ZSIZE)  memset(ZMEM, 0, ZSIZE)

//return number of things allocated.  this also prints out the memory use table, if RAM_DEBUG is enabled
zuint32 ram_allocs();

void* ram_shadow(void* thing);

void ram_free(void* thing);

//forbid standard functions
#ifndef RAM_C
#define malloc	dont_use_malloc
#define free	dont_use_free
#define realloc dont_use_realloc
#define strdup  dont_use_strdup
#endif
