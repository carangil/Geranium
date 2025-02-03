char* zstrcatsub(char* dest, char* src, zsize start, zsize count);
tokenT* hc_zstrcatsub (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
	ex->stack[ex->sp-4].as.ptr.block=(void*) zstrcatsub(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-4].as.ptr.level=0; 
	ex->stack[ex->sp-4].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+1);

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
void zw_enqueue(zwindowT* zw, zuint32 type, zuint32 a, zuint32 b, void* ptr);
tokenT* hc_zw_enqueue (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-5].as.ptr.block;
		zw_enqueue(
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-5+0);

	return tnext(t); 
}
zbool zw_queued(zwindowT* zw, zeventT* ev);
tokenT* hc_zw_queued (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.z32 =
		zw_queued(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
zbool zw_event(zwindowT* zw, zeventT* ev);
tokenT* hc_zw_event (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.z32 =
		zw_event(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
void zw_close(zwindowT* zw);
tokenT* hc_zw_close (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		zw_close(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void zprintevent(zeventT* ev);
tokenT* hc_zprintevent (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		zprintevent(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void checkGLfunc(char* file, int line, char* hint, zbool tolerable );
tokenT* hc_checkGLfunc (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		checkGLfunc(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.z32,
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
struct zwindow_s* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags);
tokenT* hc_gfx_mkwindow (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
	ex->stack[ex->sp-4].as.ptr.block=(void*) gfx_mkwindow(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-4].as.ptr.level=0; 
	ex->stack[ex->sp-4].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+1);

	return tnext(t); 
}
void gfx_swapbuffers(struct zwindow_s* zw);
tokenT* hc_gfx_swapbuffers (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_swapbuffers(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_mouse_delta(struct zwindow_s* zw, zbool rel);
tokenT* hc_gfx_mouse_delta (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gfx_mouse_delta(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_background_color(float r, float g, float b, float a);
tokenT* hc_gfx_background_color (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		gfx_background_color(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_frame_clear(zbool color, zbool depth);
tokenT* hc_gfx_frame_clear (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gfx_frame_clear(
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_depth_buffer(zbool test, zbool write);
tokenT* hc_gfx_depth_buffer (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gfx_depth_buffer(
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
tokenT* hc_gfx_setup_3d (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		gfx_setup_3d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_setup_2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom);
tokenT* hc_gfx_setup_2d (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		gfx_setup_2d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
float* gfx_get_projection_matrix();
tokenT* hc_gfx_get_projection_matrix (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-0].as.ptr.block;
	ex->stack[ex->sp-0].as.ptr.block=(void*) gfx_get_projection_matrix(
			);
	ex->stack[ex->sp-0].as.ptr.level=0; 
	ex->stack[ex->sp-0].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-0+1);

	return tnext(t); 
}
gfx_styleT* gfx_style_mk();
tokenT* hc_gfx_style_mk (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-0].as.ptr.block;
	ex->stack[ex->sp-0].as.ptr.block=(void*) gfx_style_mk(
			);
	ex->stack[ex->sp-0].as.ptr.level=0; 
	ex->stack[ex->sp-0].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-0+1);

	return tnext(t); 
}
void gfx_style_set_property(gfx_styleT* st, int idORtype, char* name_in, int index, int val, void* ptr, int action);
tokenT* hc_gfx_style_set_property (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-7].as.ptr.block;
		gfx_style_set_property(
			(void*)((ex->stack[ex->sp-7].as.ptr.block)+(ex->stack[ex->sp-7].as.ptr.offset)),
			ex->stack[ex->sp-6].as.z32,
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-7+0);

	return tnext(t); 
}
void gfx_style(gfx_styleT* st);
tokenT* hc_gfx_style (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_style(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount, char* spec);
tokenT* hc_gfx_vertex_buffer_mk (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.ptr.block=(void*) gfx_vertex_buffer_mk(
			ex->stack[ex->sp-2].as.z32,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb, zuint16 a, zuint16 b, zuint16 c);
tokenT* hc_gfx_index_triangle (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
	ex->stack[ex->sp-4].as.z32 =
		gfx_index_triangle(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+1);

	return tnext(t); 
}
void gfx_vertex_data(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
tokenT* hc_gfx_vertex_data (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-6].as.ptr.block;
		gfx_vertex_data(
			(void*)((ex->stack[ex->sp-6].as.ptr.block)+(ex->stack[ex->sp-6].as.ptr.offset)),
			ex->stack[ex->sp-5].as.z32,
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-6+0);

	return tnext(t); 
}
zuint16 gfx_vertex_done(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
tokenT* hc_gfx_vertex_done (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-6].as.ptr.block;
	ex->stack[ex->sp-6].as.z32 =
		gfx_vertex_done(
			(void*)((ex->stack[ex->sp-6].as.ptr.block)+(ex->stack[ex->sp-6].as.ptr.offset)),
			ex->stack[ex->sp-5].as.z32,
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-6+1);

	return tnext(t); 
}
void gfx_vertex_buffer_add_index(gfx_vertex_bufferT* vb, int num);
tokenT* hc_gfx_vertex_buffer_add_index (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gfx_vertex_buffer_add_index(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_vertex_buffer_update(gfx_vertex_bufferT* vb);
tokenT* hc_gfx_vertex_buffer_update (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_vertex_buffer_update(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_vertex_buffer_draw(gfx_vertex_bufferT* vb, int prim, zuint32 start, zuint32 end, zbool indexed);
tokenT* hc_gfx_vertex_buffer_draw (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-5].as.ptr.block;
		gfx_vertex_buffer_draw(
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-5+0);

	return tnext(t); 
}
gfx_vertex_bufferT* gfx_vertex_temp(struct zwindow_s* gw ,char* spec);
tokenT* hc_gfx_vertex_temp (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.ptr.block=(void*) gfx_vertex_temp(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
void gfx_vertex_buffer_draw_clear(gfx_vertex_bufferT* vb, zuint32 prim);
tokenT* hc_gfx_vertex_buffer_draw_clear (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gfx_vertex_buffer_draw_clear(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_vertex_buffer_continue(gfx_vertex_bufferT* vb, zuint32 prim, zuint32 count);
tokenT* hc_gfx_vertex_buffer_continue (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
		gfx_vertex_buffer_continue(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gfx_arrow(vec3* p1, vec3* p2, vec4* color);
tokenT* hc_gfx_arrow (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
		gfx_arrow(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+0);

	return tnext(t); 
}
zbool gfx_free_mesh(gfx_meshT* m);
tokenT* hc_gfx_free_mesh (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
	ex->stack[ex->sp-1].as.z32 =
		gfx_free_mesh(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+1);

	return tnext(t); 
}
void gfx_draw(int prim, zbool indexed, int start, int stop);
tokenT* hc_gfx_draw (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		gfx_draw(
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_spin_matrix(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gfx_mat_3x3* rot);
tokenT* hc_gfx_spin_matrix (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		gfx_spin_matrix(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_camera_init(gfx_cameraT* cam);
tokenT* hc_gfx_camera_init (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_camera_init(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_trans_init(gfx_transformT* cam);
tokenT* hc_gfx_trans_init (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_trans_init(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_camera_motion_6dof(gfx_cameraT* cam, float forward, float right, float up, float yaw, float pitch, float roll);
tokenT* hc_gfx_camera_motion_6dof (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-7].as.ptr.block;
		gfx_camera_motion_6dof(
			(void*)((ex->stack[ex->sp-7].as.ptr.block)+(ex->stack[ex->sp-7].as.ptr.offset)),
			ex->stack[ex->sp-6].as.f,
			ex->stack[ex->sp-5].as.f,
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-7+0);

	return tnext(t); 
}
void gfx_camera_view(gfx_cameraT* cam);
tokenT* hc_gfx_camera_view (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_camera_view(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_load_transform(gfx_transformT* trans);
tokenT* hc_gfx_load_transform (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_load_transform(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_save_transform(gfx_transformT* s);
tokenT* hc_gfx_save_transform (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_save_transform(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_blend_transform(float a, float b, gfx_transformT* trans);
tokenT* hc_gfx_blend_transform (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
		gfx_blend_transform(
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gfx_translate(vec3* delta) ;
tokenT* hc_gfx_translate (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_translate(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_translate3(float x, float y, float z);
tokenT* hc_gfx_translate3 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
		gfx_translate3(
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gfx_identity();
tokenT* hc_gfx_identity (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-0].as.ptr.block;
		gfx_identity(
			);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-0+0);

	return tnext(t); 
}
void gfx_rotate_x(float rad);
tokenT* hc_gfx_rotate_x (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_rotate_x(
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_rotate_y(float rad);
tokenT* hc_gfx_rotate_y (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_rotate_y(
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_rotate_z(float rad);
tokenT* hc_gfx_rotate_z (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_rotate_z(
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_rotate_3x3(gfx_mat_3x3* rot);
tokenT* hc_gfx_rotate_3x3 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_rotate_3x3(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_scale3(float x,float y, float z);
tokenT* hc_gfx_scale3 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
		gfx_scale3(
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gxi_refresh_matrix( struct gx_shader_variant_s* shader);
tokenT* hc_gxi_refresh_matrix (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gxi_refresh_matrix(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_trans_vec3(vec3* po);
tokenT* hc_gfx_trans_vec3 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_trans_vec3(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_trans_dir_vec3(vec3* pd);
tokenT* hc_gfx_trans_dir_vec3 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gfx_trans_dir_vec3(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_projection3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
tokenT* hc_gfx_projection3d (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		gfx_projection3d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_projection2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom);
tokenT* hc_gfx_projection2d (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		gfx_projection2d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
zbool zbitmap_cleanup(zbitmapT* bmp);
tokenT* hc_zbitmap_cleanup (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
	ex->stack[ex->sp-1].as.z32 =
		zbitmap_cleanup(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+1);

	return tnext(t); 
}
zbitmapT* zbitmap_mk(zuint32 w, zuint32 h, zuint16 format);
tokenT* hc_zbitmap_mk (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
	ex->stack[ex->sp-3].as.ptr.block=(void*) zbitmap_mk(
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-3].as.ptr.level=0; 
	ex->stack[ex->sp-3].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+1);

	return tnext(t); 
}
void zpset4(zbitmapT* bmp, zuint32 x, zuint32 y, zuint32 color);
tokenT* hc_zpset4 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
		zpset4(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+0);

	return tnext(t); 
}
zuint32 zpget4(zbitmapT* bmp, zuint32 x, zuint32 y);
tokenT* hc_zpget4 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
	ex->stack[ex->sp-3].as.z32 =
		zpget4(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+1);

	return tnext(t); 
}
void zline4(zbitmapT *bmp, zuint32 x, zuint32 y, zuint32 x2, zuint32 y2, zuint32 color);
tokenT* hc_zline4 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-6].as.ptr.block;
		zline4(
			(void*)((ex->stack[ex->sp-6].as.ptr.block)+(ex->stack[ex->sp-6].as.ptr.offset)),
			ex->stack[ex->sp-5].as.z32,
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-6+0);

	return tnext(t); 
}
void zpblit4(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch);
tokenT* hc_zpblit4 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-8].as.ptr.block;
		zpblit4(
			(void*)((ex->stack[ex->sp-8].as.ptr.block)+(ex->stack[ex->sp-8].as.ptr.offset)),
			ex->stack[ex->sp-7].as.z32,
			ex->stack[ex->sp-6].as.z32,
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-8+0);

	return tnext(t); 
}
void zpblit4c(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch, zuint32 color, zuint32 flags);
tokenT* hc_zpblit4c (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-10].as.ptr.block;
		zpblit4c(
			(void*)((ex->stack[ex->sp-10].as.ptr.block)+(ex->stack[ex->sp-10].as.ptr.offset)),
			ex->stack[ex->sp-9].as.z32,
			ex->stack[ex->sp-8].as.z32,
			(void*)((ex->stack[ex->sp-7].as.ptr.block)+(ex->stack[ex->sp-7].as.ptr.offset)),
			ex->stack[ex->sp-6].as.z32,
			ex->stack[ex->sp-5].as.z32,
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-10+0);

	return tnext(t); 
}
void zpblit4adebug(zbitmapT *bmp, zuint32 x, zuint32 y, zbitmapT* src, zuint32 srcx, zuint32 srcy, zuint32 srcw,zuint32 srch);
tokenT* hc_zpblit4adebug (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-8].as.ptr.block;
		zpblit4adebug(
			(void*)((ex->stack[ex->sp-8].as.ptr.block)+(ex->stack[ex->sp-8].as.ptr.offset)),
			ex->stack[ex->sp-7].as.z32,
			ex->stack[ex->sp-6].as.z32,
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-8+0);

	return tnext(t); 
}
void zdrawtext4(zbitmapT* dest, zbitmapT* font, char* text, int px, int py, zuint32 color, zuint32 flags);
tokenT* hc_zdrawtext4 (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-7].as.ptr.block;
		zdrawtext4(
			(void*)((ex->stack[ex->sp-7].as.ptr.block)+(ex->stack[ex->sp-7].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-6].as.ptr.block)+(ex->stack[ex->sp-6].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-7+0);

	return tnext(t); 
}
zbitmapT* zbitmap_load_tga( zchar* f, zuint32 flags);
tokenT* hc_zbitmap_load_tga (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.ptr.block=(void*) zbitmap_load_tga(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
gfx_textureT* gfx_texture_mk(zbitmapT* bmp);
tokenT* hc_gfx_texture_mk (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
	ex->stack[ex->sp-1].as.ptr.block=(void*) gfx_texture_mk(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-1].as.ptr.level=0; 
	ex->stack[ex->sp-1].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+1);

	return tnext(t); 
}
void gfx_texture_scaler(gfx_textureT* image, int scaler);
tokenT* hc_gfx_texture_scaler (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gfx_texture_scaler(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gxi_new_texture_set();
tokenT* hc_gxi_new_texture_set (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-0].as.ptr.block;
		gxi_new_texture_set(
			);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-0+0);

	return tnext(t); 
}
zuint32 gxi_add_texture(gfx_textureT* tex, zbool ff);
tokenT* hc_gxi_add_texture (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.z32 =
		gxi_add_texture(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
void gxi_texture_complete();
tokenT* hc_gxi_texture_complete (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-0].as.ptr.block;
		gxi_texture_complete(
			);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-0+0);

	return tnext(t); 
}
gfx_meshT* gfx_mesh_load_objmm(zchar* filename, float scale, vec3* min, vec3* max);
tokenT* hc_gfx_mesh_load_objmm (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
	ex->stack[ex->sp-4].as.ptr.block=(void*) gfx_mesh_load_objmm(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.f,
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-4].as.ptr.level=0; 
	ex->stack[ex->sp-4].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+1);

	return tnext(t); 
}
tokenT* hc_gfx_mesh_load_obj (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.ptr.block=(void*) gfx_mesh_load_obj(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.f);
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
gfx_jointT* load_bvh(char* filename, float scale, zvecT* ignorelist);
tokenT* hc_load_bvh (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
	ex->stack[ex->sp-3].as.ptr.block=(void*) load_bvh(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			ex->stack[ex->sp-2].as.f,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-3].as.ptr.level=0; 
	ex->stack[ex->sp-3].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+1);

	return tnext(t); 
}
void recurse_skeleton(gfx_jointT* joint, int frame, int op);
tokenT* hc_recurse_skeleton (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-3].as.ptr.block;
		recurse_skeleton(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void debug_print_skeleton(gfx_jointT* joint, int indent);
tokenT* hc_debug_print_skeleton (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		debug_print_skeleton(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
gx_shadergroupT* gx_shader_source(char* vsource, char* fsource);
tokenT* hc_gx_shader_source (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
	ex->stack[ex->sp-2].as.ptr.block=(void*) gx_shader_source(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+1);

	return tnext(t); 
}
void gx_set_basic_shader(char* vsource, char* fsource);
tokenT* hc_gx_set_basic_shader (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gx_set_basic_shader(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
gx_shader_variantT* gx_shader_variant(gx_shadergroupT* sg, char* key, gfx_styleT* st, gfx_vertex_bufferT* vb);
tokenT* hc_gx_shader_variant (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
	ex->stack[ex->sp-4].as.ptr.block=(void*) gx_shader_variant(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-4].as.ptr.level=0; 
	ex->stack[ex->sp-4].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+1);

	return tnext(t); 
}
gfx_shaderT* gx_compile_shader(char* vsource, char* fsource, zvecT* gfx_shader_inputs, int flags);
tokenT* hc_gx_compile_shader (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-4].as.ptr.block;
	ex->stack[ex->sp-4].as.ptr.block=(void*) gx_compile_shader(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-4].as.ptr.level=0; 
	ex->stack[ex->sp-4].as.ptr.offset=0; 

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-4+1);

	return tnext(t); 
}
int gfx_sizeof(int type);
tokenT* hc_gfx_sizeof (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
	ex->stack[ex->sp-1].as.z32 =
		gfx_sizeof(
			ex->stack[ex->sp-1].as.z32);

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+1);

	return tnext(t); 
}
void gfx_set_input(gfx_shader_inputT* input, void* data);
tokenT* hc_gfx_set_input (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-2].as.ptr.block;
		gfx_set_input(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gx_use_shader(gfx_shaderT* shader);
tokenT* hc_gx_use_shader (exectxT* ex, tokenT* t) {	
	void* firstArg = ex->stack[ex->sp-1].as.ptr.block;
		gx_use_shader(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	cleanCCall(ex, t, firstArg);

	ex->sp+= (-1+0);

	return tnext(t); 
}
#define SET_EXTENSIONS set_handlers
void set_handlers(){
	mkSymbol(global, "C_zstrcatsub", tPrimitive, hc_zstrcatsub);
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
	mkSymbol(global, "C_zw_enqueue", tPrimitive, hc_zw_enqueue);
	mkSymbol(global, "C_zw_queued", tPrimitive, hc_zw_queued);
	mkSymbol(global, "C_zw_event", tPrimitive, hc_zw_event);
	mkSymbol(global, "C_zw_close", tPrimitive, hc_zw_close);
	mkSymbol(global, "C_zprintevent", tPrimitive, hc_zprintevent);
	mkSymbol(global, "C_checkGLfunc", tPrimitive, hc_checkGLfunc);
	mkSymbol(global, "C_gfx_mkwindow", tPrimitive, hc_gfx_mkwindow);
	mkSymbol(global, "C_gfx_swapbuffers", tPrimitive, hc_gfx_swapbuffers);
	mkSymbol(global, "C_gfx_mouse_delta", tPrimitive, hc_gfx_mouse_delta);
	mkSymbol(global, "C_gfx_background_color", tPrimitive, hc_gfx_background_color);
	mkSymbol(global, "C_gfx_frame_clear", tPrimitive, hc_gfx_frame_clear);
	mkSymbol(global, "C_gfx_depth_buffer", tPrimitive, hc_gfx_depth_buffer);
	mkSymbol(global, "C_gfx_setup_3d", tPrimitive, hc_gfx_setup_3d);
	mkSymbol(global, "C_gfx_setup_2d", tPrimitive, hc_gfx_setup_2d);
	mkSymbol(global, "C_gfx_get_projection_matrix", tPrimitive, hc_gfx_get_projection_matrix);
	mkSymbol(global, "C_gfx_style_mk", tPrimitive, hc_gfx_style_mk);
	mkSymbol(global, "C_gfx_style_set_property", tPrimitive, hc_gfx_style_set_property);
	mkSymbol(global, "C_gfx_style", tPrimitive, hc_gfx_style);
	mkSymbol(global, "C_gfx_vertex_buffer_mk", tPrimitive, hc_gfx_vertex_buffer_mk);
	mkSymbol(global, "C_gfx_index_triangle", tPrimitive, hc_gfx_index_triangle);
	mkSymbol(global, "C_gfx_vertex_data", tPrimitive, hc_gfx_vertex_data);
	mkSymbol(global, "C_gfx_vertex_done", tPrimitive, hc_gfx_vertex_done);
	mkSymbol(global, "C_gfx_vertex_buffer_add_index", tPrimitive, hc_gfx_vertex_buffer_add_index);
	mkSymbol(global, "C_gfx_vertex_buffer_update", tPrimitive, hc_gfx_vertex_buffer_update);
	mkSymbol(global, "C_gfx_vertex_buffer_draw", tPrimitive, hc_gfx_vertex_buffer_draw);
	mkSymbol(global, "C_gfx_vertex_temp", tPrimitive, hc_gfx_vertex_temp);
	mkSymbol(global, "C_gfx_vertex_buffer_draw_clear", tPrimitive, hc_gfx_vertex_buffer_draw_clear);
	mkSymbol(global, "C_gfx_vertex_buffer_continue", tPrimitive, hc_gfx_vertex_buffer_continue);
	mkSymbol(global, "C_gfx_arrow", tPrimitive, hc_gfx_arrow);
	mkSymbol(global, "C_gfx_free_mesh", tPrimitive, hc_gfx_free_mesh);
	mkSymbol(global, "C_gfx_draw", tPrimitive, hc_gfx_draw);
	mkSymbol(global, "C_gfx_spin_matrix", tPrimitive, hc_gfx_spin_matrix);
	mkSymbol(global, "C_gfx_camera_init", tPrimitive, hc_gfx_camera_init);
	mkSymbol(global, "C_gfx_trans_init", tPrimitive, hc_gfx_trans_init);
	mkSymbol(global, "C_gfx_camera_motion_6dof", tPrimitive, hc_gfx_camera_motion_6dof);
	mkSymbol(global, "C_gfx_camera_view", tPrimitive, hc_gfx_camera_view);
	mkSymbol(global, "C_gfx_load_transform", tPrimitive, hc_gfx_load_transform);
	mkSymbol(global, "C_gfx_save_transform", tPrimitive, hc_gfx_save_transform);
	mkSymbol(global, "C_gfx_blend_transform", tPrimitive, hc_gfx_blend_transform);
	mkSymbol(global, "C_gfx_translate", tPrimitive, hc_gfx_translate);
	mkSymbol(global, "C_gfx_translate3", tPrimitive, hc_gfx_translate3);
	mkSymbol(global, "C_gfx_identity", tPrimitive, hc_gfx_identity);
	mkSymbol(global, "C_gfx_rotate_x", tPrimitive, hc_gfx_rotate_x);
	mkSymbol(global, "C_gfx_rotate_y", tPrimitive, hc_gfx_rotate_y);
	mkSymbol(global, "C_gfx_rotate_z", tPrimitive, hc_gfx_rotate_z);
	mkSymbol(global, "C_gfx_rotate_3x3", tPrimitive, hc_gfx_rotate_3x3);
	mkSymbol(global, "C_gfx_scale3", tPrimitive, hc_gfx_scale3);
	mkSymbol(global, "C_gxi_refresh_matrix", tPrimitive, hc_gxi_refresh_matrix);
	mkSymbol(global, "C_gfx_trans_vec3", tPrimitive, hc_gfx_trans_vec3);
	mkSymbol(global, "C_gfx_trans_dir_vec3", tPrimitive, hc_gfx_trans_dir_vec3);
	mkSymbol(global, "C_gfx_projection3d", tPrimitive, hc_gfx_projection3d);
	mkSymbol(global, "C_gfx_projection2d", tPrimitive, hc_gfx_projection2d);
	mkSymbol(global, "C_zbitmap_cleanup", tPrimitive, hc_zbitmap_cleanup);
	mkSymbol(global, "C_zbitmap_mk", tPrimitive, hc_zbitmap_mk);
	mkSymbol(global, "C_zpset4", tPrimitive, hc_zpset4);
	mkSymbol(global, "C_zpget4", tPrimitive, hc_zpget4);
	mkSymbol(global, "C_zline4", tPrimitive, hc_zline4);
	mkSymbol(global, "C_zpblit4", tPrimitive, hc_zpblit4);
	mkSymbol(global, "C_zpblit4c", tPrimitive, hc_zpblit4c);
	mkSymbol(global, "C_zpblit4adebug", tPrimitive, hc_zpblit4adebug);
	mkSymbol(global, "C_zdrawtext4", tPrimitive, hc_zdrawtext4);
	mkSymbol(global, "C_zbitmap_load_tga", tPrimitive, hc_zbitmap_load_tga);
	mkSymbol(global, "C_gfx_texture_mk", tPrimitive, hc_gfx_texture_mk);
	mkSymbol(global, "C_gfx_texture_scaler", tPrimitive, hc_gfx_texture_scaler);
	mkSymbol(global, "C_gxi_new_texture_set", tPrimitive, hc_gxi_new_texture_set);
	mkSymbol(global, "C_gxi_add_texture", tPrimitive, hc_gxi_add_texture);
	mkSymbol(global, "C_gxi_texture_complete", tPrimitive, hc_gxi_texture_complete);
	mkSymbol(global, "C_gfx_mesh_load_objmm", tPrimitive, hc_gfx_mesh_load_objmm);
	mkSymbol(global, "C_gfx_mesh_load_obj", tPrimitive, hc_gfx_mesh_load_obj);
	mkSymbol(global, "C_load_bvh", tPrimitive, hc_load_bvh);
	mkSymbol(global, "C_recurse_skeleton", tPrimitive, hc_recurse_skeleton);
	mkSymbol(global, "C_debug_print_skeleton", tPrimitive, hc_debug_print_skeleton);
	mkSymbol(global, "C_gx_shader_source", tPrimitive, hc_gx_shader_source);
	mkSymbol(global, "C_gx_set_basic_shader", tPrimitive, hc_gx_set_basic_shader);
	mkSymbol(global, "C_gx_shader_variant", tPrimitive, hc_gx_shader_variant);
	mkSymbol(global, "C_gx_compile_shader", tPrimitive, hc_gx_compile_shader);
	mkSymbol(global, "C_gfx_sizeof", tPrimitive, hc_gfx_sizeof);
	mkSymbol(global, "C_gfx_set_input", tPrimitive, hc_gfx_set_input);
	mkSymbol(global, "C_gx_use_shader", tPrimitive, hc_gx_use_shader);
	addCSize("zuint16", sizeof(zuint16));
	addCSize("namedv3", sizeof(namedv3));
	addCSize("namedv3_x", offsetof(namedv3,x));
	addCSize("namedv3_y", offsetof(namedv3,y));
	addCSize("namedv3_z", offsetof(namedv3,z));
	mkSymbol(global, "C_vec3add", tPrimitive, h_vec3add);
	mkSymbol(global, "C_vec3sub", tPrimitive, h_vec3sub);
	mkSymbol(global, "C_vec3cross", tPrimitive, h_vec3cross);
	mkSymbol(global, "C_vec3dot", tPrimitive, h_vec3dot);
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
	addCSize("gfx_vertex_bufferT", sizeof(gfx_vertex_bufferT));
	mkSymbol(global, "C_glslprocbody", tPrimitive, h_glslprocbody);
	mkSymbol(global, "C_prepshader", tPrimitive, h_prepshader);
	mkSymbol(global, "C_execdraw", tPrimitive, h_execdraw);
	addCSize("zeventT", sizeof(zeventT));
	addCSize("zeventT_type", offsetof(zeventT,type));
	addCSize("zeventT_a", offsetof(zeventT,a));
	addCSize("zeventT_b", offsetof(zeventT,b));
	addCSize("gfx_styleT", sizeof(gfx_styleT));
	addCSize("gfx_mat_3x3", sizeof(gfx_mat_3x3));
	addCSize("gfx_mat_3x3_x_axis", offsetof(gfx_mat_3x3,x_axis));
	addCSize("gfx_mat_3x3_y_axis", offsetof(gfx_mat_3x3,y_axis));
	addCSize("gfx_mat_3x3_z_axis", offsetof(gfx_mat_3x3,z_axis));
	addCSize("gfx_transformT", sizeof(gfx_transformT));
	addCSize("gfx_transformT_rot", offsetof(gfx_transformT,rot));
	addCSize("gfx_transformT_pos", offsetof(gfx_transformT,pos));
	addCSize("zbitmapT", sizeof(zbitmapT));
	addCSize("zbitmapT_w", offsetof(zbitmapT,w));
	addCSize("zbitmapT_h", offsetof(zbitmapT,h));
	addCSize("gfx_meshT", sizeof(gfx_meshT));
	addCSize("gfx_meshT_style", offsetof(gfx_meshT,style));
	addCSize("gfx_meshT_vb", offsetof(gfx_meshT,vb));
	addCSize("gfx_meshT_drawstart", offsetof(gfx_meshT,drawstart));
	addCSize("gfx_meshT_drawend", offsetof(gfx_meshT,drawend));
}
