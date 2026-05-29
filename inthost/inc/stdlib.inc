section stdlib

	// void qsort (anyArray base,size_t nmemb,size_t size,int (* compar)(const void *,const void *))

	 ( [Any]:base Size32:nmemb Size32:size (Any&:a Any&:b ->Z32):compar ) "_LIBC" "qsort" sys qsort 

	// char * getenv (const char * name)

	 ( String:name  -> String ) "_LIBC" "getenv" sys getenv 

end

