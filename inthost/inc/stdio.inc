section stdio

	Z32   "SEEK_END" sys SEEK_END
	Z32   "SEEK_SET" sys SEEK_SET
	Z32   "SEEK_CUR" sys SEEK_CUR

	type FILE end
	// FILE * fopen (const char * filename,const char * mode)

	 ( String:filename String:mode  -> FILE* ) "_LIBC" "fopen" sys fopen 

	// int fflush (FILE * stream)

	 ( FILE*:stream  -> Z32 ) "_LIBC" "fflush" sys fflush 

	// int fclose (FILE * stream)

	 ( FILE*:stream  -> Z32 ) "_LIBC" "fclose" sys fclose 

	// int fgetc (FILE * stream)

	 ( FILE*:stream  -> Z32 ) "_LIBC" "fgetc" sys fgetc 

	// char * fgets (char * s,int n,FILE * stream)

	 ( String:s Z32:n FILE*:stream  -> String ) "_LIBC" "fgets" sys fgets 

	// size_t fread (void * ptr,size_t size,size_t nmemb,FILE * stream)

	 ( Any&:ptr Size32:size Size32:nmemb FILE*:stream  -> Size32 ) "_LIBC" "fread" sys fread 

	// int fputc (int c,FILE * stream)

	 ( Z32:c FILE*:stream  -> Z32 ) "_LIBC" "fputc" sys fputc 

	// int fputs (const char * s,FILE * stream)

	 ( String:s FILE*:stream  -> Z32 ) "_LIBC" "fputs" sys fputs 

	// size_t fwrite (const void * ptr,size_t size,size_t nmemb,FILE * stream)

	 ( Any&:ptr Size32:size Size32:nmemb FILE*:stream  -> Size32 ) "_LIBC" "fwrite" sys fwrite 

	// int fseek (FILE * stream,long int offset,int whence)

	 ( FILE*:stream Long32:offset Z32:whence  -> Z32 ) "_LIBC" "fseek" sys fseek 

	// long int ftell (FILE * stream)

	 ( FILE*:stream  -> Long32 ) "_LIBC" "ftell" sys ftell 

	// void rewind (FILE * stream)

	 ( FILE*:stream ) "_LIBC" "rewind" sys rewind 

	// int feof (FILE * stream)

	 ( FILE*:stream  -> Z32 ) "_LIBC" "feof" sys feof 

	// void clearerr (FILE * stream)

	 ( FILE*:stream ) "_LIBC" "clearerr" sys clearerr 

	// int ferror (FILE * stream)

	 ( FILE*:stream  -> Z32 ) "_LIBC" "ferror" sys ferror 

end

