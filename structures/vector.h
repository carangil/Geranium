//structure definition of a vector
typedef struct vec_s {
	zuint32 _size;		// max number of elements physical array can fit
	zuint32 count;		//count of elements placed in physical array
	void** elements;	//array of void pointers
	zbool allocated;	//true if the vector structure was allocated
} vec_t;



//fast and safe access into the vector:
#define vec_count(vec)			   ((vec)->count)
#define vec_get_at(vec,pos)        ( (pos) < (vec)->count ?  (vec)->elements[(pos)] : NULL )
#define vec_set_at(vec,pos,value)  ( (pos) < (vec)->count ?  (vec)->elements[(pos)] = value : NULL )

//call this to release the allocated contents of a vector if the main vec_t structure is owned by something else
void vec_destructor(void* x);

//constructor
vec_t* vec_mk(vec_t* v, zsize initial_size);

//add item to vector, growing if necessary
zbool vec_add(vec_t* v, void* item);

//remove item from vector in constant time, order of the items is not preserved
void* vec_remove_unordered(vec_t* v, int index);

//remove item from vector, O(n) time order of the items is preserved
void* vec_remove_ordered(vec_t* v, int index);

//simple diag function to print all the elements of the vector out as strings
void vec_print( vec_t* v);


