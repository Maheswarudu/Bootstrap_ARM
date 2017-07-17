TEXT_BASE  = 0x00010000	# Starting address of code in SRAM

TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
export	TOPDIR

SUBDIRS	 =  printf

CROSS_COMPILE = arm-linux-gnueabihf-

PLATFORM_RELFLAGS = -fno-strict-aliasing  -fno-common -mhard-float -fno-stack-protector -O2
PLATFORM_CPPFLAGS = -march=armv7

OPTFLAGS = -O3 -fomit-frame-pointer 

CPPFLAGS = $(OPTFLAGS) $(PLATFORM_RELFLAGS)  -I$(TOPDIR) -I$(TOPDIR)/printf -fno-builtin -pipe $(PLATFORM_CPPFLAGS) 
CFLAGS   = $(CPPFLAGS) -g -c		# C files compile only;
SFLAGS	 = -g -o
LDFLAGS  = -nostdlib -Bstatic -T bootstrap.lds -Ttext $(TEXT_BASE)
LIBGCC	 = /opt/arm-none-eabi-gcc/lib/gcc/arm-none-eabi/4.8.4/libgcc.a

AS      = $(CROSS_COMPILE)as
AR	= $(CROSS_COMPILE)ar
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy 

OBJS    := $(shell ls *.c)
OBJS    := $(OBJS:.c=.o)
SOBJS   := $(shell ls *.S)
SOBJS   := $(SOBJS:.S=.o)

export  CROSS_COMPILE PLATFORM_RELFLAGS PLATFORM_CPPFLAGS OPTFLAGS CPPFLAGS CFLAGS LDFLAGS \
	LDFLAGS LIBGCC AS AR LD CC OBJCOPY OBJS SOBJS MAKE 

OBJS += printf/libprintf.a

all:    clean bootstrap.hex bootstrap.bin bootstrap.axf bootstrap.elf

bootstrap.hex:    bootstrap
	$(OBJCOPY) -O ihex bootstrap bootstrap.hex

bootstrap.bin:    bootstrap
	$(OBJCOPY) -O binary bootstrap bootstrap.bin

bootstrap.axf:  subdirs $(OBJS) $(SOBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SOBJS) $(LIBGCC) -Map bootstrap.map -o bootstrap.axf

bootstrap.elf:  subdirs $(OBJS) $(SOBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SOBJS) $(LIBGCC) -Map bootstrap.map -o bootstrap.elf

bootstrap:  subdirs $(OBJS) $(SOBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SOBJS) $(LIBGCC) -Map bootstrap.map -o bootstrap

subdirs:
	@for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir || exit 1 ; done

clean : 
	rm -f *.o
	rm -f printf/*.o
	rm -f printf/*.a
	rm -f bootstrap bootstrap.elf bootstrap.axf bootstrap.bin bootstrap.hex bootstrap.map

%.o:    %.S
	$(AS) $(SFLAGS) $@ $<
%.o:    %.c
	$(CC) $(CFLAGS) $<

