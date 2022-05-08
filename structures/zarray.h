
#ifndef ZARRAY
#define ZARRAY

void* zarray_allocf( zsize elemsize, zuint32 elemnum);




void* zarray_resizef(void* array, zsize elemsize, zuint32 elemnum, zbool* ok);

#define zarray_alloc(ARRAYTYPE, len)  	zarray_allocf( sizeof(ARRAYTYPE), len)
#define zarray_resize(ARRAYNAME, NEWSIZE, ISOK)  zarray_resizef(ARRAYNAME, sizeof(ARRAYNAME[0]), NEWSIZE, ISOK)


#define zarray_copy(DEST,POS,SRC,SRCS,SRCE)\
	zarray_copyf(DEST, POS, SRC, SRCS, SRCE, sizeof(DEST[0]), sizeof(SRC[0]))

void zarray_copyf(void* dest, zuint32 pos, void* src, zuint32 srcstart, zuint32 srcend, zsize elemsize1, zsize elemsize2);




#define zarray_append(DEST,SRC,GROW2X,BOK)\
	zarray_appendf(DEST, SRC, GROW2X, BOK, sizeof(DEST[0]), sizeof(SRC[0]))

void* zarray_appendf(void* dest, void* src, zbool grow2x, zbool* ok, zsize elemsize1, zsize elemsize2);

void zarray_debug(void* array);


typedef struct array_shadow{
	zuint32 capacity; //number of elements allocated
	zuint32 used; //number of elements used (optional)
} array_shadowT;

//set 'num' number of elements as in use
void zarray_use(void* array, zuint32 num);

//function versions of count and size... slower but reliable
int zarray_countf(void* array) ;
int zarray_sizef(void* array) ;



//experimental macro versions of the above functions.  They seem to work fine...
//Take a pointer to the array, subtract the size of the allocation wrapper, and then the size of the shadow struct
// malloc returns pointer to [ array_shadowT | zmem_headerT |  C array ]
// And zarrays are passed around as pointers to the C array
#define zarray_count(ARRAY)   (((array_shadowT*) (  ((char*)(ARRAY)) - z_global_ram_header_size - sizeof(array_shadowT)))->used)
#define zarray_size(ARRAY)   (((array_shadowT*) (  ((char*)(ARRAY)) - z_global_ram_header_size - sizeof(array_shadowT)))->capacity)


//checks if there is space for MORE number elements in the array
#define zarray_space(ARRAY, MORE)   ( (zarray_count(ARRAY)+(MORE)) <= zarray_size(ARRAY))

//Doubles the size of an array, returning a new pointer if necessary (returns old pointer if not)
#define zarray_expand(ARRAY) 	zarray_resizef(ARRAY, sizeof(ARRAY[0]), zarray_size(ARRAY)*2, NULL)

//check is array has space, and expand if not, returning a potentially new pointer
#define zarray_sizecheck(ARRAY, MORE)  (zarray_space(ARRAY,MORE)? (ARRAY) : zarray_expand(ARRAY) )

//fast append (only if has space)
#define zarray_add(ARRAY, ITEM) (zarray_space(ARRAY,1) ?  ARRAY[ zarray_count(ARRAY)++ ] = ITEM, ARRAY:  NULL  )



#endif
