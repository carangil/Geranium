ZHOME=.
include $(ZHOME)/includes/zmake.inc


SUBDIRS=memory

.PHONY: default remake

default: build/includes/.made build/objs
	#make all the subdirectories
	make -C memory 
	make -C thread
	make -C structures
	make -C misc
	make -C vectormath
	make -C gfx_pixeltoaster
	make -C interpreter
	touch build/objs/.made #keeps track of when the objs were updated
	@echo done

remake: clean default




#When creating the build/includes folder, copy all that's in includes to it.  This for now is just ztypes.h
#Also copy in EARLYH files.  These are header files that need to be copied before building any modules.
#zlist.h and zthread.h are included because they are required to build zmemory, and zmemory is required to build anything
EARLYH=thread/zthread.h structures/zlist.h
build/includes/.made: includes/ztypes.h $(EARLYH)
	#the .made file just keeps track of when the last time these files were copied
	mkdir -p build/includes
	$(COPY) includes/*.h build/includes
	$(COPY) $(EARLYH) build/includes
	touch build/includes/.made


build/objs:
	mkdir -p build/objs


clean:
	rm -rf build

