OBJSstructures=structures/zlist.o structures/zvector.o  #structures/zarray.o  structures/zstring.o

Hstructures=structures/zlist.h   structures/zvector.h  #structures/zarray.h

OBJS_TOCLEAN += $(OBJSstructures)

objs/libstructures.a: $(OBJSstructures)
	ar rvcs $@ $(OBJSstructures)


structures/%.o: %.c   $(Hstructures) $(Hmemory)
	$(CC) $(CFLAGS) -c -o $@ $<

clean-structures:
	-rm structures/*.o
	-rm objs/libstructures.a

structures: objs/libstructures.a
