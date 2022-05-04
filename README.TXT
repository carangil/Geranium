Zcore - Mars Sherman
--------------------

Why?

Over the years I've spent some time playing around with things in C.  Somethings, like memory management, lists, vectors, etc get used over and over, so I made them into a library.  It's called Zcore, because its the central core framework I was building Project Z on.  Z was a simple opengl-based rendering and graphics asset loading library that I occasionally add some work to.  I'm about to refactor the graphics part, but figured I could seperate out some generic functionality and use that in my other C projects.


Platform:  Mostly Linux.  There is some (stale) code for Windows, but no visual studio project files yet.  I do want to get it building in Windows again.

Components:

zmemory:
	A malloc wrapper with support for reference counting, recursive destructors, and 'shadow' allocations.  A shadow is just extra space added to an allocation that can contain user-defined additional data.  Example:

			typedef struct stringdata_s{
				int len;
				int capacity;
			}stringdata;
			
			char* str="This is a string";
			char* x = ram_alloc_shadow(  100 , NULL, sizeof(superstring));  //allocate the string, no special destructor, and allocate an extra struct
			strcpy (x, str);	//x is just a C string like normal
			stringdata* sd = ram_shadow(x); //get the allocated shadow buffer for this memory object
			sd.len = strlen(str);
			sd.capacity=100;

					
			The use of reference counting and destructors allows some automatic memory management.  It is fairly easy to build largeish data structures that can all be torn down with a single ram_free call.	

zstructures:
	Provides these data structures:

		zarray:
			An arr implementation compatible with C arrays, but uses zmemory's shadow buffers to track array size.
			zarrays are easily appended to each other or resized.

		zstring:
			A string implementation compatible with C string, built on top of the above zarray.  (null terminated zarray of chars)

		zvector:
			A simple object pointer vector implementation.  This was developed before zarray, but has different intended use.
			A zvector, when freed, will automatically free the contents, calling whatever destructor was provided during ram_alloc.
			A zarray is a single block of memory, and resizing creates a new pointer.  For zarrays, it is like C arrays, so there can only be one real 'owner' of the pointer, since zarray_resize will create a new buffer, all pointers to the buffer need to be updated.  Zvector keeps the array pointer indirect, so there can be multiple references to the same zvector.  The zvector will be freed (along with its contents recursively) only when the last reference is freed.

		zlist:
			Incomplete, basic implementation of a double linked list.  Required funcionality will be added when needed.  List and listnode code is generic.  Intended use is the user defines their own struct with zlistnodeT as its first element.

zthread:
	Provides an easy interface to create and join threads, simple locking and atomic inc/dec operations.

misc:
	Random numbers, reading microseconds off the clock, etc.  When any functionality gets large enough, it will be split off to its own module.


vmath:
	A simple implementation for 3D vector math.  The functions are intended to be used in an assembly-like manner:
	vec3set(a, 1.0, 2.0, 3.0);
	vec3set(b, 4.0, 5.0, 6.0);
	vec3add(a,b); 		//add b to a

	These 'functions' are defined with macros, so the compiler should be able to optimize freely.

	example functions:  vec3add, vec3sub, vec3scale, vec3cross, vec3dot, vec3abs_sq, vec3madd

	Optionally, vec3 can be padded out to 4 elements long, to deal with alignment or graphics libraries that insist on 4 dimensional coordinates.


graphics:
	Old opengl code I was working on.  This supports both old fixed function, and newer shaders.
	This is going to be reworked and added back in.

sound:
	Old experimental code I wrote playing around with OpenAL.  Will integrate back into here

