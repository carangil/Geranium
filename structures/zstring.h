//ZString
//(C)2021-2022 Mars Sherman
//This library provides a string implementation that is compatible with C strings
//This depends on zarray, which is a resizable array implementation compatible with C arrays

//Pass ZSTRING_ALL when ALL of a string should be used (instead of just n bytes)
#define ZSTRING_ALL -1

//Copy C or zstring into a new zstring
char* zstrndup(char* str, int count);
#define zstrdup(SSS)  zstrndup(SSS,ZSTRING_ALL)

//Split C or zstring on delimiter, returning vector of zstrings
zvecT* zstrsplit(zvecT* v, char* str, char delim);

//Combine vector of C or zstrings to a new zstring.  Optional delimiter inserted
char*  zstrbuild(zvecT* v, char delim);

//Show information about a C or zstring
void zstr_debug(char* x) ;

//Make an empty zstring with space for 'capacity' bytes.  Null terminator automatically added to length.
char* zstr_mk(int capacity);

//Append substring of a C or zstring to the end of a zstring.
//dest cannot be a C string or have more than 1 reference
//dest may be resized, so the return value may be different:
//dest = strcatsub(dest, otherstring,1,10);
char* zstrcatsub(char* dest, char* src, int start, int count);


//abbreviated version of zstrcatsub that takes the whole src string
#define zstrcat(XDEST,XSRC) zstrcatsub(XDEST,XSRC,0,ZSTRING_ALL)


