ZCORE?=../
RELEASEDIR=$(ZCORE)/build#debug 
#RELEASEDIR=$(ZCORE)/release#release
INCLUDES=-I$(RELEASEDIR)/includes  -DRAM_DEBUG
OBJDIR=$(RELEASEDIR)/objs
ZLIBS=-lzstructures -lzmemory -lzstructures -lzthread -lzmisc -lzblank
LIBS=-lpthread
CC=gcc


.PHONY: ex
ex:
	#nothing

%:	%.c  
	$(CC) -g -o $@ $<  $(INCLUDES) -L$(OBJDIR)  $(LIBS) $(ZLIBS)


int:	int.c  example.h example.c gen_extension.h
	$(CC) -g -o $@ $<  $(INCLUDES) -L$(OBJDIR)  $(LIBS) $(ZLIBS) -DEXTENSION_H example.c
