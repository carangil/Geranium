void zw_enqueue(zwindowT* zw, zuint32 type, zuint32 a, zuint32 b, void* ptr);
tokenT* hc_zw_enqueue (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		zw_enqueue(
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-5+0);

	return tnext(t); 
}
zbool zw_queued(zwindowT* zw, zeventT* ev);
tokenT* hc_zw_queued (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-2].as.z32 =
		zw_queued(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-2+1);

	return tnext(t); 
}
zbool zw_event(zwindowT* zw, zeventT* ev);
tokenT* hc_zw_event (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-2].as.z32 =
		zw_event(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-2+1);

	return tnext(t); 
}
void zw_close(zwindowT* zw);
tokenT* hc_zw_close (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		zw_close(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void zw_pixels(zwindowT* zw, void* v);
tokenT* hc_zw_pixels (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		zw_pixels(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-2+0);

	return tnext(t); 
}
void zprintevent(zeventT* ev);
tokenT* hc_zprintevent (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		zprintevent(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void checkGLfunc(char* file, int line, char* hint, zbool tolerable );
tokenT* hc_checkGLfunc (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		checkGLfunc(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.z32,
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-4+0);

	return tnext(t); 
}
struct zwindow_s* gfx_mkwindow(char* title, zuint32 w, zuint32 h, zuint32 flags);
tokenT* hc_gfx_mkwindow (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-4].as.ptr.block= gfx_mkwindow(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-4].as.ptr.level=0; 
	ex->stack[ex->sp-4].as.ptr.offset=0; 

	ex->sp+= (-4+1);

	return tnext(t); 
}
 void gfx_background_color(float r, float g, float b, float a);
tokenT* hc_gfx_background_color (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_background_color(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_frame_clear(zbool color, zbool depth);
tokenT* hc_gfx_frame_clear (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_frame_clear(
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_depth_buffer(zbool test, zbool write);
tokenT* hc_gfx_depth_buffer (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_depth_buffer(
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_setup_3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
tokenT* hc_gfx_setup_3d (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_setup_3d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_setup_2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom);
tokenT* hc_gfx_setup_2d (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_setup_2d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-4+0);

	return tnext(t); 
}
gfx_styleT* gfx_style_mk();
tokenT* hc_gfx_style_mk (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-0].as.ptr.block= gfx_style_mk(
			);
	ex->stack[ex->sp-0].as.ptr.level=0; 
	ex->stack[ex->sp-0].as.ptr.offset=0; 

	ex->sp+= (-0+1);

	return tnext(t); 
}
void gfx_style_set_property(gfx_styleT* st, int idORtype, char* name_in, int index, int val, void* ptr, int action);
tokenT* hc_gfx_style_set_property (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_style_set_property(
			(void*)((ex->stack[ex->sp-7].as.ptr.block)+(ex->stack[ex->sp-7].as.ptr.offset)),
			ex->stack[ex->sp-6].as.z32,
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-7+0);

	return tnext(t); 
}
void gfx_style(gfx_styleT* st);
tokenT* hc_gfx_style (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_style(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
  gfx_vertex_bufferT* gfx_vertex_buffer_mk(zuint16 vcount, char* spec);
tokenT* hc_gfx_vertex_buffer_mk (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-2].as.ptr.block= gfx_vertex_buffer_mk(
			ex->stack[ex->sp-2].as.z32,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	ex->sp+= (-2+1);

	return tnext(t); 
}
zuint16 gfx_index_triangle(gfx_vertex_bufferT* vb, zuint16 a, zuint16 b, zuint16 c);
tokenT* hc_gfx_index_triangle (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-4].as.z32 =
		gfx_index_triangle(
			(void*)((ex->stack[ex->sp-4].as.ptr.block)+(ex->stack[ex->sp-4].as.ptr.offset)),
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-4+1);

	return tnext(t); 
}
void gfx_vertex_data(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
tokenT* hc_gfx_vertex_data (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_vertex_data(
			(void*)((ex->stack[ex->sp-6].as.ptr.block)+(ex->stack[ex->sp-6].as.ptr.offset)),
			ex->stack[ex->sp-5].as.z32,
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-6+0);

	return tnext(t); 
}
zuint16 gfx_vertex_done(gfx_vertex_bufferT* vb, int attr, float a, float b, float c, float d);
tokenT* hc_gfx_vertex_done (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-6].as.z32 =
		gfx_vertex_done(
			(void*)((ex->stack[ex->sp-6].as.ptr.block)+(ex->stack[ex->sp-6].as.ptr.offset)),
			ex->stack[ex->sp-5].as.z32,
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-6+1);

	return tnext(t); 
}
zuint16* gfx_vertex_buffer_add_index(gfx_vertex_bufferT* vb, int num);
tokenT* hc_gfx_vertex_buffer_add_index (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-2].as.ptr.block= gfx_vertex_buffer_add_index(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	ex->sp+= (-2+1);

	return tnext(t); 
}
void gfx_vertex_buffer_update(gfx_vertex_bufferT* vb);
tokenT* hc_gfx_vertex_buffer_update (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_vertex_buffer_update(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_vertex_buffer_draw(gfx_vertex_bufferT* vb, int prim, int start, int end, zbool indexed);
tokenT* hc_gfx_vertex_buffer_draw (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_vertex_buffer_draw(
			(void*)((ex->stack[ex->sp-5].as.ptr.block)+(ex->stack[ex->sp-5].as.ptr.offset)),
			ex->stack[ex->sp-4].as.z32,
			ex->stack[ex->sp-3].as.z32,
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-5+0);

	return tnext(t); 
}
gfx_vertex_bufferT* gfx_vertex_temp(struct zwindow_s* gw ,char* spec);
tokenT* hc_gfx_vertex_temp (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-2].as.ptr.block= gfx_vertex_temp(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));
	ex->stack[ex->sp-2].as.ptr.level=0; 
	ex->stack[ex->sp-2].as.ptr.offset=0; 

	ex->sp+= (-2+1);

	return tnext(t); 
}
void gfx_vertex_buffer_draw_clear(gfx_vertex_bufferT* vb, zuint32 prim);
tokenT* hc_gfx_vertex_buffer_draw_clear (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_vertex_buffer_draw_clear(
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-2+0);

	return tnext(t); 
}
void gfx_vertex_buffer_continue(gfx_vertex_bufferT* vb, zuint32 prim, zuint32 count);
tokenT* hc_gfx_vertex_buffer_continue (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_vertex_buffer_continue(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			ex->stack[ex->sp-2].as.z32,
			ex->stack[ex->sp-1].as.z32);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gfx_arrow(vec3* p1, vec3* p2, vec4* color);
tokenT* hc_gfx_arrow (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_arrow(
			(void*)((ex->stack[ex->sp-3].as.ptr.block)+(ex->stack[ex->sp-3].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-2].as.ptr.block)+(ex->stack[ex->sp-2].as.ptr.offset)),
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-3+0);

	return tnext(t); 
}
zbool gfx_free_mesh(gfx_meshT* m);
tokenT* hc_gfx_free_mesh (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-1].as.z32 =
		gfx_free_mesh(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+1);

	return tnext(t); 
}
void gfx_spin_matrix(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gfx_mat_3x3* rot);
tokenT* hc_gfx_spin_matrix (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_spin_matrix(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_camera_init(gfx_cameraT* cam);
tokenT* hc_gfx_camera_init (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_camera_init(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_trans_init(gfx_transformT* cam);
tokenT* hc_gfx_trans_init (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_trans_init(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_camera_motion_6dof(gfx_cameraT* cam, float forward, float right, float up, float yaw, float pitch, float roll);
tokenT* hc_gfx_camera_motion_6dof (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_camera_motion_6dof(
			(void*)((ex->stack[ex->sp-7].as.ptr.block)+(ex->stack[ex->sp-7].as.ptr.offset)),
			ex->stack[ex->sp-6].as.f,
			ex->stack[ex->sp-5].as.f,
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-7+0);

	return tnext(t); 
}
void gfx_camera_view(gfx_cameraT* cam);
tokenT* hc_gfx_camera_view (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_camera_view(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_load_transform(gfx_transformT* trans);
tokenT* hc_gfx_load_transform (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_load_transform(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_save_transform(gfx_transformT* s);
tokenT* hc_gfx_save_transform (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_save_transform(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_blend_transform(float a, float b, gfx_transformT* trans);
tokenT* hc_gfx_blend_transform (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_blend_transform(
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gfx_translate(vec3* delta) ;
tokenT* hc_gfx_translate (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_translate(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_translate3(float x, float y, float z);
tokenT* hc_gfx_translate3 (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_translate3(
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gfx_identity();
tokenT* hc_gfx_identity (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_identity(
			);

	ex->sp+= (-0+0);

	return tnext(t); 
}
void gfx_rotate_x(float rad);
tokenT* hc_gfx_rotate_x (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_rotate_x(
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_rotate_y(float rad);
tokenT* hc_gfx_rotate_y (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_rotate_y(
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_rotate_z(float rad);
tokenT* hc_gfx_rotate_z (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_rotate_z(
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_rotate_3x3(gfx_mat_3x3* rot);
tokenT* hc_gfx_rotate_3x3 (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_rotate_3x3(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_scale3(float x,float y, float z);
tokenT* hc_gfx_scale3 (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_scale3(
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-3+0);

	return tnext(t); 
}
void gxi_refresh_matrix( struct gx_shader_variant_s* shader);
tokenT* hc_gxi_refresh_matrix (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gxi_refresh_matrix(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_trans_vec3(vec3* po);
tokenT* hc_gfx_trans_vec3 (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_trans_vec3(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_trans_dir_vec3(vec3* pd);
tokenT* hc_gfx_trans_dir_vec3 (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_trans_dir_vec3(
			(void*)((ex->stack[ex->sp-1].as.ptr.block)+(ex->stack[ex->sp-1].as.ptr.offset)));

	ex->sp+= (-1+0);

	return tnext(t); 
}
void gfx_projection3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
tokenT* hc_gfx_projection3d (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_projection3d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-4+0);

	return tnext(t); 
}
void gfx_projection2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom);
tokenT* hc_gfx_projection2d (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
		gfx_projection2d(
			ex->stack[ex->sp-4].as.f,
			ex->stack[ex->sp-3].as.f,
			ex->stack[ex->sp-2].as.f,
			ex->stack[ex->sp-1].as.f);

	ex->sp+= (-4+0);

	return tnext(t); 
}
#define SET_EXTENSIONS set_handlers
void set_handlers(){
	mkSymbol(global, "C_zw_enqueue", tPrimitive, hc_zw_enqueue);
	mkSymbol(global, "C_zw_queued", tPrimitive, hc_zw_queued);
	mkSymbol(global, "C_zw_event", tPrimitive, hc_zw_event);
	mkSymbol(global, "C_zw_close", tPrimitive, hc_zw_close);
	mkSymbol(global, "C_zw_pixels", tPrimitive, hc_zw_pixels);
	mkSymbol(global, "C_zprintevent", tPrimitive, hc_zprintevent);
	mkSymbol(global, "C_checkGLfunc", tPrimitive, hc_checkGLfunc);
	mkSymbol(global, "C_gfx_mkwindow", tPrimitive, hc_gfx_mkwindow);
	mkSymbol(global, "C_gfx_background_color", tPrimitive, hc_gfx_background_color);
	mkSymbol(global, "C_gfx_frame_clear", tPrimitive, hc_gfx_frame_clear);
	mkSymbol(global, "C_gfx_depth_buffer", tPrimitive, hc_gfx_depth_buffer);
	mkSymbol(global, "C_gfx_setup_3d", tPrimitive, hc_gfx_setup_3d);
	mkSymbol(global, "C_gfx_setup_2d", tPrimitive, hc_gfx_setup_2d);
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
	addCSize("zeventT", sizeof(zeventT));
	addCSize("zeventT_type", offsetof(zeventT,type));
	addCSize("zeventT_a", offsetof(zeventT,a));
	addCSize("zeventT_b", offsetof(zeventT,b));
}
