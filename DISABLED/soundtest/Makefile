INCLUDES=-I../memory -I../structures -I..  -I../graphics -I../time  -L../objs 
LIBS =  ../objs/memory.o  ../objs/time.o ../objs/sound.o -lgraphics -lstructures    -lglut -lGLEW -lGLU -lGL -lopenal
test: main.c

	gcc -o test -g main.c $(INCLUDES) $(LIBS)

