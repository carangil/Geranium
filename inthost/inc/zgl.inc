section zgl

	type GLenum end
	// void int_init ()
	 ( ) "_INTERNAL" "zgl_int_init" sys optional int_init 

	// void test (int a,char * s)
	 ( Z32:_a String:_s ) "_INTERNAL" "zgl_test" sys optional test 

	// zbool init (void * procgetter)
	 ( Any*:_procgetter  -> Bit ) "_INTERNAL" "zgl_init" sys optional init 

	// const char * ErrorString (GLenum errorCode)
	 ( GLenum:_errorCode  -> String ) "_INTERNAL" "zglErrorString" sys optional ErrorString 

	// int CheckError (int spot)
	 ( Z32:_spot  -> Z32 ) "_INTERNAL" "zglCheckError" sys optional CheckError 

	// int BuildProgram (char * vsrc,char * fsrc,char ** rlog)
	 ( String:_vsrc String:_fsrc String$&:_rlog  -> Z32 ) "_INTERNAL" "zglBuildProgram" sys optional BuildProgram 

end

