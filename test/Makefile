ZCORE?=../
RELEASEDIR=$(ZCORE)/build#debug 
#RELEASEDIR=$(ZCORE)/release#release
INCLUDES=-I$(RELEASEDIR)/includes  -DRAM_DEBUG
OBJDIR=$(RELEASEDIR)/objs
ZLIBS=-lzstructures -lzmemory -lzstructures -lzthread -lzmisc -lzblank
LIBS=-lpthread
CC=gcc

.PHONY explicit:
	#nothing

%:	%.c  explicit
	$(CC) -g -o $@ $<  $(INCLUDES) -L$(OBJDIR)  $(LIBS) $(ZLIBS)
