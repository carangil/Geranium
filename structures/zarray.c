#include "../ztypes.h"
#include "../memory/ram.h"
#include "zarray.h"

zbool zarray_resize_func(zarray_t* za, void** to_elements, int newsize, zsize elementsize)
{

	void* newelements;
	size_t sz =  newsize * elementsize;
		
	if (!to_elements || !*to_elements)
		return zfalse;

	printf(" resizing zarray to %d of %d (tot %d)\b", newsize, (int)elementsize, (int)sz);	

	newelements = ram_resize(*to_elements, sz);

	if (newelements) {
		za->size = newsize;

		//shrink count if going smaller
		if (za->count > za->size)
			za->count = za->size;

		*to_elements = newelements;
		return ztrue;
	}

	return zfalse;
}





