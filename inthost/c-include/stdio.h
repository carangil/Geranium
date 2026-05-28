//stdio subset for file i/o

//Access
FILE * fopen( const char *  filename, const char *  mode );
int fflush( FILE * stream );
int fclose( FILE * stream );

//Read
int fgetc( FILE * stream );
char * fgets( char *  s, int n, FILE *  stream );
size_t fread( void *  ptr, size_t size, size_t nmemb, FILE *  stream );

//Write
int fputc( int c, FILE * stream );
int fputs( const char *  s, FILE *  stream );
size_t fwrite( const void *  ptr, size_t size, size_t nmemb, FILE *  stream );

//Seek
int fseek( FILE * stream, long int offset, int whence );
long int ftell( FILE * stream );
void rewind( FILE * stream );

//Error
int feof( FILE * stream );
void clearerr( FILE * stream );
int ferror( FILE * stream );

