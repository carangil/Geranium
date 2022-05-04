CC=gcc
COPY=cp
CONFIG?=debugconfig.inc

include $(CONFIG)

#OBJDIR and INCDIR are populated during the build
OBJDIR?=build/objs
INCDIR?=build/includes
MAKEFLAGS += --no-builtin-rules


CFLAGS += -I. -Iincludes -I$(INCDIR)

objs: $(OBJDIR) $(INCDIR) .includes $(OBJDIR)/libzmemory.a $(OBJDIR)/libzthread.a $(OBJDIR)/libzstructures.a $(OBJDIR)/libzmisc.a $(OBJDIR)/libzvmath.a #  $(OBJDIR)/libgraphics.a
#$(OBJDIR)/libzblank_module.a	  Sample extra dir


vars:
	@echo CFLAGS = $(CFLAGS)
	@echo CONFIG = $(CONFIG)
	@echo OBJDIR = $(OBJDIR)
	@echo INCDIR = $(INCDIR)
	@echo Type make objs to make build/objs/* and build/includes/*


.includes: $(INCDIR)/ztypes.h	#list headers in includes that should be copied to build/includes
	touch .includes
	
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(INCDIR):
	mkdir -p $(INCDIR)

$(INCDIR)/%.h: includes/%.h
	$(COPY) $< $@

.PRECIOUS: $(INCDIR)/%.h

#make .a from .o's
$(OBJDIR)/libz%.a: .%-objs
	ar rvcs $@ $*/*.o

#blank_module is the 'example' template to copy from for new libs
#include blank_module/blank.inc
include thread/thread.inc
include memory/memory.inc
include structures/structures.inc
include misc/misc.inc
include vmath/vmath.inc
#include graphics/graphics.inc
#include sound/sound.inc

clean:	clean-memory clean-thread clean-structures clean-misc clean-vmath #clean-blank_module
	-rm $(OBJDIR)/*
	-rm $(INCDIR)/*



