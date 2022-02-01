#include "../ztypes.h"
#include "../memory/zmem.h"
#include "zarray.h"

//return number of items array can hold
int zarray_sizef(void* array) {

	array_shadowT* sh = ram_shadow(array);
	if (sh)
		return sh->capacity;
	fprintf(stderr, "No shadow on array %p\n", array);
	return 0;
}

//return number of items inserted in array (if using append functions, etc)
int zarray_countf(void* array) {

	array_shadowT* sh = ram_shadow(array);
	if (sh)
		return sh->used;
	fprintf(stderr, "No shadow on array %p\n", array);
	return 0;
}

void* zarray_allocf( int elemsize, int elemnum){
	size_t size = elemsize * elemnum;
	printf(" array needs %d\n", size);

	void * array = ram_alloc_shadow(size, NULL, sizeof(array_shadowT));

	array_shadowT * sh = ram_shadow(array);
	if (sh){
		sh->used = 0;
		sh->capacity = elemnum;
	}

	return array;
}

void* zarray_resizef(void* array, int elemsize, int elemnum, zbool* ok){
	size_t newsize = elemsize * elemnum;
	printf(" array resize needs %d for %d * %d\n", newsize, elemsize, elemnum);

	void * newarray = ram_resize(array, newsize, ok);

	array_shadowT * sh = ram_shadow(array);
	if (newarray && sh){
		printf(" resize successful\n");
//		sh->used = 0;
		sh->capacity = elemnum;
		return newarray;
	}

	//did not get shadow, or did not get resize
	if (ok) {
		*ok = ZFALSE;
		return NULL;  //return null value
	} 
		
	//did not get anything, and user didn't pass in a ok return pointer
	//so we die
	fprintf(stderr, "Array resize failed, and function not given an 'bool' check to recover from failure, so it is fatal.  Pass in zbool &ok to allow failures to be nonfatal\n");

	return NULL;
}
