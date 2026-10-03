# Geranium

## Introduction
Geranium is a stack-based programming language

### Features

- Postfix (RPN) primary syntax:  (such as 2 3 +)
- Strongly typed
- Procedure overloading
- libffi for calling C functions


### Experimental Bindings

- OpenGL and GLFW
- fluidsynth (MIDI synthesizer library)


## Intentional Design constraints and Limitations

- Variables are statically allocated, or are allocated temporaily and their lifetime is tied to a particular function.

- Strings and Arrays are the only dynamically allocated objects available to the user that can be returned from functions.

- The target systems are both CPU and GPU.  Given that the GPU has a more restricted computing model than the CPU, I am specifically targetting glsl vertex, fragment, and compute shaders.  Shaders are meant to read and write arrays: VBOs, SSBOs, are all array-type objects, and are easily passed to a compute or draw dispatch.  A tree of random heap pointers... is not traversable on the GPU.  However, a tree structure where nodes contained in a single array reference each other by index... that is supportable on the GPU, and is still fast on the CPU.  32-bit indices are also smaller and faster than 64-bit pointers.

- Externally sourced objects, like GLFW Window handles, etc are actually CPU heap pointers... but that's OK.  You could 'pass' these to a shader, but they wouldn't be useful with it... so everything that is sent to the GPU is going to be Plain Old Data.

- Typical data structures like Trees or Linked Lists cannot be implemented as a bunch of random heap objects in system memory.  Data stuctures such as these, must be implemented by allocating an array of structs, and using array indices instead of pointers.  (There are plans to make alloc/free of these individual array elements automatic.)  Why have such a limitiation? On the CPU it is acceptable to have lots of random pointers to locations in system memory, that kind of access pattern does not work well on GPUs.  It's also not great for the CPU's cache either.  Geranium takes a different approach: Structs must be kept in arrays.  This simplifies a few things:

- Garbage collection:  When a complex structure is discarded, it can all go at once, not worrying about internal references between nodes.
- Cache effeciency:  Related objects are near each other.
- GPU friendly: A shader can traverse array indices as if they are pointers.
- Fast copy: If a structure needs to get to/from the GPU, its a copy of a single block of memory, rather than individual, small items.
- Structure-of-arrays is a possible implementation mechanism, but needs indices anyway.  Item #3 of an array of structs, or Item #3 sourced from multiple arrays of values... still needs a common "3."

Of course, you can try to get around these limitations.  Structs can contain references to dynamic arrays, so you *could* create a traditional tree or linked list structure using arrays of 1, but such a structure would not be very effecient, and when shaders try to traverse such a structure, it would either need a lot of small buffers, or many items packed into a buffer, which is again... an array.



## Basic Stack Machine

If you haven't worked with a stack machine before, this is a quick introduction.  In a stack machine, values are evaluated by pushing them on to a stack, and each operation pops off some number if items from the stack, performs an operation, and pushes a result back onto the stack.  For instance, if you wanted to add two numbers, you would push two numbers onto the stack, then run the ADD instruction, which pops the top two numbers off, adds then, and pushes the result back on.  Expression parsing is trivial, because order of operations is entirely up to the programmer:

```
Evaluating 1+3:

instruction  | Stack
--------------------
push 1       | 1
push 3       | 1 3
add          | 4 

The Geranium code for the above would be: 1 3 +
```

```
Evaluating 4*(2+3)

instruction  | Stack
--------------------
push 4       | 4
push 2       | 4 2
push 3       | 4 2 3
add          | 4 5
mul          | 20

The Geranium code for the above would be:  4 2 3 + *
```


```
Evaluating (4*2)+3

instruction  | Stack
--------------------
push 4       | 4
push 2       | 4 2
mul          | 8
push 3       | 8 3
add          | 11


The Geranium code for the above would be:  4 2 * 3 +
```


## WORDS

A program is a sequence of words.  A 'word' is a string token in a program that is bound to some meaning.  There are many different kinds of words:

