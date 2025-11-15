RELEASEDIR=../build#debug 
#RELEASEDIR=../release#release
INCLUDES=-I$(RELEASEDIR)/includes 
OBJDIR=$(RELEASEDIR)/objs
ZLIBS=-lzthread -lzmisc -lzgfx_pixeltoaster -lzblank  -lzstructures -lzmemory -lzstructures -lzthread -lzmisc
LIBS=-lpthread  -L/usr/X11R6/lib -lX11 -lrt -lstdc++ -lm
CC=gcc

.PHONY test:	test.c 
	$(CC) -g -o test gltest.c  $(INCLUDES) -L$(OBJDIR) $(ZLIBS) $(LIBS)
