RELEASEDIR=../build#debug 
#RELEASEDIR=../release#release
INCLUDES=-I$(RELEASEDIR)/includes 
OBJDIR=$(RELEASEDIR)/objs
ZLIBS=-lzstructures -lzmemory -lzstructures -lzthread -lzmisc -lzblank
LIBS=-lpthread
CC=gcc

.PHONY test:	test.c 
	$(CC) -g -o test test.c  $(INCLUDES) -L$(OBJDIR)  $(LIBS) $(ZLIBS)
