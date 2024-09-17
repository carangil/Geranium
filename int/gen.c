tokenT* hc_zstrdup (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
	ex->stack[ex->sp-1].as.ptr.block=(void*) zstrdup(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-1].as.ptr.level=0; 
	ex->stack[ex->sp-1].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+1);

	return tnext(t); 
}
tokenT* hc_fopen (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.ptr.block=(void*) fopen(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
zuint32 zrand();
tokenT* hc_zrand (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-0].as.ptr.block;
	ex->stack[ex->sp-0].as.z32 =
		zrand(
			);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-0+1);

	return tnext(t); 
}
zfloat32 zrandf(zfloat32 min, zfloat32 max);
tokenT* hc_zrandf (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.f =
		zrandf(
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
void store16(zuint16 val, zuint16* zp);
tokenT* hc_store16 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		store16(
			ex->stack[ex->sp-2].as.z32,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
zint32 load16(zuint16 * z);
tokenT* hc_load16 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
	ex->stack[ex->sp-1].as.z32 =
		load16(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+1);

	return tnext(t); 
}
zbool ptrequal(void* a, void* b);
tokenT* hc_ptrequal (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.z32 =
		ptrequal(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
tokenT* hc_findType (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
	ex->stack[ex->sp-4].as.ptr.block=(void*) findType(
			ex->stack[ex->sp-4].as.z32,
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-4].as.ptr.level=0; 
	ex->stack[ex->sp-4].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+1);

	return tnext(t); 
}
void printTypeNoRedirect(typeT* ty, zbool line, zbool skipmembers);
tokenT* hc_printTypeNoRedirect (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
		printTypeNoRedirect(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void testComputeShader();
tokenT* hc_testComputeShader (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-0].as.ptr.block;
		testComputeShader(
			);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-0+0);

	return tnext(t); 
}
typeT* type_member(typeT* t, zuint32 i);
tokenT* hc_type_member (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.ptr.block=(void*) type_member(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
#define SET_EXTENSIONS set_handlers
void set_handlers(){
	mkSymbol(global, "C_zstrdup", tPrimitive, hc_zstrdup);
	mkSymbol(global, "C_fopen", tPrimitive, hc_fopen);
	mkSymbol(global, "C_zrand", tPrimitive, hc_zrand);
	mkSymbol(global, "C_zrandf", tPrimitive, hc_zrandf);
	mkSymbol(global, "C_store16", tPrimitive, hc_store16);
	mkSymbol(global, "C_load16", tPrimitive, hc_load16);
	mkSymbol(global, "C_ptrequal", tPrimitive, hc_ptrequal);
	mkSymbol(global, "C_findType", tPrimitive, hc_findType);
	mkSymbol(global, "C_printTypeNoRedirect", tPrimitive, hc_printTypeNoRedirect);
	mkSymbol(global, "C_testComputeShader", tPrimitive, hc_testComputeShader);
	mkSymbol(global, "C_type_member", tPrimitive, hc_type_member);
	addCSize("zuint16", sizeof(zuint16));
	addCSize("gfx_vertex_bufferT", sizeof(gfx_vertex_bufferT));
	addCSize("namedv3", sizeof(namedv3));
	addCSize("namedv3_x", offsetof(namedv3,x));
	addCSize("namedv3_y", offsetof(namedv3,y));
	addCSize("namedv3_z", offsetof(namedv3,z));
	mkSymbol(global, "C_vec3add", tPrimitive, h_vec3add);
	mkSymbol(global, "C_vec3sub", tPrimitive, h_vec3sub);
	mkSymbol(global, "C_vec3cross", tPrimitive, h_vec3cross);
	mkSymbol(global, "C_vec3dot", tPrimitive, h_vec3dot);
	mkSymbol(global, "C_glslprocbody", tPrimitive, h_glslprocbody);
	mkSymbol(global, "C_prepshader", tPrimitive, h_prepshader);
	mkSymbol(global, "C_execdraw", tPrimitive, h_execdraw);
	addCSize("typeT", sizeof(typeT));
	addCSize("typeT_name", offsetof(typeT,name));
	addCSize("typeT_size", offsetof(typeT,size));
	addCSize("typeT_category", offsetof(typeT,category));
	addCSize("typeT_ref", offsetof(typeT,ref));
	addCSize("typeT_len", offsetof(typeT,len));
	addCSize("vptrT", sizeof(vptrT));
	addCSize("vptrT", sizeof(vptrT));
	addCSize("vptrT", sizeof(vptrT));
	mkSymbol(global, "C_heretoken", tPrimitive, h_heretoken);
	mkSymbol(global, "C_tokenclip", tPrimitive, h_tokenclip);
	mkSymbol(global, "C_codecat", tPrimitive, h_codecat);
	mkSymbol(global, "C_tokeninsert", tPrimitive, h_tokeninsert);
	mkSymbol(global, "C_tokennext", tPrimitive, h_tokennext);
	mkSymbol(global, "C_tokenstring", tPrimitive, h_tokenstring);
	addCSize("valueT", sizeof(valueT));
	mkSymbol(global, "C_firsttoken", tPrimitive, h_firsttoken);
	mkSymbol(global, "C_tokenvaltype", tPrimitive, h_tokenvaltype);
	mkSymbol(global, "C_tokenevaltype", tPrimitive, h_tokenevaltype);
	mkSymbol(global, "C_tokenprim", tPrimitive, h_tokenprim);
	mkSymbol(global, "C_tokensymbol", tPrimitive, h_tokensymbol);
	mkSymbol(global, "C_symboltype", tPrimitive, h_symboltype);
}
