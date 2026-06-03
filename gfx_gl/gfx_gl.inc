Hgfx_gl= $(INCDIR)/ff.h $(INCDIR)/gfx_gl.h $(INCDIR)/gfx_texture.h $(INCDIR)/glsl.h $(INCDIR)/gx_trans.h $(INCDIR)/objloader.h
Hgfx_gldeps+=

#List all o files here
#ff.c  gfx_gl.c  gfx_texture.c  gfx_window.c  glsl.c  gx_trans.c  objloader.c

.gfx_gl-objs: gfx_gl/gfx_gl.o  gfx_gl/gfx_texture.o  gfx_gl/gfx_window.o  gfx_gl/glsl.o  gfx_gl/gx_trans.o  gfx_gl/objloader.o

	touch .gfx_gl-objs

gfx_gl/%.o: gfx_gl/%.c  $(Hgfx_gl) $(Hgfx_gldeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

#copy all h files
$(INCDIR)/%.h: gfx_gl/%.h
	echo doing copy $< $(INCDIR)
	$(COPY) $< $(INCDIR)


clean-gfx_gl:
	echo clean-gfx_gl
	-rm gfx_gl/*.o


