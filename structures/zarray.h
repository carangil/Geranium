#include "../ztypes.h"

#ifndef ZARRAY
#define ZARRAY

void* zarray_allocf( int elemsize, int elemnum);




void* zarray_resizef(void* array, int elemsize, int elemnum, zbool* ok);

#define zarray_alloc(ARRAYTYPE, len)  	zarray_allocf( sizeof(ARRAYTYPE), len)
#define zarray_resize(ARRAYNAME, NEWSIZE, ISOK)  zarray_resizef(ARRAYNAME, sizeof(*ARRAYNAME), NEWSIZE, ISOK)

typedef struct array_shadow{
	int capacity; //number of elements allocated
	int used; //number of elements used (optional)
} array_shadowT;


//function versions of count and size... slower but reliable
int zarray_countf(void* array) ;
int zarray_sizef(void* array) ;


//experimental macro versions of the above functions.  They seem to work fine...
//Take a pointer to the array, subtract the size of the allocation wrapper, and then the size of the shadow struct
// malloc returns pointer to [ array_shadowT | zmem_headerT |  C array ]
// And zarrays are passed around as pointers to the C array
#define zarray_count(ARRAY)   (((array_shadowT*) (  ((char*)(ARRAY)) - z_global_ram_header_size - sizeof(array_shadowT)))->used)
#define zarray_size(ARRAY)   (((array_shadowT*) (  ((char*)(ARRAY)) - z_global_ram_header_size - sizeof(array_shadowT)))->capacity)


//fast append


#define zarray_hasspaceI(ARRAY)   ( zarray_count(ARRAY) < zarray_size(ARRAY))

#define zarray_expand(ARRAY) 	zarray_resizef(ARRAY, sizeof(*ARRAY), zarray_size(ARRAY)*2, NULL)


#define zarray_add(ARRAY, ITEM) (zarray_hasspaceI(ARRAY) ?  ARRAY[ zarray_count(ARRAY)++ ] = ITEM, ARRAY:  NULL  )



#endif
