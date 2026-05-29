//subset of stdlib.h

typedef void* anyArray;

void qsort(anyArray base, size_t nmemb, size_t size, int ( *compar )( const void *, const void * ) );


char * getenv( const char * name );
