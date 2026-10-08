ZHOME=..
LIBNAME=zbitmap
include $(ZHOME)/includes/zmake.inc


CFILES=$(wildcard *.c)
OFILES=$(CFILES:%.c=%.o)

$(OBJDIR)/lib$(LIBNAME).a: $(OFILES) *.h *.c
	ar rvcs $@ $(OFILES)
	$(COPY) *.h $(INCDIR)


%.o: %.c *.h  $(INCDIR)/.made
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm $(OBJDIR)/lib$(LIBNAME).a




