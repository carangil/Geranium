// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define RAM_C

#include "../ztypes.h"
#include <malloc.h>
#include <string.h>
#include "zmem.h"
#include <stdio.h>
#include "../thread/zthread.h"

#ifdef RAM_DEBUG
#include "../structures/zlist.h"
#endif

#define MYMAGIC 0xf1e2f3e4

/*****************
 *RAM allocation
 *****************/

/* stats */
static zuint32 ram_allocs_cnt = 0; //count of current allocations (to check for leaks)


#ifdef RAM_DEBUG
static zbool    ram_debug_lock_valid=0;
static zlock_t  ram_debug_lock;
#endif

/* Memory block header */


typedef struct mem_header_s
{
#ifdef RAM_DEBUG
	zlistnode_t zlistnode;
#endif
	ram_destructor destructor;
	int refcount;
	int shadow_size; //allow alloced buffers to have a shadow buffer of out-of-band data (lengths on cstring, CRAP like that)
	int magic;
#ifdef RAM_DEBUG
	char* file;
	int   line;
#endif
	
} mem_header_t;

#ifdef RAM_DEBUG
zlist_t _ram_debuglist = {NULL,NULL};
#endif

void ram_init() {

#ifdef RAM_DEBUG
  if (!ram_debug_lock_valid){
                fprintf(stderr,"Creating ram debug lock\n");
		
                /* This creates the lock that memory shares when */
                zlock_init(&ram_debug_lock); 
                ram_debug_lock_valid=ZTRUE;
       }      
#endif  

}

/* Allocate memory.  Takes size and destructor */
#ifdef RAM_DEBUG
void* ram_alloc_shadow_debug(zsize size, ram_destructor destructor, zuint32 shadow_size, char* file, int line)
#else
void* ram_alloc_shadow(zsize size, ram_destructor destructor, zuint32 shadow_size)
#endif
{
	mem_header_t* x;
	char * xbuffer;
    int rem = 0;

#ifdef RAM_DEBUG 
	if (!ram_debug_lock_valid) {
		fprintf(stderr, "(warning)Auto-initing ram module.\n");
		ram_init();
	}
#endif
		
	if (shadow_size) {
		//keep alignment when we allocate the shadow buffer
		printf(" Requested %d shadow bytes\n", shadow_size);
        
        rem = shadow_size % sizeof(void*);
        if (rem) {
            shadow_size += (sizeof(void*)) - rem;
        }
        
        printf(" %d shadow bytes allocated\n", shadow_size); 
		
	}
		
	xbuffer = malloc( sizeof(mem_header_t)  + size + shadow_size); //allocate header + some size

	if (xbuffer)
	{
		memset(xbuffer, 0,  sizeof(mem_header_t)  + size  + shadow_size);
		
		x = (mem_header_t*) (xbuffer + shadow_size);
		if (shadow_size) {
				printf("Allocate physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", xbuffer, shadow_size, x, x+1);
		}
		x->shadow_size = shadow_size;
		x->destructor = destructor;
		x->magic = MYMAGIC;
		x->refcount = 1;
                
                int aa=zlock_inc(&ram_allocs_cnt);

#ifdef RAM_DEBUG
				
                zlock(&ram_debug_lock);
                
		
		
		x->file = file;
		x->line = line;
		zlist_addhead(&_ram_debuglist, &x->zlistnode);
                
                zunlock(&ram_debug_lock);
#endif

		return x + 1;  //return just past the header
	}

	return NULL; //failed to allocate
}

//returns pointer to the shadow data allocated with an object.
//if the shadow size is zero, a null pointer is returned
void* ram_shadow(void* thing)
{
	mem_header_t* header = (mem_header_t*) thing;
	char* shadow = NULL;
	
	if (header) 
	{
		header--; //decrement pointer to header struct
		
		if (header->magic != MYMAGIC) {
			printf(" attempt shadow on bad magic!\n");
			return NULL;
		}
			
		if (header->shadow_size <=0 )  //return nothing if no shadow
			return NULL;
		
		shadow = (void*) header;
		shadow -= header->shadow_size;
		//printf(" returning shadow %p of header %p of block %p\n", shadow, header, thing);
		return shadow;
		
	}
	
	return NULL;
}

