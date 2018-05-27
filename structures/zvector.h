// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


//structure definition of a vector
typedef struct zvec_s {
	zuint32 _size;		// max number of elements physical array can fit
	zuint32 count;		//count of elements placed in physical array
	void** elements;	//array of void pointers
	zbool own_elements;  //should free elements when destroing structure?
} zvec_t;


//fast and safe read/write into the vector
#define zvec_count(vec)			   ((vec)->count)
#define zvec_get_at(vec,pos)        ( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] : NULL )
#define zvec_set_at(vec,pos,value)  ( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] = value : NULL )
#define zvec_get_x_at(vec, type, pos)  ((type)( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] : NULL ))
#define zvec_elements(vec)  ((vec)->elements)
#define zvec_elements_as(type, vec)   ((type*)((vec)->elements))

//what index contains item pointer?  (caution: O(n))
int zvec_find_idx(zvec_t* v, void* item);


//call this to cleanup the contents of a zvec_t, without freeing the zvec_t itself
void zvec_cleanup(zvec_t* x);

//constructor
zvec_t* zvec_mk(zvec_t* v, zuint32 initial_size);

/* There are two ways to deal with allocation:

preallocated zvec_t:


zvec_t vec1;
zvec_mk(&vec1, 10);
zvec_add(&vec1, ram_strdup("This is whatever"));
zvec_cleanup(&vec1);


dynamic allocation of the zvec_t:

zvec_t*  myVec = NULL;
myVec = zvec_mk( NULL, 10);
zvec_add( myVec, ram_strdup("This is whatever"));
ram_free(myVec);

*/

//add item to vector, growing if necessary
zbool zvec_add(zvec_t* v, void* item);

//add item to vector, returns item if successful.  otherwise frees the item
//why does this exist?
//something = zvec_add_or_free(v, ram_alloc(...,...));  returns a new item in a vector, or kills it without an extra cleanup step!
void* zvec_add_or_free(zvec_t* v, void* item);

//remove item from vector in constant time, order of the items is not preserved
void* zvec_remove_unordered(zvec_t* v, zuint32 index);

//remove item from vector, O(n) time order of the items is preserved
void* zvec_remove_ordered(zvec_t* v, zuint32 index);

//simple diag function to print all the elements of the vector out as strings
void zvec_print( zvec_t* v);

//disowns vectors contents (User will have to free things that were put in the vector)
void zvec_disown(zvec_t* v);
