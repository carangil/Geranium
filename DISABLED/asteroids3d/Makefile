INCLUDES=-I../memory -I../structures -I..  -I../graphics  -L../objs 
LIBS =  ../objs/memory.o  -lgraphics -lstructures   -lglut -lGLEW -lGLU -lGL
test: main.c
	gcc -o test -g main.c $(INCLUDES) $(LIBS)

