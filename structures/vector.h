// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


//structure definition of a vector
typedef struct vec_s {
	zuint32 _size;		// max number of elements physical array can fit
	zuint32 count;		//count of elements placed in physical array
	void** elements;	//array of void pointers
	zbool own_elements;  //should free elements when destroing structure?
} vec_t;


//fast and safe read/write into the vector
#define vec_count(vec)			   ((vec)->count)
#define vec_get_at(vec,pos)        ( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] : NULL )
#define vec_set_at(vec,pos,value)  ( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] = value : NULL )
#define vec_get_x_at(vec, type, pos)  ((type)( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] : NULL ))
#define vec_elements(vec)  ((vec)->elements)
#define vec_elements_as(type, vec)   ((type*)((vec)->elements))

//what index contains item pointer?  (caution: O(n))
int vec_find_idx(vec_t* v, void* item);


//call this to cleanup the contents of a vec_t, without freeing the vec_t itself
void vec_cleanup(vec_t* x);

//constructor
vec_t* vec_mk(vec_t* v, zuint32 initial_size);

/* There are two ways to deal with allocation:

preallocated vec_t:


vec_t vec1;
vec_mk(&vec1, 10);
vec_add(&vec1, ram_strdup("This is whatever"));
vec_cleanup(&vec1);


dynamic allocation of the vec_t:

vec_t*  myVec = NULL;
myVec = vec_mk( NULL, 10);
vec_add( myVec, ram_strdup("This is whatever"));
ram_free(myVec);

*/

//add item to vector, growing if necessary
zbool vec_add(vec_t* v, void* item);

//add item to vector, returns item if successful.  otherwise frees the item
//why does this exist?
//something = vec_add_or_free(v, ram_alloc(...,...));  returns a new item in a vector, or kills it without an extra cleanup step!
void* vec_add_or_free(vec_t* v, void* item);

//remove item from vector in constant time, order of the items is not preserved
void* vec_remove_unordered(vec_t* v, zuint32 index);

//remove item from vector, O(n) time order of the items is preserved
void* vec_remove_ordered(vec_t* v, zuint32 index);

//simple diag function to print all the elements of the vector out as strings
void vec_print( vec_t* v);

//disowns vectors contents (User will have to free things that were put in the vector)
void vec_disown(vec_t* v);
