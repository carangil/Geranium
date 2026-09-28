Zcore - Mars Sherman
--------------------

Why?

Over the years I've spent some time playing around with things in C.  Somethings, like memory management, lists, vectors, etc get used over and over, so I made them into a library.  It's called Zcore, because its the central core framework I was building Project Z on.  Z was a simple opengl-based rendering and graphics asset loading library that I occasionally add some work to.  Its unofficial goal was to build a simple game engine.  I'm about to refactor the graphics part, but figured I could seperate out some generic functionality and use that in my other C projects.


Platform:  Linux and Windows

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
	Provides an easy interface to create and join threads, simple locking and atomic inc/dec operations.

misc:
	Random numbers, reading microseconds off the clock, etc.  When any functionality gets large enough, it will be split off to its own module.
	Also has simple bitmap functionality which needs to eventually be split off, and a simple event system.

zwindow/zevent:
	Current part of 'misc'.  Provides a simple event queue to handle keypresses, mouse movement, etc.  Zwindow is a simple interface and there are currently two implementations:
	
		gfx_pixeltoaster:  Uses the pixeltoaster library to create a window, display bitmaps, handle keyboard input, etc.  The input is handled with the zevent queue.
		gfx_gl: Uses glfw and glew to create an opengl window.  Uses zevent for keyboard/mouse interaction.  Note, since both use zevent, and the zbitmaps gfx_pixeltoaster accepts to draw, and zbitmaps gfx_gl uses for textures are all the same, its easy to write code generic to both.

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


---GLEW---

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