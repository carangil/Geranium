// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#include "../ztypes.h"
#include <malloc.h>
#include <string.h>

/*****************
 *RAM allocation
 *Note:  This is NOT yet thread safe.
 *****************/

/* stats */
static zuint32 _ram_allocs = 0; //count of current allocations (to check for leaks)


/* Memory block header */

typedef struct mem_header_s
{
	void (*destructor)(void* block);
	int refcount;
} mem_header_t;


/* Allocate memory.  Takes size and destructor */
void* ram_alloc(zsize size, void (*destructor)(void*) )
{
	mem_header_t* x;

		
	x = malloc( sizeof(mem_header_t)  + size); //allocate header + some size

	if (x)
	{
		memset(x, 0,  sizeof(mem_header_t)  + size);
		_ram_allocs++;  //count allocations

		x->destructor = destructor;

		x->refcount = 1;

		return x + 1;  //return just past the header
	}

	return NULL; //failed to allocate
}

//executes a block's destructor
void ram_free(void* thing)
{
	mem_header_t* header = (mem_header_t*) thing;

	if (header) 
	{
		header--; //decrement pointer to header struct
	
		header->refcount --;
		if (header->refcount ==0)
		{
			if (header->destructor)  //if a destructor was declared
			{
				header->destructor(thing); //destruct this thing (destructor must call ram_free)
			}
			else
			{
				free(header);
				_ram_allocs--;
			}
		}

	}
}

void* ram_addref(void* thing)
{
	mem_header_t* header = (mem_header_t*) thing;
	if (header)
	{
		header --;
		header->refcount++;
	}
	return thing;
}

//frees a block without calling its destructor.
//does not obey reference counts
void ram_shallow_free(void* thing)
{
	mem_header_t* header = (mem_header_t*) thing;

	if (header) 
	{
		header--; //decrement pointer to header struct
		
		free(header);  //then free our block

		_ram_allocs--;
	}
}


void* ram_resize(void* ram, zsize size)
{

	mem_header_t* header = (mem_header_t*) ram; //take pointer given to application

	if (header)
	{
		header --; //decrement to header

		header = realloc(header, sizeof(mem_header_t) + size);  //attempt resize to new size;

		if (header)
			return header+1;
		else
			return NULL;  //could not resize, return NULL
	}

	return NULL;
}

void* ram_clear(void* v, zsize size)
{
	if (v)
		memset(v, 0, size);	

	return v;
}

char* ram_strdup(char* in)
{
	char* x = ram_alloc( strlen(in) + 1 , NULL); //allocate

	if (x)
	{
		strcpy(x, in);
	}

	return x;
}


//returns number of open allocations
zuint32 ram_allocs()
{
	return _ram_allocs;
}




