OBJSgraphics=
OBJSgraphics+=graphics/gx_buffers.o 
OBJSgraphics+=graphics/gx_image.o
OBJSgraphics+=graphics/gx_line.o
OBJSgraphics+=graphics/gx_misc.o
OBJSgraphics+=graphics/gx_sys.o
OBJSgraphics+=graphics/gx_drawstyle.o
OBJSgraphics+=graphics/gx_light.o
OBJSgraphics+=graphics/gx_mesh.o
OBJSgraphics+=graphics/gx_sprite.o
OBJSgraphics+=graphics/gx_quadpatch.o

OBJS_TOCLEAN += $(OBJSgraphics)

Hgraphics=
Hgraphics+=graphics/glstuff.h
Hgraphics+=graphics/gx_buffers.h
Hgraphics+=graphics/gx_drawstyle.h
Hgraphics+=graphics/gx_image.h
Hgraphics+=graphics/gx_light.h
Hgraphics+=graphics/gx_line.h
Hgraphics+=graphics/gx_mesh.h
Hgraphics+=graphics/gx_misc.h
Hgraphics+=graphics/gx_sprite.h
Hgraphics+=graphics/gx_sys.h
Hgraphics+=graphics/gx_quadpatch.h


graphics/%.o: %.c    $(Hgraphics) $(Hstructures) $(Hmemory)
	$(CC) $(CFLAGS) -c -o $@ $<

objs/libgraphics.a: $(OBJSgraphics) 
	ar rvcs $@  $(OBJSgraphics)



clean-graphics:
	-rm graphics/*.o
	-rm objs/libgraphics.a

graphics:  objs/libgraphics.a