- Variable: The word represents a named variable
- Type: The word represents a data type
- Constant: Constants such as  1.0, 3, "Strings"
- Procedure: The word represents code that expects certain items on the stack... (a 'function')
- Parser Words:  Some words trigger behavior in the compiler

Words are in general seperated by space, which consists of carriage returns, linefeeds, tabs, and spaces.  (ASCII bytes 13, 10, 9, 32)

Numbers without decimal points (20) are considered Z32 (signed integer).  Numbers with decimal points (1.8) are considered Real.  A number must begin with a digit or a negative sign.  Numbers starting with '0x' are treated as hexadecimal integers, and accept A-F characters of any capitalization.  Numbers with decimal points may also contain an exponent in the form 12.0e10, 1.0E-3, 15.3e+13, etc, with any capitalization.

Some one and two character symbols are always matched as their own token, regardless of space.  ` ( ) [ ]  [] -> >= == : + - * / etc` )  Spaces are not necessary in those cases...  `10 3+` will produce 13.  Note that '[' and ']' are one character symbols, and '[]' is a two character symbol.  Note, this means '[' and ']' cannot appear next to each other without a space between them!

Words that are not matched as a symbol or number are assumed to be an alphanumeric token, which must start with:  ` . _ a-z A-Z _ ` Once started, they can also contain numbers.   `x123` is a valid alphanumeric token.

Symbols and alphanumeric tokens can be used as variable and procedure names.  For instance, `+` can be the name of a procedure; it would be recommended that usual mathematical operations on custom datatypes use names such as + and * instead of add and mul.


### Major differences from FORTH

- Types: In Forth, every item is a *cell*, which can either be an integer, pointer, floating point, etc.  Each value in Geranium has a specific type.
- Stack matching:  In Forth, every procedure has a unique name.  In Geranium, names can be re-used, because matching is perform both by name AND by stack types.  While parsing the program into a syntax tree, a stack of datatypes is kept, which is counterpart to the runtime evaluation stack.  When a word is encountered during parsing, it only accepts the word if the stack has those arguments on top... meaning the same name can be reused in different contexts.  '+' can mean integer or floating point addition, string concatenation, or anything else the user has defined, depending on the preceding context.  This is similar to StrongForth.  https://www.stephan-becher.de/strongforth/intro.htm


## Types and Variables

Geranium has a few built-in simple data types:

- Z32: 32-bit signed integer
- Real: A floating pointer number.  Currently this is 32-bit single precision.
- String: A NULL-terminated, reference-counted, immutable*, C-compatible string, with a metadata header 'behind' the string. (C functions can mutate Strings, because C is outside the enforcement of the type system.  In general, a lot of care must be taken when calling into C.)
- Byte: 8-bit unsigned integer

There are several more types, but this is just an introduction to get started.


### Simple Variables

The above data types are enough to show how to create a simple value variable:

```
Z32 var i
Z32 var j

10 i=		//assign 10 to i
20 j=		//assign 20 to j

i j + print	//add i and j, printing the result

j 1 + j=		//add 1 to j (j is now 21)
```

### Simple Procedures

With the basic data types, it is also enough to show how to create a simple procedure.  First we need to introduce simple procedure types:


```
(argument type:name  argument type:name ...  -> return type)
```


Both arguments and return types are optional.  The arrow (->) is only required if a type is being returned, and is the delimiter seperating arguments from return values.  Arguments also have names, return values do not.  Here are some example procedure types:


```
(Z32:a -> Z32)  //Notation for a procedure that takes a 32-bit signed integer parameter (named 'a'), and returns a 32-bit signed integer.

(Z32:a Z32:b-> Z32)  //Notation for a procedure that takes two 32-bit signed integer parameters (named 'a' and 'b'), and returns a 32-bit signed integer.

( -> Z32)  //Notation for a procedure that takes no parameters and returns a 32-bit signed integer.

(Z32:c) 	//Notation for a procedure that takes a 32-bit signed integer, but returns no value.

```


