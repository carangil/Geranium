section stdlib

	// void qsort (anyArray base,size_t nmemb,size_t size,int (* compar)(const void *,const void *))
	 ( [Any]:_base Size32:_nmemb Size32:_size (Any&:a Any&:b ->Z32):_compar ) "_LIBC" "qsort" sys optional qsort 

	// char * getenv (const char * name)
	 ( String:_name  -> String ) "_LIBC" "getenv" sys optional getenv 

	// void usleep (int microseconds)
	 ( Z32:_microseconds ) "_LIBC" "usleep" sys optional usleep 

end

