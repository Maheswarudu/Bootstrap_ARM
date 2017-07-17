
#**************************************************************************
#Author  : Sunil Goutham
#Project : Retarget
#Date    : 08/08/2005
#$Id: Makefile,v 1.1 2007/10/24 04:03:54 sunilg Exp $
#**************************************************************************

LIB	= libprintf.a

OBJS  =  ctype.o console.o vsprintf.o

all:  $(OBJS) $(LIB) 


$(LIB):	$(OBJS)
	$(AR) crv $@ $(OBJS)

clean : 
	rm -f *.o


%.o:    %.S
	$(AS) -o $@ $<
%.o:    %.c
	$(CC) $(CFLAGS) $<

