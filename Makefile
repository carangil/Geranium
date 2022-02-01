CC=gcc
CONFIG?=debugconfig.inc

include $(CONFIG)

CFLAGS += -I. -Imemory -Ivmath -Istructures

vars:
	@echo CFLAGS = $(CFLAGS)
	@echo CONFIG = $(CONFIG)


objs: objs/memory.o  objs/thread.o  objs/libstructures.a #  objs/time.o objs/zmath.o  objs/libgraphics.a 


include thread/thread.inc
include memory/memory.inc
include structures/structures.inc
#include graphics/graphics.inc
#include time/time.inc
#include sound/sound.inc
#include vmath/vmath.inc
include test/test.inc

clean-objs: 
	-rm objs/*
	-rm $(OBJS_TOCLEAN)


clean: clean-objs