#ifdef RAM_DEBUG
void* ram_alloc(zsize size, ram_destructor destructor) {
	
	return ram_alloc_debug(size, destructor, NULL, 0);
}
#endif


#ifdef RAM_DEBUG
void* ram_alloc_debug(zsize size, ram_destructor destructor, char* file, int line){
		return ram_alloc_shadow_debug(size, destructor, 0, file, line);
}
#else
void* ram_alloc(zsize size, ram_destructor destructor) {
		return ram_alloc_shadow(size, destructor, 0);
}
#endif



//executes a block's destructor

void ram_free(void* thing)
{
	mem_header_t* header = (mem_header_t*) thing;
	zbool do_free = ZTRUE;
	char* buffer= NULL;

	if (! thing)
		return;

	if (header) 
	{
		header--; //decrement pointer to header struct
	
		header->refcount --;

		if (header->refcount<0)
		{
			fprintf(stderr,"negative refcount!\n");
		}

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
                              int x=  zlock_dec(&ram_allocs_cnt);
							  
#ifdef RAM_DEBUG
								
                                zlock(&ram_debug_lock);
                                
                                zlist_remove(&_ram_debuglist, &header->zlistnode);

                                zunlock(&ram_debug_lock);
#endif
                         
				buffer = (char*) header;
				buffer -= header->shadow_size;
				if (header->shadow_size) {
					printf("Free physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", buffer, header->shadow_size, header, header+1);
				}
				free(buffer);
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

void* ram_resize(void* ram, zsize size)
{
	char* buffer;
	int shadow_size;
	mem_header_t* header = (mem_header_t*) ram; //take pointer given to application
#ifdef RAM_DEBUG
        mem_header_t* oldheader;
#endif

	if (header)
	{
		header --; //decrement to header

		//cannot resize if more than one reference
		if (header->refcount !=1 )
		{
			fprintf(stderr, " can't resize if refcount !=1\n");
			return NULL;
		}
#ifdef RAM_DEBUG
                zlock(&ram_debug_lock);
		zlist_remove(&_ram_debuglist, &header->zlistnode);
                oldheader = header;
#endif
				
		shadow_size = header->shadow_size;
				
		buffer = (char*) header;
		buffer -=shadow_size;  //get back to the base block we allocated originally
		
		
		if (shadow_size) {
				printf("Realloc physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", buffer, shadow_size, header, header+1);
		}
		
		buffer = realloc(buffer, sizeof(mem_header_t) + size + header->shadow_size);  //attempt resize to new size;
		if (buffer)
			header = (mem_header_t*)(buffer + shadow_size);
		else
			header = NULL;
		
		if (shadow_size) {
				printf("Realloced to  physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", buffer, shadow_size, header, header+1);
		}
		

#ifdef RAM_DEBUG
		if (header)          /*Put new one on */
				zlist_addhead(&_ram_debuglist, &header->zlistnode);
                else                    /*Put old one back on list */
                       	zlist_addhead(&_ram_debuglist, &oldheader->zlistnode);
                
                zunlock(&ram_debug_lock);
#endif

		if (header) {
			return header+1;
		}
		else
			return NULL;  //could not resize, return NULL
	}

	return NULL;
}


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
		fprintf(stderr, "%p alloced at %s:%d (%d refs)\n",
			node+1, node->file, node->line, node->refcount	);
		count++;
		node = zlist_next(node);
	}

	if (ram_allocs_cnt != count)
		fprintf(stderr, " Internal inconsistency in ram.c, oops\n");

#endif

	return ram_allocs_cnt;
}


char* ram_loadstr(char* filename) {
	char* str = NULL;
	FILE* f;
	int len;

	f = fopen(filename, "rb");
	if (f){
		fseek (f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
#ifdef RAM_DEBUG
		fprintf(stderr, "ram.c load file %s : %d bytes\n", filename, len);	
#endif
		str = ram_alloc(len+1, NULL);

		if (str) {

			fread(str, len, 1, f);
			str[len]= '\0';
		}
		fclose(f);
	}

	return str;

}
