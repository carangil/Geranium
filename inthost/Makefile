ZHOME?=..

include $(ZHOME)/includes/zmake.inc


SRCtest=*.c
ZLIBS=-lzint -lzmem -lzstructures -lzmem  -lzthread
LIBS=


test: $(SRCtest) *.h
	make -C ..
	$(CC) $(CFLAGS) -o $@ $(SRCtest) -L$(OBJDIR) $(ZLIBS) $(LIBS)

clean: 
	-rm test

cleanall:
	-rm test
	make -C .. clean

run: test
	./test

debugger: test
	DEBUGINFOD_URLS=""  gdb -q -x cmds.gdb ./test

