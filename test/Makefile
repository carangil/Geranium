RELEASEDIR=../build#debug 
#RELEASEDIR=../release#release
INCLUDES=-I$(RELEASEDIR)/includes 
OBJDIR=$(RELEASEDIR)/objs
ZLIBS=-lzmemory -lzstructures -lzthread -lzmisc
LIBS=-lpthread
CC=gcc

.PHONY test:	test.c 
	$(CC) -g -o test test.c  $(INCLUDES) -L$(OBJDIR) $(ZLIBS) $(LIBS)
