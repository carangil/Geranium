Hmisc=$(INCDIR)/ztime.h $(INCDIR)/zrand.h
Hmiscdeps+=

#List all o files here
.misc-objs: misc/tm.o misc/zrand.o
	touch .misc-objs

misc/%.o: misc/%.c  $(Hmisc) $(Hmiscdeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(INCDIR)/%.h: misc/%.h
	$(COPY) $< $(INCDIR)


$(INCDIR)/%.h: misc/%.h
	$(COPY) $< $(INCDIR)


clean-misc:
	echo clean-misc
	-rm misc/*.o


