#test/stringtest: test/stringtest.c objs/memory.o objs/libstructures.a objs/thread.o
#	$(CC) $(CFLAGS)  -o test/stringtest test/stringtest.c objs/memory.o objs/libstructures.a objs/thread.o -lpthread


test/test: test/test.c objs/memory.o objs/libstructures.a objs/thread.o
	$(CC) $(CFLAGS)  -o test/test test/test.c objs/memory.o objs/libstructures.a objs/thread.o -lpthread

#test/test: test/test.c objs/memory.o objs/libstructures.a objs/thread.o
#	$(CC) $(CFLAGS)  -o test/test test/test.c objs/memory.o objs/libstructures.a objs/thread.o -lpthread



clean-test: 
	-rm test/test

