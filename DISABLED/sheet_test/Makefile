INCLUDES=-I../memory -I../structures -I..  -I../graphics  -L../objs 
LIBS =  ../objs/memory.o  ../objs/vmath.o  -lgraphics -lstructures   -lglut -lGLEW -lGLU -lGL

CFILES=main.c 

test: $(CFILES) 
	gcc -o test -g $(CFILES) $(INCLUDES) $(LIBS) -Wall

libs:
	(cd ..; make)

clean:
	rm test
