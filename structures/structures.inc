OBJSstructures=structures/linkedlist.o structures/vector.o  structures/zarray.o

Hstructures=structures/linkedlist.h structures/zarray.h structures/vector.h

OBJS_TOCLEAN += $(OBJSstructures)

objs/libstructures.a: $(OBJSstructures)
	ar rvcs $@ $(OBJSstructures)

clean-structures:
	-rm structures/*.o
	-rm objs/libstructures.a

structures: objs/libstructures.a
