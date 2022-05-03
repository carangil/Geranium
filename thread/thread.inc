Hthread=thread/zthread.h
Hthreaddeps=

#List all o files here

.thread-objs: thread/zthread.o
	touch .thread-objs

thread/%.o: thread/%.c  $(Hthread) $(Hthreaddeps)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(INCDIR)/%.h: thread/%.h
	$(COPY) $< $(INCDIR)

clean-thread:
	-rm thread/*.o

