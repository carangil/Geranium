ZHOME=..
LIBNAME=zmem
include $(ZHOME)/includes/zmake.inc


CFILES=$(wildcard *.c)
OFILES=$(CFILES:%.c=%.o)

$(OBJDIR)/lib$(LIBNAME).a: $(OFILES) *.h *.c
	ar rvcs $@ $(OFILES)
	$(COPY) *.h $(INCDIR)

#rebuild o files if any c, h, files , or if the incdir was remade
%.o: %.c *.h  $(INCDIR)/.made
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm $(OBJDIR)/lib$(LIBNAME).a




