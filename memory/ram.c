// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define _RAM_C

#include "../ztypes.h"
#include <malloc.h>
#include <string.h>
#include "ram.h"
#include <stdio.h>

#ifdef RAM_DEBUG
#include "../structures/linkedlist.h"
#endif

/*****************
 *RAM allocation
 *Note:  This is NOT yet thread safe.
 *****************/

/* stats */
static zuint32 _ram_allocs = 0; //count of current allocations (to check for leaks)

/* Memory block header */

typedef struct mem_header_s
{
#ifdef RAM_DEBUG
	zlistnode_t zlistnode;
#endif
	ram_destructor destructor;
	int refcount;
#ifdef RAM_DEBUG
	char* file;
	int   line;
#endif
} mem_header_t;

#ifdef RAM_DEBUG
zlist_t _ram_debuglist = {NULL,NULL};
#endif


void* _ram_pending_free = NULL;


//free later
void ram_destructor_tail(void* block)
{
	if (!block)
		return;

	if (_ram_pending_free)
	{
		printf(" Warning: ram_destructor_tail called twice in one destructor\n");
		ram_free(block);
	}
	else
	{
		_ram_pending_free = block;
	}
}



/* Allocate memory.  Takes size and destructor */
#ifdef RAM_DEBUG
void* ram_alloc_debug(zsize size, ram_destructor destructor, char* file, int line)
#else
void* ram_alloc(zsize size, ram_destructor destructor)
#endif
{
	mem_header_t* x;

		
	x = malloc( sizeof(mem_header_t)  + size); //allocate header + some size

	if (x)
	{
		memset(x, 0,  sizeof(mem_header_t)  + size);
		_ram_allocs++;  //count allocations

		x->destructor = destructor;

		x->refcount = 1;

#ifdef RAM_DEBUG
		x->file = file;
		x->line = line;
		zlist_addhead(&_ram_debuglist, &x->zlistnode);
#endif

		return x + 1;  //return just past the header
	}

	return NULL; //failed to allocate
}

//executes a block's destructor
void ram_free(void* thing)
{
	mem_header_t* header = (mem_header_t*) thing;
	zbool do_free = ztrue;

	if (! thing)
		return;

	//if (header) 
	while (1)  //might free more than 1 item
	{
		header--; //decrement pointer to header struct
	
		header->refcount --;
		if (header->refcount ==0)
		{
			if (header->destructor)  //if a destructor was declared
			{
				//call the destructor
				//if the destructor returns true, free it
				do_free = header->destructor(thing); 
				
			}
			
			if (do_free)
			{
#ifdef RAM_DEBUG
				zlist_remove(&_ram_debuglist, &header->zlistnode);
#endif
				_ram_allocs--;
				free(header);
			}

		}

		//if there are 'leftover' items to free, lets continue
		if (_ram_pending_free)
		{
			header = thing = _ram_pending_free;
			_ram_pending_free = NULL;
			continue;
		}

		break;
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

void* ram_resize(void* ram, zsize size)
{

	mem_header_t* header = (mem_header_t*) ram; //take pointer given to application

	if (header)
	{
		header --; //decrement to header

		//cannot resize if more than one reference
		if (header->refcount !=1 )
			return NULL;

#ifdef RAM_DEBUG
				zlist_remove(&_ram_debuglist, &header->zlistnode);
#endif

		header = realloc(header, sizeof(mem_header_t) + size);  //attempt resize to new size;

#ifdef RAM_DEBUG
		if (header)
				zlist_addhead(&_ram_debuglist, &header->zlistnode);
#endif
	


		if (header) {
			return header+1;
		}
		else
			return NULL;  //could not resize, return NULL
	}

	return NULL;
}
/*
void* ram_clear(void* v, zsize size)
{
	if (v)
		memset(v, 0, size);	

	return v;
}
*/
char* ram_strdup_func(char* in, char* file, int line)
{
	size_t len;
#ifdef RAM_DEBUG
	char* x = ram_alloc_debug(len = (strlen(in) + 1), NULL, file, line);
#else
	char* x = ram_alloc(  len = (strlen(in) + 1) , NULL); //allocate
#endif

	if (x)
	{
		strncpy(x, in, len);
	}

	return x;
}

char* ram_strdup_cat(char* in1, char* in2)
{
	zsize in1_len = strlen(in1);
	zsize in2_len = strlen(in2);

	zbyte* x = ram_alloc( in1_len + in2_len + 1, NULL); //allocate
	
	if (x)
	{
		strcpy(x, in1);
		strcpy(x+in1_len, in2);
	}

	return x;
}

//returns number of open allocations
zuint32 ram_allocs()
{

#ifdef RAM_DEBUG
	int count=0;

	mem_header_t* node = zlist_head(&_ram_debuglist);

	

	while(node)
	{
		printf("%p alloced at %s:%d (%d refs)\n",
			node+1, node->file, node->line, node->refcount	);
		count++;
		node = zlist_next(node);
	}

	if (_ram_allocs != count)
		printf(" Internal inconsistency in ram.c, oops\n");

#endif

	return _ram_allocs;
}

