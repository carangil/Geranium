CC=gcc
CFLAGS=-g

#rule to build o files into c files
#%.o: %.c $(H)
#	$(CC) $(CFLAGS) -c -o $@ $<


objs: objs/memory.o  objs/thread.o  objs/libstructures.a  objs/time.o objs/vmath.o  objs/libgraphics.a 


include thread/thread.inc
include memory/memory.inc
include structures/structures.inc
include graphics/graphics.inc
include time/time.inc
#include sound/sound.inc
include vmath/vmath.inc

include test/test.inc

clean-objs: 
	-rm objs/*
	-rm $(OBJS_TOCLEAN)


clean: clean-objs

