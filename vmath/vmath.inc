Hvmath=$(INCDIR)/zvectormath.h 
Hvmathdeps+=

#List all o files here
.vmath-objs: vmath/vectormath.o 
	touch .vmath-objs

vmath/%.o: vmath/%.c  $(Hvmath) $(Hvmathdeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(INCDIR)/%.h: vmath/%.h
	$(COPY) $< $(INCDIR)


$(INCDIR)/%.h: vmath/%.h
	$(COPY) $< $(INCDIR)


clean-vmath:
	echo clean-vmath
	-rm vmath/*.o


