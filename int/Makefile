ZCORE?=../
RELEASEDIR=$(ZCORE)/build#debug 
#RELEASEDIR=$(ZCORE)/release#release
INCLUDES=-I$(RELEASEDIR)/includes  -DRAM_DEBUG
OBJDIR=$(RELEASEDIR)/objs
ZLIBS=-lzstructures -lzmemory -lzstructures -lzthread -lzgfx_gl -lzmisc -lzblank 
LIBS=-lpthread -lm
CC=gcc



.PHONY: ex
ex:
	#nothing

%:	%.c  
	$(CC) -g -o $@ $<  $(INCLUDES) -L$(OBJDIR)  $(LIBS) $(ZLIBS)


int:	int.c  $(ZCORE)/.achange  #.achange is 'touch'ed after any zcore .a file is updated; forces rebuild/link
	$(CC) -O0 -g -o $@ $<  $(INCLUDES) -L$(OBJDIR)  $(LIBS) $(ZLIBS) -DEXTENSION_H   scan.c  int.c 	
