#include "../ztypes.h"
#include "../memory/ram.h"
#include "zarray.h"

zbool zarray_resize_func(zarray_t* za, void** to_elements, int newsize_in, zsize elementsize, zbool doubling)
{
	void* newelements;
	int newsize = newsize_in;
	size_t sz;

	if (!to_elements || !*to_elements)
		return ZFALSE;  //fail

	//if doubling is enabled, and we are growing
	if (doubling && ( newsize > za->size) ) 
	{
		//if oldsize is passed in
		if ( za->size *2 > newsize)
			newsize = za->size * 2;
	}

	sz =  newsize * elementsize;

#ifdef ZARRAY_DEBUG
	fprintf(stderr, " resizing zarray to %d of %d (tot %d) dbl:%d req:%d\n", newsize, (int)elementsize, (int)sz, (int) doubling, newsize_in);	
#endif
	
	newelements = ram_resize(*to_elements, sz);

	if (newelements) 
	{


		*to_elements = newelements;
		za->size = newsize;

		//shrink count if going smaller
		if (za->count > za->size)
			za->count = za->size;

		return ZTRUE;
	}

	return ZFALSE;
}


zsize debug_amount( zsize esize , int ecount)
{
	fprintf(stderr, " selected size %d from %d of %d\n", (int) (ecount* esize), ecount, (int) esize);

	return ecount * esize;
}


