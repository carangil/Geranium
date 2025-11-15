Hgfx_pixeltoaster=$(INCDIR)/gfx_pixeltoaster.h
Hgfx_pixeltoasterdeps+=$(INCDIR)/ztypes.h

# from pixeltoaster makefile.linux:
PIXELTOASTER_CFLAGS = -O3 -Wall -DPLATFORM_UNIX -I$(PIXELTOASTER) -I$(INCDIR) $(ZFLAGS)
#PIXELTOASTER_LDFLAGS = -L/usr/X11R6/lib -lX11 -lrt


#List all o files here
.gfx_pixeltoaster-objs: gfx_pixeltoaster/gfx_pixeltoaster.o  gfx_pixeltoaster/PixelToaster.o
	touch .gfx_pixeltoaster-objs

#build our source files from our dir
gfx_pixeltoaster/%.o: gfx_pixeltoaster/%.cpp  $(Hgfx_pixeltoaster) $(Hgfx_pixeltoasterdeps)
	echo inddir is $(INCDIR)
	$(CXX) $(PIXELTOASTER_CFLAGS) $(PIXELTOASTER_LDFLAGS) -c -o $@ $< 

#build pixeltoaster source files from its dir
gfx_pixeltoaster/%.o: $(PIXELTOASTER)/%.cpp  $(Hgfx_pixeltoaster) $(Hgfx_pixeltoasterdeps)
	echo inddir is $INCDIR
	$(CXX) $(PIXELTOASTER_CFLAGS) $(PIXELTOASTER_LDFLAGS) -c -o $@ $< 


$(INCDIR)/%.h: gfx_pixeltoaster/%.h
	$(COPY) $< $(INCDIR)


$(INCDIR)/%.h: gfx_pixeltoaster/%.h
	$(COPY) $< $(INCDIR)


clean-gfx_pixeltoaster:
	echo clean-gfx_pixeltoaster
	-rm gfx_pixeltoaster/*.o


