// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>
#include "vector.h"
#include <string.h>

// A simple, generic vector 'class' in C.  
// A vector just stores void*

// Frees the internal contents of a vec_t structure 
void vec_cleanup(vec_t* vec)
{
	zuint32 i;

	if (!vec) 
		return; //what?

	if (vec->elements) //if elements array is defined 
	{
		for (i=0;i<vec->count;i++)
		{
			if (i==0x3af)
 				printf("boo");
			ram_free(vec->elements[i]);  //free each element
		}
		ram_shallow_free(vec->elements);  //free the array
	}
}

// This is the destructor function called by ram_free, passed as arg to ram_alloc
void _vec_destructor(void* x)
{
	vec_cleanup((vec_t*) x); //free the contents of this structure
	ram_shallow_free(x);	   //then free the 'host' structure
}

/* This creates a vector */ 
vec_t* vec_mk(vec_t* v, zsize initial_size)
{  
	zbool allocated = zfalse;

	if (!v)
	{
		//no input pointer specified, so just allocate one, with proper destructor
		v =  (vec_t*) ram_alloc( sizeof(vec_t), _vec_destructor);

		if (!v) 
			return NULL; //failed to allocate

		allocated = ztrue;  //flag that we created the allocation
	}

	v->_size = initial_size;
	v->count = 0;  //vector is empty
	v->elements = ram_alloc(v->_size * sizeof(void*), NULL );
	
	
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
zbool vec_add(vec_t* v, void* item)
{
	if (!v) 
		return NULL;

	if (!item)
		return NULL;

	v->count++;

	if ( (v->count) > (v->_size))
	{
		//too big, time to grow
		void ** el = ram_resize(v->elements, v->_size * sizeof(void*) *2);
		
		if (el)
		{
			v->elements=el;
			v->_size*=2;
		}
		else
		{
			return zfalse;  //could not store item
		}
	}

	v->elements [ v->count-1] = item;
	return ztrue;
}

//tries to add item to vector. if succesful returns item. if unsuccessfull returns null and FREEs the item
void* vec_add_or_free(vec_t* v, void* item)
{
	if ( vec_add(v, item))
		return item;

	ram_free(item);
	return NULL;
}

//removes an element, but does not preserve order of the items (last item fills the place of the removed item)
//returns the element being removed, does not free it
void* vec_remove_unordered(vec_t* v, int index)
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
void* vec_remove_ordered(vec_t* v, int index)
{
	void* x = NULL;
	
	if (!v)
		return NULL;
	
	if (index >= v->count)
		return NULL; //out of range

	x = v->elements[index];  //grab item to remove

	//is this right?
		
	printf(" memmove to  %d from %d    %d  items \n", index, index+1, ((v->count - index)-1));

	memmove(v->elements+index, v->elements+index+1, sizeof(void*)* ((v->count - index)-1)   );

	v->count--;

	return x; //return the item
}


void vec_print( vec_t* v)
{	
	int a;
	printf( "Vector %p has size %d and count %d and elements:\n", v, v->_size, v->count);
	for (a=0;a<v->count;a++){
		printf("%d:%s\n",a, (char*) v->elements[a]);
	}
}

