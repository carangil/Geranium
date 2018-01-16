


typedef struct zstring_s
{
	zarray_def(char, chars);
} zstring;

#define ZSTRING_NULL  { NULL}

//init blank string
void zstring_init(zstring* s);

//set string to be copy of a cstring
zbool zstring_set(zstring* s, char* cs);

//allocate string to hold len chars (allocates this + terminator)
zbool zstring_setspace(zstring* s, int len);

//return length of zstring in bytes (not including terminator)
int zstring_len(zstring* s);

//append a c string to the end of a zstring
int zstring_catc(zstring* s, char* cs, int start, int len_copy);

//append zstring to the end of a zstring
int zstring_catz(zstring* s, zstring* s2, int start, int len_copy);

//add a character to the end of a string
int zstring_catchar(zstring* s, char c);


zbool zstring_valid(zstring* s);

char* zstring_detach(zstring* s);

//cleanup stotage associated with zstring
zbool zstring_delete(zstring* s);

//puts result of sprintf into a new zstring
//usage:
// zstring dest;
// zstring_printf( &dest, "format", ...);

#define zstring_printf(dest,  ...)    zstring_setspace(dest, snprintf(NULL, 0,  __VA_ARGS__)) , snprintf( (dest)->chars, 1+zstring_len(dest) ,  __VA_ARGS__)