Here is a sample procedure that squares in integer:


```
(Z32:a -> Z32) proc Square 
	
	a a * return
	
end

```

In general, the form of a procedure is:
```

(procedure type) proc Name

	//code

end

```

The 'return' keyword, returns the top-of-stack value to whatever called the procedure.  Return has a few rules:

- The top-of-stack type has to be the same type that is declared.
- Any other stack values that have accumulated as the procedure has run, and the arguments themselves, are popped off the stack, leaving the stack how it was before the arguments were pushed on.
- If a procedure is not indicated to return a value, then 'return' simply cleans up the stack, returning it to the state it was before the arguments were pushed onto it.
- It is an error to get to the bottom of a procedure that returns a value, and NOT have a value to return.

Procedure arguments also have another rule of their own:  They are read-only.  It is an error to attempt to write to an argument value.

### Procedure/Operator Overloading

Geraniumn does not have 'operators' in the sense that a language like C does... it only has words.  When compiling, words are matched not just by name, but my the types currently on the stack:


```
(Z32:a -> Z32) proc Square 
	
	a a * return
	
end

(Real:a -> Real) proc Square 
	
	a a * return
	
end

1.1 square   //called (Real->Real) square
4 square		//called (Z32->Z32) square
"banana" square // error, there is no 'square' function accepting banana
```

When matching, the shortest match is taken, meaning if you have two functions of the same name, but take different amounts of the same arguments (one function's argument list is a prefix for the other's), then it becomes impossible to call that longer one.  *There are plans to make this a detectable error.*


### Type Suffix

A suffix on the end of a type name can change its meaning.

#### & : Reference
This means the value is a reference to that kind of value.  This is often implemented as a pointer, but can be implemented with copy in/copy out when giving a function a reference to a variable, or in a sequence of operations that 'cancels' the pointer (de-referencing a just-created pointer to a value), so in many cases a 'pointer' may not be created at all, and it was an 'imaginary' value that only existed transiently in the compiler's syntax tree.

```
Z32& : reference to an integer variable
```



Now that we have the reference suffix, we can do some more interesting things:

``` 
(Z32&:a) proc increment

	a 1 + a=

end
```

The above increment procedure appears to write to the argument.  Not exactly.  The argument is not the integer itself, but the integer *variable*.  Inside the increment procedure, that variable can be used normally, but it isn't reading or storing a local value, it is modifying the variable that was used to call this procedure:

```
(Z32&:a) proc Increment

	a 1 + a=

end

Z32 var x

10 x= //x is 10

x& Increment  //x is incremented to 11
x& Increment  //x is incremented to 12

```


When calling a procedure, the *&* suffix puts the *variable* on the stack instead of the *value*.  When Increment returns, any changes Increment made, still stand.  *Note: This is implementation-defined whether this is a pointer, or if the value was copied into the procedure as a local variable, and then copied back out afterwards.  In cases where the procedure does not write the value, it also may just be a regular copy.*


A procedure that receives a reference can even forward that reference to another function:
```

(Z32&:a) proc Increment

	a 1 + a=		//using 'a' by it self returns the value in a
	
end

(Z32&:a) proc IncrementTwice

	a& Increment	  //using a& gives a reference to the called function.
	a& Increment
	
end

10 a=
a& IncrementTwice
a print  //a is 12

```




#### Limitations of variable references:

References to variables can be put on the stack and passed as arguments to procedures, but cannot be returned from procedures:  Why?  Procedure local variables are, in many programming languages, implemented as a stack.  Sending a reference to a local variable to a procedure being called is not a problem, because the caller's variables are all guaranteed to be valid for as long as the callee is running.  By preventing return or storage of references, this makes this kind of pointer escape impossible.


