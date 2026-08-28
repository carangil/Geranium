
#ifndef ZARRAY
#define ZARRAY


void* zarray_allocf( zsize elemsize, zuint32 elemnum, ram_destructor custom_destructor, char* file, int line);
#define zarray_alloc_size(ELEMSIZE, ELEMNUM, DESTRUCT)  zarray_allocf( ELEMSIZE, ELEMNUM, DESTRUCT, __FILE__, __LINE__)

void* zarray_resizef(void* array, zsize elemsize, zuint32 elemnum, zbool* ok);

//allocare len items of ARRAYTYPE.  the array may have a custom destructor for when ram_free is called on it
//array is discarded by ram_free... can also be reference counted with ram_addref


//alloc allocates an array
//allocd allocates an array, but gives the block a custom destructor (so you can iterate the array elements and free stuff if you need)
#define  zarray_alloc(ARRAYTYPE, len)  	zarray_allocf( sizeof(ARRAYTYPE), len, NULL, __FILE__, __LINE__)
#define zarray_allocd(ARRAYTYPE, len, DESTRUCTOR)  	zarray_allocf( sizeof(ARRAYTYPE), len, DESTRUCTOR, __FILE__, __LINE__)
#define zarray_resize(ARRAYNAME, NEWSIZE, ISOK)  zarray_resizef(ARRAYNAME, sizeof(ARRAYNAME[0]), NEWSIZE, ISOK)

//pass zarray_destruct_pointers to allocd if the array is a bunch of pointers to free
zbool zarray_destruct_pointers(void* v);

#define zarray_copy(DEST,POS,SRC,SRCS,SRCE)\
	zarray_copyf(DEST, POS, SRC, SRCS, SRCE, sizeof(DEST[0]), sizeof(SRC[0]))

void zarray_copyf(void* dest, zuint32 pos, void* src, zuint32 srcstart, zuint32 srcend, zsize elemsize1, zsize elemsize2);




#define zarray_append_array(DEST,SRC,GROW2X,BOK)\
	zarray_append_arrayf(DEST, SRC, GROW2X, BOK, sizeof(DEST[0]), sizeof(SRC[0]))

void* zarray_append_arrayf(void* dest, void* src, zbool grow2x, zbool* ok, zsize elemsize1, zsize elemsize2);

void zarray_debug(void* array);

extern int zarray_global_shadow_offset;

typedef struct array_shadow{
	zuint32 capacity;	//number of elements allocated
	zuint32 used;		//number of elements used (optional)
	void* userptr;	
} array_shadowT;

//set 'num' number of elements as in use
void zarray_use(void* array, zuint32 num);

void* zarray_cow(void* array, int elemsize);


//each zarray can carry 1 pointer the calling function can use to store additional metadata
void* zarray_set_meta(void* array, void* ptr);
void* zarray_get_meta(void* array);


//function versions of count and size... slower but reliable
int zarray_countf(void* array) ;
int zarray_sizef(void* array) ;


#define ZARRAY_LINE , __FILE__, __LINE__


//experimental macro versions of the above functions.  They seem to work fine...
//Take a pointer to the array, subtract the size of the allocation wrapper, and then the size of the shadow struct
// malloc returns pointer to [ array_shadowT | zmem_headerT |  C array ]
// And zarrays are passed around as pointers to the C array
#define zarray_count(ARRAY)   (((array_shadowT*) (  ((char*)(ARRAY)) + zarray_global_shadow_offset))->used)
#define zarray_size(ARRAY)    (((array_shadowT*) (  ((char*)(ARRAY)) + zarray_global_shadow_offset))->capacity)


//checks if there is space for MORE number elements in the array
#define zarray_space(ARRAY, MORE)   ( (zarray_count(ARRAY)+(MORE)) <= zarray_size(ARRAY))


//resizes array to hold at least n more items.  Will resize to currentsize *2 or currentsize+n, whichever is bigger
void* zarray_moref(void* array, size_t itemsize, int n, zbool* ok);
#define zarray_more(ARRAY,N,POK)  zarray_moref(ARRAY, sizeof((ARRAY)[0]), N, POK)


//adds if enough space, or panics
#define zarray_append(ARRAY, ITEM)     ( zarray_space(ARRAY, 1)? (ARRAY)[ zarray_count(ARRAY)++] = ITEM, (ARRAY) : zarray_spaceerr() )
void* zarray_spaceerr();

//gives address of appended element, and increments count:      foo* f = zarray_appendptr(foos)    , adds a foo* to the array
#define zarray_appendptr(ARRAY)     ( zarray_space(ARRAY, 1)?  &(ARRAY)[ zarray_count(ARRAY)++] : zarray_spaceerr() )


#endif
