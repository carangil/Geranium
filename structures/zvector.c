// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#include "ztypes.h"
#include "zmem.h"
#include <stdio.h>
#include "zvector.h"
#include <string.h>
#include <signal.h>

// A simple, generic vector 'class' in C.  
// A vector just stores void*

// Frees the internal contents of a zvecT structure 
void zvec_cleanup(zvecT* vec)
{
	zuint32 i;

	if (!vec) 
		return; //what?

	if (vec->elements) //if elements array is defined 
	{
		if (vec->own_elements)
		{
			for (i=0;i<vec->count;i++)
			{
				ram_free(vec->elements[i]);  //free each element
			}
		}
		ram_free(vec->elements);  //free the array
	}
}

// This is the destructor function called by ram_free, passed as arg to ram_alloc
zbool _zvec_destructor(void* x)
{
	zvec_cleanup((zvecT*) x); //free the contents of this structure
	return ZTRUE;
}

/* This creates a vector */ 
zvecT* zvec_mk(zvecT* v, zuint32 initial_size)
{  
	zbool allocated = ZFALSE;

	if (!v)
	{
		//no input pointer specified, so just allocate one, with proper destructor
		v =  (zvecT*) ram_alloc( sizeof(zvecT), _zvec_destructor);

		if (!v) 
			return NULL; //failed to allocate

		allocated = ZTRUE;  //flag that we created the allocation
	}

	v->_size = initial_size;
	v->count = 0;  //vector is empty
	v->elements = ram_alloc(v->_size * sizeof(void*), NULL );
	v->own_elements = ZTRUE;  //default that the vector owns all the elements stored within

	
	
	if (! (v->elements))
	{
		//failed to allocate elements
		if (allocated)
			ram_free(v);  //kill our vector
		
		v = NULL;
	}

	return v;  //return the vector (if created)
}

//add item to vector, if successful return true
zbool zvec_add(zvecT* v, void* item)
{
	if (!v) 
		return ZFALSE;

	if (v->_size == 0) {  //uninitialied!  need to init.  this is experimental
		printf(" warning: auto-init zvecT\n");
		zvec_mk(v, 4);
	}

	if ( (v->count) >= (v->_size))
	{
		zbool ok = ZFALSE;
		void* el = ram_resize(v->elements, v->_size * sizeof(void*) *2, &ok);
			
		if (ok && el) {
			v->_size*=2;
			v->elements = el;
		} else {
			return ZFALSE;  //could not store item
		}
		
	}

	v->elements [ v->count++] = item;

	return ZTRUE;
}

//tries to add item to vector. if succesful returns item. if unsuccessfull returns null and FREEs the item
void* zvec_add_or_free(zvecT* v, void* item)
{
	if ( zvec_add(v, item))
		return item;

	ram_free(item);
	return NULL;
}

//removes an element, but does not preserve order of the items (last item fills the place of the removed item)
//returns the element being removed, does not free it
void* zvec_remove_unordered(zvecT* v, zuint32 index)
{  
	void* x = NULL;
	
	if (!v)
		return NULL;
	
	if (index >= v->count)
		return NULL; //out of range

	x = v->elements[index];  //grab item to remove
	v->elements[index]=v->elements[--(v->count)]; //move last item there

	return x; //return the item
}

//removes item from vector, preserving the order
// O(n)
void* zvec_remove_ordered(zvecT* v, zuint32 index)
{
	void* x = NULL;
	
	if (!v)
		return NULL;
	
	if (index >= v->count)
		return NULL; //out of range

	x = v->elements[index];  //grab item to remove

	//is this right?
	
	#ifdef RAMDEBUG
	printf(" memmove to  %d from %d    %d  items \n", index, index+1, ((v->count - index)-1));
	#endif

	memmove(v->elements+index, v->elements+index+1, sizeof(void*)* ((v->count - index)-1)   );

	v->count--;

	return x; //return the item
}

//removes and returns the last item.  NULL is vector is empty
void* zvec_remove_last(zvecT* v)
{

	if (!v)
		return NULL;

	if (v->count==0){
		fprintf(stdout, "Empty vector\n");
		raise(SIGINT);
		exit(1);
		return NULL; //out of range
	}


	return v->elements[--(v->count)]; //return last item, decrementing count

}


int zvec_find_idx(zvecT* v, void* item)
{
	zuint32 a;
	for (a=0;a<v->count;a++)
	{
		if (item == v->elements[a])
			return a;
	}

	return -1;
}

/* Tells vector not to free elements when vector is destroyed */
/* returns the vector it modified so you can use syntax liek v = zvec_disown(zvec_mk(...)) ; */
 
zvecT*  zvec_disown(zvecT* v) {
	if (v) {
		v->own_elements = 0;
	}
	return v;
}


//returns vector as an array with optional count return value
void* zvec_detach(zvecT* v, int* n){
	void* array = v->elements;
	if (n)
		*n = v->count;

	v->count = 0;
	v->_size = 0;
	v->elements=NULL;
	return array;
}
