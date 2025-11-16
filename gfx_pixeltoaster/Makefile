ZHOME=..
LIBNAME=zgfx_pixeltoaster
include $(ZHOME)/includes/zmake.inc

#directory for pixeltoaster (change to use different version)
PIXELTOASTER=pixeltoaster


CPPFILES=$(wildcard *.cpp) $(PIXELTOASTER)/Profile.cpp  $(PIXELTOASTER)/PixelToaster.cpp
OFILES=$(CPPFILES:%.cpp=%.o)

CFLAGS += -I$(PIXELTOASTER)


$(OBJDIR)/lib$(LIBNAME).a: $(OFILES) *.h *.cpp
	ar rvcs $@ $(OFILES)
	$(COPY) *.h $(INCDIR)


%.o: %.cpp *.h  $(INCDIR)/.made $(PIXELTOASTER)/*.cpp $(PIXELTOASTER)/*.h
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm $(PIXELTOASTER)/*.o
	-rm $(OBJDIR)/lib$(LIBNAME).a




