CONFIG?=$(ZHOME)/debugconfig.inc

include $(CONFIG)

CC=gcc

INCDIR?=$(ZHOME)/build/includes
OBJDIR?=$(ZHOME)/build/objs

CFLAGS += -I. -I$(INCDIR)





