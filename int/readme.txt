Zlang is a statically typed concatenative postfix programming language.

The input is parsed into tokens.  Tokens are separated by whitespace, and some sequences of characters are recognized as tokens regardless of whitespace.

Tokens:

_Alphanumeric123    Tokens may start with underscore or alphabetic characters, and continue with numbers
._Alphanumeric123   Prefixing some tokens with a . changes the meaning.
1234                Integers
-1234               Signed integers
0x123abc            Hex Integers
0.123  -0.123e2      Floating point values.  Cannot start with '.'
[] == != <= >= **   2-char tokens; recognized regardless of whitespace
+-/*=#(:)%&         Single character tokens, recognized regardless of whitespace, but only if it cannot be recognized as part of a 2-char token.
"String literal"    String literals can contain spaces


Imaginary Stack

Data is manipulated using an imaginary stack, where items consist of both a value and a type.  The type information mostly only exists at compile time; at runtime only values are present, with some minimal type information for some virtual types.  

    Example                     Output          Explanation

    10 print                    10            10 is put in the stack.  Print takes the 10 and displays it.
    10 5 + print                15            10 is put on stack. 5 is put on stack. '+' adds the two values.  Print displays it.
    5 #x  x x * print            25           5 is put on the stack.  #x creates a new variable, assigns the 5 to it.  Each x puts a 5 back on the stack.  * multiplies the two 5's, and print displays it.
    



As you can see, the syntax is very FORTH-like.  However there are some important differences:

    - Using the name of a variable does not put its address on the stack; instead it loads the value.
    - It is convenient to create named variables for items on the stack using the '#' token.
    - Stack manipulation keywords like over, pick, etc are not provided or are required

Writing to an existing variable is done by immediately following the variable name with '='.
    10 x =   //Writes 10 to x

    
Calling procedures:

    A new procedure can be created with the proc keyword:

    proc square(x:Z32 -> Z32)       //square takes an integer(Z32) x.  And returns a Z32.
        x x * return
    end

    Procedures can be overloaded:

    proc square(x:Real -> Real)     //This version takes floating point real numbers
        x x * return
    end

    0.5 square print        //will print 0.25
    5 square print          //will print 25


    A procedure can return nothing, or a single value.

    
The '#' is a handy way to create variables if you already have a value.  A variable can also be created with 'var'.    
    There are two scopes: Global and Local.  Variables created in functions are accessible only in that function and are destroyed when the function exits... they are similar to stack variables in C. Variables created inside if/else statements and loops are assessible even after the loop or true/false clause exists... there is no 'inner' scope.

    var x:Z32;  //creates variable x.  It is initialized to zero.
    
    
Casting

    Sometimes it is necessary to convert data from one form to another.  :Type can perform the cast:

    2 #x
    5 #y
    x y / print     //this prints 0  (2/5 is 0 in integer division)
    x:Real y:Real / print   //this prints 0.4, because both x and y have been cast to Real numbers.

Comparisons

    1 #x

    x 1 == if                       //The '==' operator returns a Bit value: true or false.
        "X is 1" print newline
    x 2 == elseif
        "X is 2" print newline
        X 2 == print newline  //prints true
        X 3 == print newline //prints false
    x 3 == elseif                       //note:  Indentation doesn't matter.  This 'X 3 ==' is NOT part of the above 2== case because of the elseif.
        "X is 3" print newline
    else
        "X is something else" print newline
    end

    'Dangling else' is resolved by forcing the using elseif keyword instead of 'if else' as in many other languages.

    If's can be nested:
    
    x 1 == if
        y 2 == if 
            "x=1 and y=2" print
        else
            "x=1 and y is something else" print
        end
    else
        "x is something else" print newline
    end


Loops:

    The loop keyword creates an endless loop:

    0 #i
    loop

        i print newline
        i 1+ i=

    end
    

    The break keyword will end a loop:

     0 #i
    loop

        i 10 >= if
            break
        end

        i print newline
        i 1+ i=

    end


    The keywords 'until' and 'while' are shorthand for the if/break statement.
    Until and while can appear multiple times inside a loop and at any position in the loop.

    loop

        i 10 < while        //if the boolean statement is false, the loop exits

       

        i print newline

        i 5 == until        //if the boolean statement is true, the loop exits

        i 1+ i=
    end


    The 'continue' keyword jumps back to the top of the loop, as in C


    Custom data types:

    The 'type' keyword creates a structure datatype:

    type vec2
        x:Real;
        y:Real;
    end

    Var can create an instance of a structure:
    
    var myVector:vec2;
    
    1.0 myVector .x =   //assign 1 to x field of myVector.  A '.' in front of a token searches the top of stack item for a field of that name.
    2.0 myVector.y =     //The space isn't really necessary.  Object.field.subfield... works well in a stack based language AND is familiar to users of other languages
    
    
    Structures themselves cannot be put on the evaluation stack:
    
    myVector    //doesn't put the vec2 on the stack.  Instead it puts a vec2&    & means reference
    
    myVector    // vec2& on the stack
    myVector .y //the .x offsets the reference to find field y, and retrieves the value.  In this case, a Z32 is put on the stack.
    
    
Nesting Structures
        
        A struct may contain another struct:
        
        type matrix2
            topRow:vec2;
            bottomRow:vec2;
        end
    
        
        var m:matrix2;  //make a matrix2 variable.
        
        m       //puts a matrix2& on the stack
        m.bottomRow.x       //reads the x field from the bottomrow field of m
        
        10 m.bottomRow.x =       //writes 10 to the x field of bottomrow field of m
                
        m.bottomRow //puts a reference to bottomRow on the stack (a vec2)
    
Passing structures to procedures:

    Structures are past to procedures by using references
    
    proc print:(v:vec2&->)
        "(" print v.x print "," print v.y print ")" print
    end
    
    m.topRow print

    
Dynamically allocating memory:

    The new keyword creates an item and returns a reference to it:
    
    vec2 new #myVec     //myVec contains a reference to the newly created vec2
    
    The reference is different from a regular vec2&.  It has a special name... vec2%.  The '%' means possessive reference.
    possessive references have specific rules to prevent memory leaks and corruption
    
    Posessive refences follow these rules:
    
        The only kind of references that can be returned by a proc are possessive references.  This means you cannot return a reference to subparts of an object or local objects.  (you may think you can return regular references indirectly through another pointer, but this is prevented in another way)
        
        Reading a possessive reference variable puts a regular reference on the stack:
            
            vec 2 new #myVec
            
            10 myVec.x=
            20 myVec.y=
            myVec print //print still receives a normal vec2&
        
        unless the keywords 'take' or 'keep' are used:
        
        myVec take      // a vec2% is put on the stack.  BUT myVec now contains NULL.  We took the reference.
        myVec keep      // a vec2% is put on the stack.  myVec still contains a refernce.  The reference count of the object has been increased by 1.
        
        Assigning a possessive reference to a variable that already contains another possessive reference will call free on the object.
        
        vec2 new # myVec //create a vec2
        vec2 new myVec= //create a new vec2.  Writing to myVec will free the previous reference.
        
        AND of course, 'free' means decrement reference count.  If there was a copy kept elsewhere, the object is still valid other there.. just the local pointer to it is now gone.
               
                
        Possessive references passed to procs  are 'gone.'  What that means is the caller can proceed not caring if the taken or duplicate made by keep is freed... It is now the resposibility of the callee.
        
        Possessive references received by procs are the procs responsibility.  They must be assigned or trashed.
        
        myVec take trash    //the trash keyword will free any possessive pointer passed to it in this example, myVec was set to null with take, and the pointer taken was then trashed.
        
        myVec keep trash    // A duplicate reference to myVec is made with keep... and then immediatly trashed.
        
        When a function goes out of scope,  all possessive reference variables that are not null, are freed.
        When a struct or awway containing possessive pointers is freed, those pointers are recursivly freed.
        Using these rules around possessive pointers, you cannot free too many times. Dangling pointers become a non-issue.  
        Memory leaks with tree-like structures are also avoidable.  Reference cycles can be a problem.  Weak references is a TODO item. Currently breaking reference cycles is the only manual memory management that has to be done.  If the cycles are broken, all remaining non-cyclical references will eventually be freed.
    
    Regular references follow these rules:
    
        A possessive reference can be passed to a function as a regular reference.  The function recieving the regular reference is guaranteed the reference is valid for the entire duration of the call, since the caller holds a valid counted reference.
        
        A regular reference to a local variable, one that was passed in from a calling function can be passed down to other functions.
        
        A regular reference cannot be returned from a function.
        
        A regular reference to a local variable cannot be stored in memory via a:   
            possessive reference, because could cause a local variable to escape past the time it is valid
        
        
        
        
        
        
    
    
