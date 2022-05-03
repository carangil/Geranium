INCLUDES=-I../build/includes -I../includes
OBJDIR=../build/objs
LIBS=-lzmemory -lzstructures -lzthread -lpthread
CC=gcc

.PHONY test:	test.c 
	$(CC) -g -o test test.c  $(INCLUDES) -L$(OBJDIR) $(LIBS)
