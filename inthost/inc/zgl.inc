section zgl

	type ShaderGroupT end
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

	// int BuildProgram (char * header,char * vsrc,char * fsrc,char ** rlog)
	 ( String:_header String:_vsrc String:_fsrc String$&:_rlog  -> Z32 ) "_INTERNAL" "zglBuildProgram" sys optional BuildProgram 

	// zglShaderGroupT * CreateShaderGroup (char * version,char * vsrc,char * fsrc,char * optnames)
	(String:_version String:_vsrc String:_fsrc String:_optnames  -> ShaderGroupT*$ ) "_INTERNAL" "zglCreateShaderGroup" sys optional CreateShaderGroup
	// int FindVariant (zglShaderGroupT * sg,int optionmask,char ** rlog)
	 ( ShaderGroupT*:_sg Z32:_optionmask String$&:_rlog  -> Z32 ) "_INTERNAL" "zglFindVariant" sys optional FindVariant 

	// void Dirty (void * v)
	 ( Any*:_v ) "_INTERNAL" "zglDirty" sys optional Dirty 

	// int BindUpdateArrayf (void * v,int elemsize,int target,int usage)
	 ( Any*:_v Z32:_elemsize Z32:_target Z32:_usage  -> Z32 ) "_INTERNAL" "zglBindUpdateArrayf" sys optional BindUpdateArrayf 

 /*zgl.post*/ 	([Any]:a  Z32:target Z32:usage) proc BindUpdateArray
	    a.cptr a.size1  target usage  BindUpdateArrayf
	end


	([Any]:a) proc BindUpdateArray
		a.cptr a.size1  gl.ARRAY_BUFFER 0 BindUpdateArrayf
	end

end

