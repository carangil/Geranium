// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <malloc.h>


#define RAM_DEBUG


//destructor returns true if the ram should be freed
typedef zbool (*ram_destructor)(void* block);

#ifdef RAM_DEBUG
void* ram_alloc_debug(zsize size, ram_destructor destructor, char* file, int line);
#define ram_alloc(SSS,DDD) ram_alloc_debug(SSS,DDD,__FILE__, __LINE__)
#else
void* ram_alloc(zsize size, ram_destructor destructor);
#endif



void* ram_resize(void* ram, zsize newsize);
void* ram_addref(void* thing);
void  ram_free(void* thing);
void  ram_destructor_tail(void* block);  //should only be called in a destructor: once per desructor right before returning

char* ram_strdup(char* in);
char* ram_strdup_cat(char* in1, char* in2); //cat two string together into a new buffer

//void* ram_clear(void* v, zsize size);
#define ram_clear(ZMEM, ZSIZE)  memset(ZMEM, 0, ZSIZE)

zuint32 ram_allocs();

//allocate 1 item with the appropriate destructor.
#define ram_alloc_item(ZTYPE, ZDESTRUCTOR)  (ZTYPE*) ram_alloc(sizeof(ZTYPE), ZDESTRUCTOR)

//allocates n items. no destructor; intended for arrays of simple items (ints, etc)
#define ram_alloc_array(ZTYPE, COUNT)  (ZTYPE*) ram_alloc(sizeof(ZTYPE) * (ZCOUNT), NULL)

//forbid standard functions
#ifndef _RAM_C
#define malloc	dont_use_malloc
#define free	dont_use_free
#define realloc dont_use_realloc
#define strdup  dont_use_strdup
#endif