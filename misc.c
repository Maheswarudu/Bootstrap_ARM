#include "types.h"
#include "config.h"

void * memset(void * s, char c, size_t count)
{
	char *xs = (char *) s;

	while (count--)
		*xs++ = c;

	return s;
}

/* To Convert string into an integer */

int atoi(char *str)
{
    char *p;
    int r=0;
    for (p = str; *p != '\0'; p++)
    {
      r = r*10 + (*p - '0');
    }
        return r;
}


unsigned long convert_to_hex(char *addr)
{
  int i,len;
  unsigned long result = 0;

  if(*addr == '0')
  {
    addr++;
    if(*addr == 'x')
     addr++;
  }

  len = strlen(addr);
  for(i = 0;i < len;i++)
  {
   if(!((*addr >= 48 && *addr <=57) || (*addr >= 97 && *addr <= 102) || (*addr >= 65 && *addr <= 70)))
   {
     printf(" Invalid address \n");
     return 0;
   }
   if(*addr >= 48 && *addr <=57)
     result = result * 16 + *addr - '0';
   else
   {
     if(*addr >= 97 && *addr <= 102) 
       result = result * 16 + *addr + 10 - 'a';
     if(*addr >= 65 && *addr <= 70)
       result = result * 16 + *addr + 10 - 'A';
   }
  addr++;
  }
  return result;
}

static void pretty_print_size(ulong size)
{
    if (size > 0x100000)
      printf("%ld MB", size / 0x100000);
    else
      printf("%ld KB", size / 0x400);

}

void display_flash_size(unsigned long size)
{
    printf("Flash size       : ");
    pretty_print_size(size);
    printf("\n");
    
}


/* Timer functions */
unsigned long get_timer (ulong base)
{
    ulong now;
    //now = rd_d(TIMER_REG + 0x0C);
    if (now < base) {
		return ((ulong)(0-base)) + now;
    } else {
		return now - base;
    }
}

void reset_timer(void)
{
  //wr_d(TIMER_REG , 0x00);  /* Stop timer */
  //wr_d(TIMER_REG + 0x0C , (unsigned int) (0)); /* Write timer value */
  //wr_d(TIMER_REG , 0x03);  /* Start timer */
}

ulong get_timer_val(void)
{
    ulong val;
    //val = rd_d(TIMER_REG + 0x0C);
    return val;
}

#define TICKS_PER_USEC  50
#define TICKS_PER_MSEC  50000
#define TICKS_PER_SEC   50000000

/* delay in seconds */
void delays(ulong sec)
{
  ulong val;
  reset_timer();
  while(1)
  {
    val = get_timer_val();
    if(val > (sec*TICKS_PER_SEC))
        break;
  }
}

/* delay in milli seconds */
void delayms(ulong msec)
{
  ulong val;
  reset_timer();
  while(1)
  {
    val = get_timer_val();
    if(val > (msec*TICKS_PER_MSEC))
        break;
  }
}


/* delay in micro seconds */
void delayus(ulong usec)
{
  ulong val;
  reset_timer();
  while(1)
  {
    val = get_timer_val();
    if(val > (usec*TICKS_PER_USEC))
        break;
  }
}


int abortboot(int bootdelay)
{
   int abort = 0;

   printf("Hit any key to stop autoboot: %2d ", bootdelay);

   while (bootdelay > 0) 
   {
     int i;
    
     --bootdelay;
     /* delay 100 * 10ms */
     for (i=0; !abort && i<100; ++i) 
     {
       if (tstc()) {	/* we got a key press	*/
           abort  = 1;	/* don't auto boot	*/
	    bootdelay = 0;	/* no more delay	*/
	    (void) getc();  /* consume input	*/
	    break;
	 }
	 delayms (10);
       }
       printf ("\b\b\b%2d ", bootdelay);
     }
     putc ('\n');
     //printf("\n");
    return abort;
}
