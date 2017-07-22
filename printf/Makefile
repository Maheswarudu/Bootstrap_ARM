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

