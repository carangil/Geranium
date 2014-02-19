#include "../ztypes.h"

//leads the header of all zarrays
typedef struct zarray_t
{
	zuint32 count; //number of items in array
	zuint32 size;  //number of items array can hold
} zarray_t;


//use zarray_def to declare an array
//zarray_def(char, boo)  will declare a char* boo, and metadata struct boo_za
//boo is a 'regular' c array.  after its inited, boo[3], etc are valid as lvalue or rvalue

#define zarray_def(zatypename, zaname)  zatypename* zaname; zarray_t zaname ## _za;

//return count and size of array
//zarray_count(boo) will return number of things in boo
//zarray_size(boo) will return number of things boo can hold before growing

#define zarray_count(zname)  zname ## _za.count
#define zarray_size(zname)  zname ## _za.size


//zarray_resize(boo, 20) will resize boo to 20.  will reduce count to 20 if it is larger

//resize array
#define zarray_resize(zname, newsize)  zarray_resize_func(   (zname ## _za)    , (void**) (zname), newsize,   sizeof((zname)[1]))


//zarray(boo, 5, 10) will size the array to hold 10 things, the current insert position will be at '5', so it is 5 full

#define zarray_init(zname, icount, isize)   (  (zname = ram_alloc(sizeof(zname[1])*icount, NULL))!=NULL?  zname ## _za.count=icount, zname ## _za.size=isize   : 0   )


zbool zarray_resize_func(zarray_t* za, void** to_elements, int newsize, zsize elementsize);

