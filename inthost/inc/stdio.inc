section stdio

	Z32   "SEEK_END" sys SEEK_END
	Z32   "SEEK_SET" sys SEEK_SET
	Z32   "SEEK_CUR" sys SEEK_CUR

	type FILE end
	// FILE * fopen (const char * filename,const char * mode)
	 ( String:_filename String:_mode  -> FILE* ) "_LIBC" "fopen" sys optional fopen 

	// int fflush (FILE * stream)
	 ( FILE*:_stream  -> Z32 ) "_LIBC" "fflush" sys optional fflush 

	// int fclose (FILE * stream)
	 ( FILE*:_stream  -> Z32 ) "_LIBC" "fclose" sys optional fclose 

	// int fgetc (FILE * stream)
	 ( FILE*:_stream  -> Z32 ) "_LIBC" "fgetc" sys optional fgetc 

	// char * fgets (char * s,int n,FILE * stream)
	 ( String:_s Z32:_n FILE*:_stream  -> String ) "_LIBC" "fgets" sys optional fgets 

	// size_t fread (void * ptr,size_t size,size_t nmemb,FILE * stream)
	 ( Any&:_ptr Size32:_size Size32:_nmemb FILE*:_stream  -> Size32 ) "_LIBC" "fread" sys optional fread 

	// int fputc (int c,FILE * stream)
	 ( Z32:_c FILE*:_stream  -> Z32 ) "_LIBC" "fputc" sys optional fputc 

	// int fputs (const char * s,FILE * stream)
	 ( String:_s FILE*:_stream  -> Z32 ) "_LIBC" "fputs" sys optional fputs 

	// size_t fwrite (const void * ptr,size_t size,size_t nmemb,FILE * stream)
	 ( Any&:_ptr Size32:_size Size32:_nmemb FILE*:_stream  -> Size32 ) "_LIBC" "fwrite" sys optional fwrite 

	// int fseek (FILE * stream,long int offset,int whence)
	 ( FILE*:_stream Long32:_offset Z32:_whence  -> Z32 ) "_LIBC" "fseek" sys optional fseek 

	// long int ftell (FILE * stream)
	 ( FILE*:_stream  -> Long32 ) "_LIBC" "ftell" sys optional ftell 

	// void rewind (FILE * stream)
	 ( FILE*:_stream ) "_LIBC" "rewind" sys optional rewind 

	// int feof (FILE * stream)
	 ( FILE*:_stream  -> Z32 ) "_LIBC" "feof" sys optional feof 

	// void clearerr (FILE * stream)
	 ( FILE*:_stream ) "_LIBC" "clearerr" sys optional clearerr 

	// int ferror (FILE * stream)
	 ( FILE*:_stream  -> Z32 ) "_LIBC" "ferror" sys optional ferror 

end

