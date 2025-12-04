ZHOME?=..

include $(ZHOME)/includes/zmake.inc


SRCtest=*.c
ZLIBS=-lzint -lzmem -lzstructures -lzmem  -lzthread
LIBS=


test: $(SRCtest) *.h $(INCDIR)/*.h $(OBJDIR)/*.a 
	make -C ..
	$(CC) $(CFLAGS) -o $@ $(SRCtest) -L$(OBJDIR) $(ZLIBS) $(LIBS)

clean: 
	-rm test

run: test
	./test

debugger: test
	DEBUGINFOD_URLS=""  gdb -q -x cmds.gdb ./test

