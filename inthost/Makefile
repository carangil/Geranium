ZHOME?=..

include $(ZHOME)/includes/zmake.inc


SRCtest=*.c
ZLIBS=-lzint -lzmem -lzstructures -lzmem  -lzthread -lffi -lzgl
LIBS=


test: $(SRCtest) *.h ../interpreter/*.c ../interpreter/*.h
	make -C ..
	$(CC) $(CFLAGS) -o $@ $(SRCtest) -L$(OBJDIR) $(ZLIBS) $(LIBS)

clean: 
	-rm test

cleanall:
	-rm test
	make -C .. clean

generate:
	cd c-include ;	./generate

run: test
	./test

debugger: test
	DEBUGINFOD_URLS=""  gdb -q -x cmds.gdb ./test

debugscan: test
	DEBUGINFOD_URLS=""  gdb -q -x cmds.gdb --args ./test -scan $(ARGS)



debugstop: test
	DEBUGINFOD_URLS=""  gdb  ./test

