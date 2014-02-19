CC=gcc
CFLAGS=-g

#rule to build o files into c files
#%.o: %.c $(H)
#	$(CC) $(CFLAGS) -c -o $@ $<


objs: objs/memory.o objs/libstructures.a objs/libgraphics.a objs/time.o objs/sound.o objs/vmath.o


include memory/memory.inc
include structures/structures.inc
include graphics/graphics.inc
include time/time.inc
include sound/sound.inc
include vmath/vmath.inc




clean-objs: 
	-rm objs/*
	-rm $(OBJS_TOCLEAN)


clean: clean-objs

