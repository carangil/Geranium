INCLUDES=-I../memory -I../structures -I..  -I../graphics   -I../vmath   -L../objs 
LIBS =  ../objs/memory.o  ../objs/zmath.o  ../objs/thread.o  -lgraphics -lstructures   -lglut -lGLEW -lGLU -lGL -lm -pthread

CFILES=main.c gx_quadpatch.c

test: $(CFILES)  ../objs/*
	gcc -o test -g $(CFILES) $(INCLUDES) $(LIBS) -Wall

libs:
	(cd ..; make)

clean:
	rm test
