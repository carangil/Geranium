// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.
#include "ztypes.h"
#include <malloc.h>
#include <string.h>

#pragma once

int ram_shadow_offset(size_t s);

//destructor returns true if the ram should be freed
typedef zbool (*ram_destructor)(void* block);

#ifdef RAM_DEBUG

#define ram_free_internal(TTT)  ram_free_debug(TTT, __FILE__, __LINE__)

#ifndef RAM_C
//If debugging memory, all source files that are not ram.c, all calls to allocate have line numbers passed thru automatically
//Calls to ram_alloc_debug and ram_alloc_shadow_debug can specify alternate file/line info (like zstring might wanna pass thru where zstring was called from... knowing it was zstring that did the allocation isn't too useful)
#define ram_alloc(SSS,DDD) ram_alloc_debug (SSS,DDD,__FILE__, __LINE__)
#define ram_alloc_shadow(SSS,DDD,SHSH) ram_alloc_shadow_debug (SSS,DDD,SHSH, __FILE__ , __LINE__)


#define ram_free(TTT)  ram_free_debug(TTT, __FILE__, __LINE__)

#endif 

void* ram_alloc_debug(zsize size, ram_destructor destructor, char* file, int line);
void* ram_alloc_shadow_debug(zsize size, ram_destructor destructor, zuint32 shadowsize, char* file, int line);
void ram_free_debug(void* thing, char* file, int line);
#else  //NON-DEBUGGING MEMORY

//Just call the allocator directly (no line numbers)
void* ram_alloc(zsize size, ram_destructor destructor);
void* ram_alloc_shadow(zsize size, ram_destructor destructor, zuint32 shadowsize);
void  ram_free(void* thing);  //call on an object to dec refcount, ultimately calling its destructor when refcount==0


//when 'debug' ram call is done explicitly (to pass thru line numbers), strip it off and call the non-debug version
#define ram_alloc_debug(SSS,DDD, FFF, LLL)  ram_alloc(SSS,DDD);
#define ram_alloc_shadow_debug(SSS,DDD, SHSH, FFF, LLL)  ram_alloc(SSS,DDD, SHSH);


#endif

/* If multiple threads will be allocating memory, ram_init must be called before creating those threads to prevent a race condition. */
void ram_init();

//returns true if resize successful. Replaces pointer with new one
void* ram_resize(void* ram, zsize size, zbool* successful);
void* ram_addref(void* thing);

//char* ram_strdup(char* in);
char* ram_strdup_func(char* in, char* file,  int line);
#define ram_strdup(xx) ram_strdup_func(xx,__FILE__,  __LINE__)

char* ram_strdup_cat(char* in1, char* in2); //cat two string together into a new buffer

int ram_numrefs(void* ptr);

char* ram_loadstr(char* filename);  //load contents of filename into a buffer

//void* ram_clear(void* v, zsize size);
#define ram_clear(ZMEM, ZSIZE)  memset(ZMEM, 0, ZSIZE)

//return number of things allocated.  this also prints out the memory use table, if RAM_DEBUG is enabled
zuint32 ram_allocs();

void* ram_shadow(void* thing);

extern void (*abyss)(void* unknown, char* file, int line);
//'abyss' is global and can be assigned to any function that takes a void*.  It is called on pointers that get a negative refcount
//These are double-free type crashes.  the object might not even be valid.  this can be used for debugging if the same object keeps
//crashing and you need to examine that object

extern void* flagged; //If this is set to non-null and addref/free will call into abyss


//ram_malloc_interface can be passed to functions that expect a pointer to malloc.  No destructor will be available, but functions that take in a malloc function pointer handle their own memory
//ram_free can be passed as a free interface already
void* ram_malloc_interface(size_t size);
#define ram_free_interface ram_free



//GPU allocation

typedef struct gpu_storageS {
	int buffer;
}gpu_storageT;

gpu_storageT* gpu_storage(void* v);

//forbid standard functions
#ifndef RAM_C
#define malloc	dont_use_malloc
#define free	dont_use_free
#define realloc dont_use_realloc
#define strdup  dont_use_strdup
#endif
