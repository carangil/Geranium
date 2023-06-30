// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


//structure definition of a vector
#ifndef ZVECTOR_H
#define ZVECTOR_H

typedef struct zvec_s {
	zuint32 _size;		// max number of elements physical array can fit
	zuint32 count;		//count of elements placed in physical array
	zbool own_elements;  //should free elements when destroing structure?
	void** elements;	//array of void pointers
} zvecT;


//fast and safe read/write into the vector
#define zvec_count(vec)			   ((vec)->count)
#define zvec_get_at(vec,pos)        ( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] : NULL )
#define zvec_set_at(vec,pos,value)  ( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] = value : NULL )
#define zvec_get_x_at(vec, type, pos)  ((type)( ((unsigned int)(pos)) < (vec)->count ?  (vec)->elements[(pos)] : NULL ))
#define zvec_elements(vec)  ((vec)->elements)
#define zvec_elements_as(type, vec)   ((type*)((vec)->elements))
#define zvec_setcount(vec,value) ((vec)->count=(value))


//call this to cleanup the contents of a zvecT, without freeing the zvecT itself
void zvec_cleanup(zvecT* x);

//constructor
zvecT* zvec_mk(zvecT* v, zuint32 initial_size);

/* There are two ways to deal with allocation:

preallocated zvecT:


zvecT vec1;
zvec_mk(&vec1, 10);
zvec_add(&vec1, ram_strdup("This is whatever"));
zvec_cleanup(&vec1);


dynamic allocation of the zvecT:

zvecT*  myVec = NULL;
myVec = zvec_mk( NULL, 10);
zvec_add( myVec, ram_strdup("This is whatever"));
ram_free(myVec);

*/

//add item to vector, growing if necessary
zbool zvec_add(zvecT* v, void* item);

//add item to vector, returns item if successful.  otherwise frees the item
//why does this exist?
//something = zvec_add_or_free(v, ram_alloc(...,...));  returns a new item in a vector, or kills it without an extra cleanup step!
void* zvec_add_or_free(zvecT* v, void* item);

//remove item from vector in constant time, order of the items is not preserved
void* zvec_remove_unordered(zvecT* v, zuint32 index);

//remove item from vector, O(n) time order of the items is preserved
void* zvec_remove_ordered(zvecT* v, zuint32 index);


//disowns vectors contents (User will have to free things that were put in the vector)
zvecT* zvec_disown(zvecT* v);


#endif

