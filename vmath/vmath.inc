Hvmath=vmath/vmath.h

o_vmath=objs/vmath.o

$(o_vmath): vmath/vmath.c   $(Hvmath)
	$(CC) $(CFLAGS) -O3 -c -o objs/vmath.o vmath/vmath.c

vmath: $(o_vmath)


clean-vmath: 
	-rm $(o_vmath)

