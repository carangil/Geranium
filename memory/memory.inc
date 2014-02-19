Hmemory=memory/ram.h  structures/linkedlist.h

o_mem=objs/memory.o

$(o_mem): memory/ram.c   $(Hmemory)
	$(CC) $(CFLAGS) -c -o objs/memory.o memory/ram.c 

memory: $(o_mem)


clean-memory: 
	-rm $(o_mem)