```
Z32& var x	

(->Z32&) proc escape	 //procedure returns reference to Z32	

	Z32 var escaped
	
	10 escaped = //escape contains ten
	
	escaped& return  //CANNOT return reference, compiler error 

end

escape x = 
x print   //Crash!: escaped.  Fortunately this program will not compile.

```


References cannot be stored inside structures, or even variables.  You can't get around reference rules by tring to have a reference to a reference variable, because you can't have a reference variable in the first place.  If you could have a reference variable, you could attempt this:


```
Z32& var x		//compile error: variables cannot hold references

(Z32&&:a) proc escape		//references and even double references can exist as arguments

	Z32 var escaped
	
	10 escaped = //escape contains ten
	
	escaped& x=	//attempt to write a reference to variable x

end

x& escape //giving 'escape' a reference to x is OK

x print newline //Crash!  x would reference a variable that no longer exists.  Fortunately this program will not compile.
```


This makes references a bit limited.  A function can't even return a reference that it received as an argument, even though there may be cases where that would be safe to do so. 

#### $: Steward
This means the variable holding this value must keep track of this variable's memory.  If overwriting a steward variable with another value, or a procedure returns with non-null values in steward variables, the objects are freed.

Remember that you cannot return references, or have reference variables?  Stewards are the way around that.  A regular String is treated like a reference:

```
(String:s String:t) proc PrintStuff
	s print
	t print
end

"whatever" "boo" PrintStuff  //can pass strings 
```

But you cannot store a string in a variable, or return a string from a function, because a String internally is a reference to some sequence of characters stored *elsewhere*.

```
String var s			//error: s is a reference variable, which are forbidden
```

A procedure gets string arguments from its caller, so it can assume it's valid the entire call.  It can go on the stack and be passed down to deeper procedure calls, but cannot be stored in a variable, or returned, because outside the current function, the lifetime of that String is unknown.  For instance, the *print* procedure that accepts a String doesn't care where its stored, but it is also unable to resize, store, return or free that object.  It can however pass it down into helper functions, iterate on its characters, etc.


Instead of a `String`, a `String$` steward variable will keep track of the memory of the String.  If a steward variable goes out of scope, or gets overwritten, it is properly freed.  Unlike references, stewards can be returned, or stored in variables and structs.  If multiple variables steward the same String, the string is not freed until they are all freed.


```

(String:s String:t) proc PrintStuff
	s print
	t print
end

String$ var s  //String steward variable
String$ var t

"Testing"$ s=  //Write a copy of the string 'Testing' to s
"Stuff"$ t=  //Write a copy of the string 'Stuff' to s. 
s t PrintStuff  // using 's' and 't' like this 'demotes' the steward to a normal reference:  PrintStuff sees a limited reference for s and t.... PrintStuff cannot free these objects, store them in variables, or return them.

```

Note the string constant is also suffixed with a '$'.  A $ on an string constant makes a copy of that constant, and the lifetime of that object is tracked.  It can be passed to a procedure that accepts a String$, or it can be stored in a variable.

```

//Note:  accepting String $instead of String
(String$:s String$:t ->String$) proc First

	s return // ERROR: using 's' demotes it to a normal string reference, which can't be returned

	s$ return  //OK:  using s$ returns using a steward.  

end

String$ var s
String$ var t
String$ var u

"Testing"$ s=  //Write a copy of the string 'Testing' to s
"Stuff"$ t=  //Write a copy of the string 'Stuff' to s. 

s$ t$ First  //Error because the String$ returned by First isn't put anywhere, so this would leak memory

s$ t$ First u=  //OK, this assigns to variable u

s$ t$ First trash  //OK.  The trash keyword frees the object

u=	//note the returned string steward is stored in u

```

When the above is completed, t contains a single reference to 'Stuff', and both s and u have a reference to 'Testing'.  The string 'Testing' won't be freed until BOTH s and t are overwritten or out of scope.  Internally, stewards perform reference counting on the objects they track.




### Variable Shortcut

The colon `:` assigns the top of stack value to a new variable of that name and type.

```
10 3 + :x   //x is a Z32 variable containing 13
```

