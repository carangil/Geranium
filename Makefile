CC=gcc
COPY=cp
CONFIG?=debugconfig.inc

#OBJDIR and INCDIR are populated during the build
OBJDIR=build/objs
INCDIR=build/includes
MAKEFLAGS += --no-builtin-rules

include $(CONFIG)

CFLAGS += -I. -Iincludes -Ibuild/includes

objs: $(OBJDIR) $(INCDIR) $(OBJDIR)/libzmemory.a  $(OBJDIR)/libzthread.a  $(OBJDIR)/libzstructures.a #  $(OBJDIR)/time.o $(OBJDIR)/zmath.o  $(OBJDIR)/libgraphics.a


vars:
	@echo CFLAGS = $(CFLAGS)
	@echo CONFIG = $(CONFIG)
	@echo OBJDIR = $(OBJDIR)
	@echo INCDIR = $(INCDIR)
	@echo Type make objs to make build/objs/* and build/includes/*



$(OBJDIR):
	mkdir -p $(OBJDIR)

$(INCDIR):
	mkdir -p $(INCDIR)



.PRECIOUS: build/includes/%.h

#make .a from .o's
$(OBJDIR)/libz%.a: .%-objs
	ar rvcs $@ $*/*.o


include thread/thread.inc
include memory/memory.inc
include structures/structures.inc
#include graphics/graphics.inc
#include time/time.inc
#include sound/sound.inc
#include vmath/vmath.inc
include test/test.inc

clean:	clean-memory clean-thread clean-structures
	-rm $(OBJDIR)/*
	-rm $(INCDIR)/*



