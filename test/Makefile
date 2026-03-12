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
	./test
	#gdb -ex run ./test



debugger: test
	DEBUGINFOD_URLS=""  gdb -q -x cmds.gdb ./test
