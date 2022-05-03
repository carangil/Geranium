/*typedef struct zstring_sh
{
	zuint32 len;
	zuint32 capacity;
} zstring_shadow_t;
*/


char* zstrndup(char* str, int count);

zvecT*  zsplit(zvecT* v, char* str, char delim);

void zstr_debug(char* x) ;

char* zstr_mk(int capacity);

//only works if dest is a zstring with shadow buffer. src can be anything
char* zstrcatsub(char* dest, char* src, int start, int count);

#define ZSTRING_ALL -1

#define zstrdup(STR) zstrndup(STR, ZSTRING_ALL)

