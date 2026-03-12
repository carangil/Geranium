//ZString
//(C)2021-2022 Mars Sherman
//This library provides a string implementation that is compatible with C strings
//This depends on zarray, which is a resizable array implementation compatible with C arrays
#include "zvector.h"

//Pass ZSTRING_ALL when ALL of a string should be used (instead of just n bytes)
#define ZSTRING_ALL ((size_t)-1)

//size when making empty strings
//overridable
#ifndef ZSTRING_INITSIZE
	#define ZSTRING_INITSIZE 32
#endif

//Copy C or zstring into a new zstring
char* zstrndup(char* a, zsize n);
#define zstrdup(SSS)  zstrndup(SSS,ZSTRING_ALL)

char* zstrtrim(char* s); //removes leading and following whitespace (according to isspace(char))

//Split C or zstring on delimiter, returning vector of zstrings
zvecT* zstrsplit(zvecT* initial, char* str, char delim);

//Combine vector of C or zstrings to a new zstring.  Optional delimiter inserted
char*  zstrbuild(zvecT* v, char delim);

//Show information about a C or zstring
void zstr_debug(char* x, char* label) ;

void zstr_reset(char* s);

//Make an empty zstring with space for 'capacity' bytes.  Null terminator automatically added to length.
char* zstr_mk(zsize capacity);




//Append substring of a C or zstring to the end of a zstring.
//dest must be null or a zsting.
//if dest has more than 1 reference, a copy is returned
//dest may be resized, so the return value may be different (a copy)
//anything returned by zstrcatsub has only 1 reference
//dest = strcatsub(dest, otherstring,1,10);
char* zstrcatsub(char* dest, char* src, zsize start, zsize count);

//abbreviated version of zstrcatsub that takes the whole src string
#define zstrcat(XDEST,XSRC) zstrcatsub(XDEST,XSRC,0,ZSTRING_ALL)


//creates a new zstring from 2 C strings together
char* zstrdup2(char* left, char* right);

char* zstrprintf(char* initial, char* format, ...);

zuint32 zstr_hash(char* s);





/*
 *
ZSTRNDUP

  zstring* zstrndup(char* src, int n);

  Copies a string (C or Z).   (So it always does the slow strnlen)
  Copies only first n bytes (-1 means copy all)  (+1 byte for terminator)
  If src is NULL, this allocates a new string for n bytes (+1 for terminator)
  If src is shorter than n, then src is null-terminated in a buffer large enough to hold n bytes (+1 for terminator)

  zstrndup always returns:
   null-terminated
   single reference
   the size specified


ZSTRNCATSUB

  zstring* zstrncatsub(zstring* dest, char* src, int start, int count);

  Appends src to dest.  Only copies bytes  (start) to (start+count).  This copies (or creates if somehow missing) the terminator
  -1 for count goes to the end of the src string
  If dest has more than 1 reference, a copy is made instead of mutating dest.
  If dest is too small, it is grown 2x, or to the size needed, whichever is larger.  Old dest is freed if growing and dest was the only reference.
  If dest is null, a copy of src is returned.

  zstrncatsub always returns:
    null terminated
    single reference

ZSTRPRINTF

  zstring* zstrprintf(zstring* dest, char* format, ...);

  Supports anything vsprintf supports.
  If dest is null, this allocates a new string with the sprintf result.
  If dest is not null, this sprintf appends to the dest, growing needed.

 NOTE: this one currently does not respect the single-reference thing yet.

    */
