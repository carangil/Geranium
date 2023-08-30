Zlang is a statically typed concatenative postfix programming language.

The input is parsed into tokens.  Tokens are separated by whitespace, and some sequences of characters are recognized as tokens regardless of whitespace.

Tokens:

_Alphanumeric123    Tokens may start with underscore of alphabetic characters, and continue with numbers
'_Alphanumeric123   Some reserved words are prefixed with a '
._Alphanumeric123   Prefixing some tokens with a . changes the meaning.
1234                Integers
-1234               Signed integers
0x123abc            Hex Integers
.123  -0.123e2      Floating point values
[] == != <= >= **   2-char tokens; recognized regardless of whitespace
+-/*=#(:)%&         Single character tokens, recognized regardless of whitespace, but only if it cannot be recognized as part of a 2-char token.
"String literal"    String literals can contain spaces

Imaginary Stack

Data is manipulated using an imaginary stack, where items consist of both a value and a type.  The type information mostly only exists at compile time; at runtime only values are present, with some minimal type information for some virtual types.

    Example                     Output          Explanation

    10 print                    10            10 is put in the stack.  Print takes the 10 and displays it.
    10 5 + print                15            10 is put on stack. 5 is put on stack. '+' adds the two values.  Print displays it.
    5 #x  x x * print            25           5 is put on the stack.  #x creates a new variable, assigns the 5 to it.  Each x puts a 5 on the stack.  * multiplies the two 5's, and print displays it.



As you can see, the syntax is very FORTH-like.  However there are some important differences:

    - Using the name of a variable does not put its address on the stack; instead it loads the value.
    - It is convenient to create named variables for items on the stack using the '#' token.
    - Stack manipulation keywords like over, pick, etc are put provided nor required.

Writing to an existing variable is done by appending a '=' to the variable name:
    10 x=   //Writes 10 to x

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
    x 3 == elseif
        "X is 3" print newline
    else
        "X is something else" print newline
    end

    'Dangling else' is resolved by forcing the using elseif keyword.

    If's can be neste:
    
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


    The 'continue' keyword jumps back to the top of the loop


    Custom data types:

    The 'type' keyword creates a structure datatype:

    type vec2
        x:Real;
        y:Real;
    end





proc +:(a:vec2&;b:vec2&
