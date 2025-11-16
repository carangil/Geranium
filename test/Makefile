ZHOME?=..

include $(ZHOME)/includes/zmake.inc


SRCtest=*.c
ZLIBS=-lzmem -lzstructures -lzthread
LIBS=

test: $(SRCtest) *.h $(INCDIR)/*.h $(OBJDIR)/*.a
	$(CC) $(CFLAGS) -o $@ $(SRCtest) -L$(OBJDIR) $(ZLIBS) $(LIBS)

clean: 
	-rm test

run: test
	./test
