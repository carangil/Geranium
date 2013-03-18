// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <malloc.h>

//destructor returns true if the ram should be freed
typedef zbool (*ram_destructor)(void* block);

void* ram_alloc(zsize size, ram_destructor destructor  );
void* ram_resize(void* ram, zsize newsize);
void  ram_free(void* thing);
void  ram_destructor_free(void* x);  //should only be called in a destructor
void  ram_destructor_tail(void* block);  //should only be called in a destructor: once per desructor right before returning


char* ram_strdup(char* in);
void* ram_clear(void* v, zsize size);

zuint32 ram_allocs();

//forbid standard functions
#ifndef _RAM_C
#define malloc	dont_use_malloc
#define free	dont_use_free
#define realloc dont_use_realloc
#define strdup  dont_use_strdup
#endif