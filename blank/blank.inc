Hblank=$(INCDIR)/blank.h
Hblankdeps+=$(INCDIR)/zstring.h

#List all o files here
.blank-objs: blank/blank.o
	touch .blank-objs

blank/%.o: blank/%.c  $(Hblank) $(Hblankdeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(INCDIR)/%.h: blank/%.h
	$(COPY) $< $(INCDIR)


$(INCDIR)/%.h: blank/%.h
	$(COPY) $< $(INCDIR)


clean-blank:
	echo clean-blank
	-rm blank/*.o


