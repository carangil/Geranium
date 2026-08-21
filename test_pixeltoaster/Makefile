ZHOME?=..

include $(ZHOME)/includes/zmake.inc


SRCtest=*.c
ZLIBS=   -lzgfx_pixeltoaster    -lzmisc -lzstructures  -lzmem  -lzstructures -lzthread
#-lzgfx_pixeltoaster -lzmem -lzstructures -lzthread -lzmisc
LIBS=-lpthread  -L/usr/X11R6/lib -lX11 -lstdc++ -lm

test: $(SRCtest)   $(OBJDIR)/*.a
	$(CC) $(CFLAGS) -o $@ $(SRCtest) -L$(OBJDIR) $(ZLIBS) $(LIBS)

clean: 
	-rm test

run: test
	./test
