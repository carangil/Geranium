typedef struct zstring_sh
{
	zuint32 len;
	zuint32 capacity;
} zstring_shadow_t;



char* zstrndup(char* str, int count);

zvec_t*  zsplit(zvec_t* v, char* str, char delim);

void zstr_debug(char* x) ;

char* zstr_mk(int capacity, int use);


//only works if dest is a zstring with shadow buffer. src can be anything
char* zstr_cat(char* dest, char* src);


#define ZSTRING_ALL -1

