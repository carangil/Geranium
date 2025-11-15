Hmisc=$(INCDIR)/ztime.h $(INCDIR)/zrand.h $(INCDIR)/zbitmap.h $(INCDIR)/zwindow.h

Hmiscdeps+=

#List all o files here
.misc-objs: misc/tm.o misc/zrand.o misc/zbitmap.o
	touch .misc-objs

misc/%.o: misc/%.c  $(Hmisc) $(Hmiscdeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(INCDIR)/%.h: misc/%.h
	echo doing copy $< $(INCDIR)
	$(COPY) $< $(INCDIR)


#$(INCDIR)/%.h: misc/%.h
#	$(COPY) $< $(INCDIR)


clean-misc:
	echo clean-misc
	-rm misc/*.o


