INCLUDES=-I../memory -I../structures -I..  -I../graphics  -L../objs 
LIBS =  ../objs/memory.o  -lgraphics -lstructures   -lglut -lGLEW -lGLU -lGL

CFILES=main.c systest.c


test: $(CFILES) $(DEPS)
	gcc -o test -g $(CFILES) $(INCLUDES) $(LIBS)

libs:
	(cd ..; make)

clean:
	rm test
