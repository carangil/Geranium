#include "ztypes.h"
#include "zmem.h"
#include "zarray.h"
#include <stdio.h>

//return number of items array can hold
int zarray_sizef(void* array) {

	array_shadowT* sh = ram_shadow(array);
	if (sh)
		return sh->capacity;
	fprintf(stderr, "No shadow on array %p\n", array);
	exit(1);
	return 0;
}

//return number of items inserted in array (if using append functions, etc)
int zarray_countf(void* array) {

	array_shadowT* sh = ram_shadow(array);
	if (sh)
		return sh->used;
	fprintf(stderr, "No shadow on array %p\n", array);
	exit(1);
	return 0;
}

void* zarray_set_meta(void* array, void* ptr){
	
	array_shadowT* sh = ram_shadow(array);
	if (sh)
		return (sh->userptr = ptr);
		
	fprintf(stderr, "No shadow on array %p\n", array);
	exit(1);
	return 0;
	
}

void* zarray_get_meta(void* array){
	
	array_shadowT* sh = ram_shadow(array);
	if (sh)
		return sh->userptr;
		
	fprintf(stderr, "No shadow on array %p\n", array);
	exit(1);
	return 0;
	
}


void* zarray_allocf( zsize elemsize, zuint32 elemnum, ram_destructor custom_destructor, char* file, int line){
	size_t size = elemsize * elemnum;

#ifdef STRUCT_DEBUG
	printf(" array needs %d\n", (int)size);
#endif

	
	//if array is byte array (elements are size one), allocate an extra byte
	if (elemsize==1)
	    size++;
	
	
	void * array = ram_alloc_shadow_debug(size, custom_destructor, sizeof(array_shadowT), file, line);

	if (array&&(elemsize==1)){
		    //if array is bytearray, keep a zero after all the elements
		    //(So that a byte array is always safe as a C string)
	    
		    ((char*)array)  [elemnum] = 0;
	}
	
	
	array_shadowT * sh = ram_shadow(array);
	if (sh){
		sh->used = 0;
		sh->capacity = (zuint32) elemnum;
	}
	return array;
}

void* zarray_resizef(void* array, zsize elemsize, zuint32 elemnum, zbool* ok){
	size_t newsize = elemsize * elemnum;
#ifdef STRUCT_DEBUG
	printf(" array resize needs %d /*for*/ %d * %d\n", (int)newsize, (int)elemsize, (int)elemnum);
#endif

	//allocate extra byte for byte arrays null terminator (in case we want C strings out of here)
	if (elemsize==1)
	    newsize++;
		
	
	void * newarray = ram_resize(array, newsize, ok);

	if (newarray && (elemsize == 1))
	    ((char*)newarray)[elemnum] = 0;	//null terminator for byte arrays
	
	
	array_shadowT * sh = ram_shadow(newarray);
	if (newarray && sh){
		
	 
		sh->capacity = elemnum;
#ifdef STRUCT_DEBUG
		printf(" resize successful %d %d\n", sh->used, sh->capacity);
#endif
		return newarray;
	}

	//did not get shadow, or did not get resize
	if (ok) {
		*ok = ZFALSE;
		return NULL;  //return null value
	} 
		
	//did not get anything, and user didn't pass in a ok return pointer
	//so we die
	fprintf(stderr, "Array resize failed, and function not given a 'bool' check to recover from failure.\n");
	exit(1);

	return NULL;
}

//copies part of source array to destination array
void zarray_copyf(void* dest, zuint32 pos, void* src, zuint32 srcstart, zuint32 srcend, zsize elemsize1, zsize elemsize2){
	if  ( (dest == NULL) || (src==NULL) || (elemsize1 != elemsize2)  ||(srcend < srcstart) ) {
		fprintf(stderr, "ERROR: array copy bad parms %p +%d by %d=  %p (+%d to %d) by %d\n", 
			dest, pos, (int)elemsize1, src, srcstart, srcend, (int)elemsize2);
	exit(1);
		return;
	}

	if (pos+(srcend-srcstart) > zarray_size(dest)) {
		fprintf(stderr, " Exceed dest array bounds on zarray_copy \n");
	exit(1);
	}
	
	memmove( ((char*)dest) + (elemsize1*pos) , ((char*)src) + (elemsize2*srcstart), elemsize2*(srcend-srcstart));

}

void zarray_use(void* array, zuint32 num){

	if(!array)
		return;

	zuint32 zs = zarray_sizef(array);

	if (num > zs) {
		fprintf(stderr, "Past array bounds %d/%d\n",num,zs );
	exit(1);
		return; //past end of array
	}

	zarray_count(array) = num;

}


void* zarray_appendf(void* dest, void* src, zbool grow2x, zbool* ok, zsize elemsize1, zsize elemsize2){
	if  ( (dest == NULL) || (src==NULL) || (elemsize1 != elemsize2)   ) {
		fprintf(stderr, "ERROR: array copy bad parms %p by %d=  %p  by %d\n", 
			dest, (int)elemsize1, src, (int)elemsize2);
	exit(1);
		return NULL;
	}
	
	zuint32 pos = zarray_count(dest);
	zuint32 srccount = zarray_count(src);

	zuint32 exact_needed = pos + srccount;
	zuint32 needed = exact_needed;
	
	//determine if we need extra for performance or string reasons:
	
	if (elemsize1 == 1){
#ifdef STRUCT_DEBUG
	    printf(" need %d, but alloc +1 for terminator\n", needed);
#endif
	    needed ++; //add space for byte array null terminator
	}

	
	//if using a doubleing (amortized O(1) growth performance like a vector)
	if (needed > zarray_size(dest)){
	    int size2x = zarray_size(dest) * 2;
	    if (grow2x && (size2x > needed)) {
		needed = size2x;
	    }
	}
#ifdef STRUCT_DEBUG
	printf(" append will resize to %d to acccomdate %d (+%d)\n", needed, pos+srccount, srccount);
#endif	
	//array resize guarantees that newarray will return a pointer OR ok will be set to false
	//if user did not pass us ok and it fails, zarray_resizef below will fail the whole program for us
	void* newarray = zarray_resizef(dest, elemsize1, needed, ok);
	
	if (newarray){
	    memmove(((char*)dest) + (elemsize1*pos) , ((char*)src) , elemsize2*(srccount));
	    zarray_use(newarray, exact_needed);

	    if (elemsize1 == 1) {
#ifdef STRUCT_DEBUG
		printf(" Putting null terminator at [%d]\n", exact_needed);
#endif
		((char*)newarray)[exact_needed]=0;
	    }

	}
		
	return newarray;
}


void zarray_debug(void* array){
    
	   
	
	if (array)
	    printf(" count/size:%d/%d count/size(slow):%d/%d\n", zarray_count(array), zarray_size(array), zarray_countf(array), zarray_sizef(array));    
	else
	    printf("zarray is null\n");
    
}