The double-colon `::`, assigns to a new variable, but does not remove it from the stack:

```
10:x 20:y + print  //prints 30
x print  //x still has 10
y print  //y still has 20
```


### Arrays

An array type is defined by placing a data type in square brackets.
`[Z32]` is an array of integers.

Arrays are similar to Strings, in that they are references.  They can be passed to procedures.  Here is a procedure that accept an array:

```
([Z32]:a) proc print

	"This array contains " print
	a .count print
	" elements\n" print

end

```


How to create an array?  First there needs to be a place to put it:

```
[Z32]$ var X		//X is an integer array steward
```

Of course, the place it goes has a steward on it.  The above only declares a place to hold it, but did not create the array itself. `dim` is the word that allocates an array.  It takes a reference to the steward, a count, and a size:

```
X& 0 10 dim		//Allocates an array that holds 10 integers.
```

The above creates an empty array with space to hold 10 items.

```
X& 10 10 dim		//Allocates an array that holds 10 integers.
```

The above creates a full array of 10 integers.  The values are initialized to zero.

The difference between count and size is subtle.  Size is the number of items the array can hold before it is full and needs to be resized.  Count is the number of items in the array.  The array has an internal item counter, which is useful when appending an array.  `++` is the append operator, and it increments the count of an array, and returns a reference to that element, which can be assigned to:

```
	X& 0 10 dim  //10 spaces
	
	5 X++ =   
	7 X++ =   
	8 X++ =	 

```

`[]` is the array indexing word.  It accepts an array reference and an integer, and returns that element of the array.  If the next word is the reference symbol `&`, a reference to that slot is returned instead.  Similarly, '=' will perform a store on that slot:

```
X 0[] 	//get the 0th element (an integer)
X 2[]&	//get a reference to slot 2
7 X 0[]=			//assign 7 to slot 0

```

Note arrays are 0-indexed.

### Array words

There are some special words for arrays:

```
	X .count   //.count returns the count of the array
	X .size	   //.size returns the capacity of the array (in elements, not bytes)
	X 3 .setcount  //Sets the count of an array to a value, in this case 3.  This is useful for 'resetting' an array.  Count MUST be less then or equal to size
	
	
	X& 3 20 dim  //You can dim an array again to change its size and count.  The array is NOT cleared, but it may have moved in memory.  It is a runtime error to dim and array that has more than one steward.

```


### Frames: custom structures

A structure type can be created from other base types, and made into a variable or an array.  Internally, these are referred to as Frames.


```
type Pair
	String$ var s
	String$ var t
end

Pair var P		//A single Pair

5 P.x =
6 P.y =
```

The 'dot operator' may look familiar from C and Java-like languages.  It is not an operator.  Let's breakdown a line:

```
5		//5 on stack	
P		//puts reference to the Pair on the stack
.x		//access the 'x' variable of the Pair that's on the stack
=		//Assign the 5 to that variable.

```
Dots can be chained:

```
type Boo
	Pair var p1
	Pair var p2
end

Boo var myBoo

10 myBoo.p1.x= //assign 10 to myBoo's p1's x variable
20 myBoo.p1.y= //assign 10 to myBoo's p1's x variable
```

#### Frame Assignment Shortcut

There is a shortcut available to assign many values to a structure.

`=[` accepts a reference to a structure type.  The colon `:` variable creation shortcut is temporarily redefined to assign the the value in the struct.  This behavior persists until a matching `]`.


```

myBoo.p1 =[    
	10:x			
	20:y
	30:z //error myBoo.p1 does not have a 'z' field
]

```


If a frame contains other frames, the behavior of the colon simply puts that subframe's reference on the stack.  This allows these assignments to be nested:

```

myBoo =[

	:p1 =[
		10:x
		20:y
	]
	
	:p2 =[
		40:x
		//no store to y; y is whatever it was before
	]

]

Note that `=[ ]` performs bulk assignment NOT initialization.  Variables not explicitly named are NOT set to zero.  If this is a newly created variable or an element of a newly created array, it *was* previously initialized to zero at creation.

```


