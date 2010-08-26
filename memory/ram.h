// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


void* ram_alloc(zsize size, void (*destructor) (void* thing)  );
void* ram_resize(void* ram, zsize newsize);
void  ram_free(void* thing);
void  ram_shallow_free(void* x);
char* ram_strdup(char* in);
void* ram_clear(void* v, zsize size);

zuint32 ram_allocs();

//forbid standard functions
#define malloc	dont_use_malloc
#define free	dont_use_free
#define realloc dont_use_realloc
#define strdup  dont_use_strdup