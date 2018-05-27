Hvmath=vmath/zmath.h

o_vmath=objs/zmath.o
src_vmath=vmath/vmath.c


$(o_vmath): $(src_vmath) $(Hvmath)
	echo DO $(CC)   $(CFLAGS)  -c -o $@ $< 
	$(CC)   $(CFLAGS)  -c -o $@ $< 

vmath: objs/vmath.o


clean-vmath: 
	-rm $(o_vmath)

