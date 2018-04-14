OBJSgraphics=
OBJSgraphics+=graphics/gx_buffers.o 
OBJSgraphics+=graphics/gx_image.o
#OBJSgraphics+=graphics/gx_line.o
#OBJSgraphics+=graphics/gx_misc.o
OBJSgraphics+=graphics/gx_trans.o
OBJSgraphics+=graphics/gx_sys.o
OBJSgraphics+=graphics/gx_drawstyle.o
OBJSgraphics+=graphics/gx_light.o
#OBJSgraphics+=graphics/gx_mesh.o
#OBJSgraphics+=graphics/gx_sprite.o
OBJSgraphics+=graphics/gx_quadpatch.o


OBJS_TOCLEAN += $(OBJSgraphics)

Hgraphics=
Hgraphics+=graphics/glheaders.h
Hgraphics+=graphics/gx_buffers.h
Hgraphics+=graphics/gx_drawstyle.h
Hgraphics+=graphics/gx_image.h
Hgraphics+=graphics/gx_light.h
#Hgraphics+=graphics/gx_line.h
#Hgraphics+=graphics/gx_mesh.h
#Hgraphics+=graphics/gx_misc.h
Hgraphics+=graphics/gx_trans.h
#Hgraphics+=graphics/gx_sprite.h
Hgraphics+=graphics/gx_sys.h
Hgraphics+=graphics/gx_quadpatch.h
Hgraphics+=graphics/defshader.v.h
Hgraphics+=graphics/defshader.f.h


graphics/defshader.v.h: graphics/shader.v ./tools/string-to-c
	tools/string-to-c graphics/shader.v graphics/defshader.v.h gxi_def_shader_v
	
graphics/defshader.f.h: graphics/shader.f ./tools/string-to-c
	tools/string-to-c graphics/shader.f graphics/defshader.f.h gxi_def_shader_f
	
graphics/%.o: graphics/%.c    $(Hgraphics) $(Hstructures) $(Hmemory) 
	$(CC)   $(CFLAGS)  -c -o $@ $< 

	
objs/libgraphics.a: $(Hgraphics)  $(OBJSgraphics) 
	ar rvcs $@  $(OBJSgraphics)




clean-graphics:
	-rm graphics/*.o
	-rm objs/libgraphics.a
	-rm graphics/defshader.f.h graphics/defshader.v.h 

graphics:  objs/libgraphics.a



