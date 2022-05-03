Hstructures=$(INCDIR)/zlist.h   $(INCDIR)/zvector.h $(INCDIR)/zarray.h  $(INCDIR)/zstring.h
Hstructuresdeps+=$(INCDIR)/zmem.h

#List all o files here
.structures-objs: structures/zlist.o structures/zarray.o structures/zstring.o structures/zvector.o
	touch .structures-objs

structures/%.o: structures/%.c  $(Hstructures) $(Hstructuresdeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(INCDIR)/%.h: structures/%.h
	$(COPY) $< $(INCDIR)


$(INCDIR)/%.h: structures/%.h
	$(COPY) $< $(INCDIR)


clean-structures:
	echo clean-structures
	-rm structures/*.o


