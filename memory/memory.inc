Hmemory=memory/zmem.h
Hmemorydeps+=$(INCDIR)/zthread.h $(INCDIR)/zlist.h 

#List all o files here
.memory-objs: memory/ram.o
	touch .memory-objs

memory/%.o: memory/%.c  $(Hmemory) $(Hmemorydeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(INCDIR)/%.h: memory/%.h
	$(COPY) $< $(INCDIR)


$(INCDIR)/%.h: structures/%.h
	$(COPY) $< $(INCDIR)


clean-memory:
	echo clean-memory
	-rm memory/*.o

