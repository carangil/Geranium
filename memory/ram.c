// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define RAM_C


#include <malloc.h>
#include <string.h>
#include "ztypes.h"
#include "zmem.h"


#ifdef RAM_DEBUG
#include "zthread.h"
#include "zlist.h"
#endif

#define MYMAGIC 0xf1e2f3e4


/*****************
 *RAM allocation
 *****************/

/* stats */
static zuint32 ram_allocs_cnt = 0; //count of current allocations (to check for leaks)

zsize	z_global_ram_header_size = 0;  //advertises the size of an allocation wrapper header

#ifdef RAM_DEBUG
static zlockT  ram_debug_lock;
#endif
static zbool    zmem_inited=0;

/* Memory block header */


typedef struct mem_header_s
{
#ifdef RAM_DEBUG
	zlistnodeT zlistnode;
#endif
	ram_destructor destructor;
	int refcount;
	int shadow_size; //allow alloced buffers to have a shadow buffer of out-of-band data (lets zstrings be passed or ram_free'd like regular c strings, but allows additional metadata
	int magic;
#ifdef RAM_DEBUG
	char* file;
	int   line;
#endif
	
} mem_headerT;

#ifdef RAM_DEBUG
zlistT _ram_debuglist = {NULL,NULL};
#endif

void ram_init() {

  if (!zmem_inited){

	z_global_ram_header_size = sizeof(mem_headerT);
	
#ifdef RAM_DEBUG
        fprintf(stderr,"Creating ram debug lock\n");
		
        /* This creates the lock that memory shares when */
        zlock_init(&ram_debug_lock); 
   
#endif  
		zmem_inited = ZTRUE;
       }      

}

/* Allocate memory.  Takes size and destructor */
#ifdef RAM_DEBUG
void* ram_alloc_shadow_debug(zsize size, ram_destructor destructor, zuint32 shadow_size, char* file, int line)
#else
void* ram_alloc_shadow(zsize size, ram_destructor destructor, zuint32 shadow_size)
#endif
{
	mem_headerT* x;
	char * xbuffer;
	int rem = 0;

#ifdef LARGEST_ALLOC
	if ((shadow_size > LARGEST_ALLOC) || (size > LARGEST_ALLOC)){
		fprintf(stderr, "tried to alloc %d:%d but LARGEST_ALLOC is %d\n", size, shadow_size, LARGEST_ALLOC);
		return NULL;
	}
#endif

	if (!zmem_inited) {
		fprintf(stderr, "(warning)Auto-initing ram module.\n");
		ram_init();
	}
		
	if (shadow_size) {
		//keep alignment when we allocate the shadow buffer
		//printf(" Requested %d shadow bytes\n", shadow_size);
        
        rem = shadow_size % sizeof(void*);
        if (rem) {
            shadow_size += (sizeof(void*)) - rem;
        }
        
        //printf(" %d shadow bytes allocated\n", shadow_size); 
		
	}
		
	xbuffer = malloc( sizeof(mem_headerT)  + size + shadow_size); //allocate header + some size

	if (xbuffer)
	{
		memset(xbuffer, 0,  sizeof(mem_headerT)  + size  + shadow_size);
		
		x = (mem_headerT*) (xbuffer + shadow_size);
		if (shadow_size) {
				//printf("Allocate physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", xbuffer, shadow_size, x, x+1);
		}
		x->shadow_size = shadow_size;
		x->destructor = destructor;
		x->magic = MYMAGIC;
		x->refcount = 1;
                

#ifdef RAM_DEBUG
                int aa=zlock_inc(&ram_allocs_cnt);
				
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
	mem_headerT* header = (mem_headerT*) thing;
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
	mem_headerT* header = (mem_headerT*) thing;
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
			fprintf(stderr,"ERROR: negative refcount on %p\n", thing);
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
							  
#ifdef RAM_DEBUG
                              int x=  zlock_dec(&ram_allocs_cnt);
								
                                zlock(&ram_debug_lock);
                                
                                zlist_remove(&_ram_debuglist, &header->zlistnode);

                                zunlock(&ram_debug_lock);
#endif
                         
				buffer = (char*) header;
				buffer -= header->shadow_size;
				if (header->shadow_size) {
					//printf("Free physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", buffer, header->shadow_size, header, header+1);
				}
				free(buffer);
			}

		}
	}
}

void* ram_addref(void* thing)
{
	mem_headerT* header = (mem_headerT*) thing;
	if (header)
	{
		header --;
		header->refcount++;
	}
	return thing;
}

int ram_numrefs(void* thing)
{
	mem_headerT* header = (mem_headerT*) thing;
	if (header)
	{
		header --;
		if (header->magic != MYMAGIC){

			printf(" Attempt to count references for non-zmem object\n");
			return 0;
		}
		return header->refcount;
	}
	return 0;
}


void* ram_resize(void* ram, zsize size, zbool* okptr)
{
	char* buffer;
	int shadow_size;

	
	mem_headerT* header = (mem_headerT*) ram; //take pointer given to application
#ifdef RAM_DEBUG
        mem_headerT* oldheader;
#endif

	if (header)
	{
		header --; //decrement to header

		//cannot resize if more than one reference
		if (header->refcount !=1 )
		{
			fprintf(stderr, " can't resize if refcount !=1\n");
			
			if (okptr)
			    *okptr = ZFALSE;

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
				//printf("Realloc physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", buffer, shadow_size, header, header+1);
		}
	

#ifdef LARGEST_ALLOC
		if ((shadow_size > LARGEST_ALLOC) || (size > LARGEST_ALLOC)){
			fprintf(stderr, "tried to realloc %d:%d but LARGEST_ALLOC is %d\n", size, shadow_size, LARGEST_ALLOC);
			buffer = NULL;
		} else
#endif
		buffer = realloc(buffer, sizeof(mem_headerT) + size + header->shadow_size);  //attempt resize to new size;


		if (buffer)
			header = (mem_headerT*)(buffer + shadow_size);
		else 
			header = NULL;
		
	//	if (shadow_size) {
				//printf("Realloced to  physical buffer %p with shadow %d.  Header starts at %p Userdata at %p\n", buffer, shadow_size, header, header+1);
	//	}
		

#ifdef RAM_DEBUG
		if (header)          /*Put new one on */
			zlist_addhead(&_ram_debuglist, &header->zlistnode);
                else                    /*Put old one back on list */
                       	zlist_addhead(&_ram_debuglist, &oldheader->zlistnode);
                
                zunlock(&ram_debug_lock);
#endif

		if (header) {
			if (okptr)
				*okptr = ZTRUE;
			return header+1;
		}
		
	}
	
	if (okptr)
			okptr = ZFALSE;
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

	mem_headerT* node = zlist_head(&_ram_debuglist);
	

	while(node)
	{
		fprintf(stderr, "%p alloced at %s:%d (%d refs)\n",
			node+1, node->file, node->line, node->refcount	);
		count++;
		node = zlist_next(node);
	}
	
	fprintf(stderr, "%d unfreed allocations\n", count);

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