#### Arrays of frames

You can also have arrays of a frame type:

```
[Pair]$ var Ps		

Ps& 1 10 dim	  //array that can hold 10 Pairs, but has 1 in use
55 Ps 0[].x=
66 Ps 0[].y=

```


Lets break one down:
```
55 	//55 on stack
Ps	//Array reference on stack
0 [] //Reference the 0th element
.y   //Reference the 'f' variable of that element
=	//Store the 5.
```



Array incrementing and the frame assignment shortcut work well together:

```

[Pair]$ var Ps		

Ps& 0 10 dim	  //array that can hold 10 Pairs

Ps ++ =[
	55:x
	66:y
]

Ps ++ =[
	123:x
	//even though 'y' is not referenced here, because '++' was performed on an empty array, this element had already been initialized to zero.
]




```


### The following parts of the documentation is very rough, incomplete, and are about features that are works in progress.

## FFI (Foreign Function Interface)

C functions from dynamic libraries (.so files) can be loaded and called by Geranium programs using libffi.  Not all types are supported, but major things like ints, pointers, etc are supported, and support is slowly growing with time.  Currently the main limitation is you can't pass or return a C struct through libffi.  (You CAN pass and return pointers to C structs.  Many APIs can be supported through this, such as OpenGL.)

You can also make a C callback for a Geranium function using libffi's closure API.

## Generics and *like* parameters

A limited form of generics are available.  They are currently mostly applied to built-in functions.  The `Any` type will match anything.  `Any` can also be suffixed or wrapped with other types.  For instance, while `Any` will match anything, `[Any]` will match only arrays (of any type), `[[Any]$]$` will match arrays of arrays of any type, etc.

Consider the generic type for storing a value:

`(Any:a  Any&:p)` 

This would allow you to store any value 'a' into any reference 'p'.  This is not good.  We want to limit what can be stored there:

`(Any:a  like a&:p)` 

In the above, 'a' can be anything, but 'p' has to be a reference to 'like a'.  In the case of 'a' being Z32, 'p' must be Z32&.

Array indexing also has a similar internal definition:

`([Any]:arr Z32:index -> like arr@&)`

arr can be any array.
The return value, like arr@& gets broken down into:

- **like arr**:  The same type of arr. So if arr is [Z32], like arr is [Z32]
- **@**	: This removes a layer of brackets or a suffix.  So [Z32]@ is Z32
- **&** : Adds a reference.

So the above definition, indexing into a [Real] array will return a Real&, which is a reference to a Real... exactly what you want when indexing an array.  That reference can then either be read as Real number, or stored to.

Reading is also done with a similar definition:

`(p:Any& -> like p@)`

p is a reference to anything.  The return value is like p, but with the reference removed from the type.  Reading a reference to any type returns a value of that type.


## Introspection

This is where Geranium starts to get more like LISP than FORTH.  Any Geranium WORD can be viewed at runtime as a syntax tree.  You can get variable names, types, complete expression trees, everything needed to compile a function.  Why?  Because the current strategy being developed in Geranium to deal with shaders... is to write the shaders in Geranium, and then having a Geranium program pick the compiled shader's tree apart and generate glsl.  This is not limited to glsl, this potentially allows JIT into any system.

Unlike LISP, the trees are not directly executable.  Before a tree can run, it needs to be compiled.  Currently there is a bytecode interpreter written in C, and a C implementation of a tree-to-bytecode compiler.  The goal is to perform a glsl compute dispach of a Geranium function across an entire array of values in parallel.  Basically, every Geranium function gets a bytecode implementation.  As needed, a glsl implementation can be generated and cached for functions that are dispatched as compute shaders.  Same for vertex and fragment shaders.





# Old Readme: This will be edited and 'folded' in the real readme eventually.


## Zcore

