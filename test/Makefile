ZHOME?=..

include $(ZHOME)/includes/zmake.inc


SRCtest=*.c
ZLIBS=-lzmem -lzstructures -lzthread
LIBS=

test: $(SRCtest) *.h $(INCDIR)/*.h
	make -C ..
	$(CC) $(CFLAGS) -o $@ $(SRCtest) -L$(OBJDIR) $(ZLIBS) $(LIBS)

clean: 
	-rm test


run: test
	gdb -ex run ./test