This project grew out of an other old project of mine.  Over the years I've spent some time playing around with things in C.  Somethings, like memory management, lists, vectors, etc get used over and over, so I made them into a library.  It's called Zcore, because its the central core framework I was building Project Z on.  Z was a simple opengl-based rendering and graphics asset loading library that I occasionally add some work to.  Its unofficial goal was to build a simple game engine.  I started working on an interpreted langauge, and I've recently done a complete rewrite of the interpreter, changed a bunch of things in the language, and gave it a new name: Geranium

Platform:  Currently Linux, but I have developed parts of this in the past in Windows, and am confident it can be fairly easy to port it back to Windows.

## Components:

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
			An array implementation compatible with C arrays, but uses zmemory's shadow buffers to track array size.
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
	Provides an easy interface to create and join threads, simple locking and atomic inc/dec operations.  This is not currently used, but will be brought back in.

misc:
	Random numbers, reading microseconds off the clock, etc.  When any functionality gets large enough, it will be split off to its own module.
	Also has simple bitmap functionality which needs to eventually be split off, and a simple event system.

zwindow/zevent:  This is deprecated, but parts of it will come back:  Provides a simple event queue to handle keypresses, mouse movement, etc.  Zwindow is a simple interface and there are currently two implementations:
	
*gfx_pixeltoaster*:  Uses the pixeltoaster library to create a window, display bitmaps, handle keyboard input, etc.  The input is handled with the zevent queue.

*gfx_gl*: Uses glfw and glew to create an opengl window.  Uses zevent for keyboard/mouse interaction.  Note, since both use zevent, and the zbitmaps gfx_pixeltoaster accepts to draw, and zbitmaps gfx_gl uses for textures are all the same, its easy to write code generic to both.

vmath:
	A simple implementation for 3D vector math.  The functions are intended to be used in an assembly-like manner:
	vec3set(a, 1.0, 2.0, 3.0);
	vec3set(b, 4.0, 5.0, 6.0);
	vec3add(a,b); 		//add b to a

	These 'functions' are defined with macros, so the compiler should be able to optimize freely.

	example functions:  vec3add, vec3sub, vec3scale, vec3cross, vec3dot, vec3abs_sq, vec3madd

	Optionally, vec3 can be padded out to 4 elements long, to deal with alignment or graphics libraries that insist on 4 dimensional coordinates.



sound:
	Old experimental code I wrote playing around with OpenAL.  Will integrate back into here.  Not functional.


Credits:
	All code is my own, with the following exceptions:

		GLEW	https://glew.sourceforge.net/
		GLFW	https://www.glfw.org
		YXML	https://dev.yorhel.nl/yxml
		PixelToaster  Original site defunct. Found it here https://github.com/thedmd/pixeltoaster  My fork: https://github.com/carangil/pixeltoaster

---ZCORE---

Copyright 2023 Mark W. Sherman <m@mwsherman.com>

Released under the terms of CC BY-NC.  See COPYING file for full license text.

Summary here:

	Free to copy and redistribute
	Free to alter, adapt or change
	Must give attribution
	Must not use for commercial purposes.


---GLEW--- (not currently used, BUT is still in the tree history)

The OpenGL Extension Wrangler Library
Copyright (C) 2008-2016, Nigel Stewart <nigels[]users sourceforge net>
Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>
Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>
Copyright (C) 2002, Lev Povalahev
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* The name of the author may be used to endorse or promote products 
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.

---GLFW---


    Copyright © 2002-2006 Marcus Geelnard

    Copyright © 2006-2019 Camilla Löwy

    This software is provided ‘as-is’, without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

        The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.

        Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.

        This notice may not be removed or altered from any source distribution.



---PixelToaster ---

    PixelToaster Framebuffer Library.

    Copyright © 2004-2007 Glenn Fiedler

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.

    Glenn Fiedler
    gaffer@gaffer.org

---YXML---

Copyright (c) 2013-2014 Yoran Heling

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.