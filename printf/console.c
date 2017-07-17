#include "printf.h"

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

void serial_printf(const char *fmt, ...)
{
	va_list	args;
	unsigned int	i;
	char	printbuffer[CFG_PBSIZE];

	va_start(args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	i = vsprintf(printbuffer, fmt, args);
	va_end(args);

	serial_puts(printbuffer);
}

int	getc(void)
{
    /* Send directly to the handler */
    return serial_getc();
}

int	tstc(void)
{
    /* Send directly to the handler */
    return serial_tstc();
}

void	putc(const char c)
{
	/* Send directly to the handler */
	serial_putc(c);
}

void	puts(const char *s)
{
	/* Send directly to the handler */
	serial_puts(s);
}

void printf(const char *fmt, ...)
{
	va_list	args;
	unsigned int	i;
	char	printbuffer[CFG_PBSIZE];

	va_start(args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	i = vsprintf(printbuffer, fmt, args);
	va_end(args);

	/* Print the string */
	puts (printbuffer);
}

/* test if ctrl-c was pressed */
static int ctrlc_disabled = 0;		/* see disable_ctrl() */
static int ctrlc_was_pressed = 0;
int ctrlc(void)
{
	if (!ctrlc_disabled) {
		if (tstc()) {
			switch (getc()) {
			case 0x03:	/* ^C - Control C */
				ctrlc_was_pressed = 1;
				return 1;
			default:
				break;
			}
		}
	}
	return 0;
}

/* pass 1 to disable ctrlc() checking, 0 to enable.
 * returns previous state
 */
int disable_ctrlc(int disable)
{
	int prev = ctrlc_disabled;	/* save previous state */
	ctrlc_disabled = disable;
	return prev;
}

int had_ctrlc(void)
{
	return ctrlc_was_pressed;
}

void clear_ctrlc(void)
{
	ctrlc_was_pressed = 0;
}

int strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

int memcmp(const void * cs,const void * ct, size_t count)
{
	const unsigned char *su1, *su2;
	signed char res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

void * memcpy(void * dest,const void *src, size_t count)
{
	if (((((int)dest) | ((int)src) | ((int) count))&3)==0) {
		/*fast double word move*/
		unsigned int *d = (unsigned int *) dest, *s = (unsigned int *) src;
		while (count) {
			*d++ = *s++;
			count -= 4;
		}
	} else {
		/*slow byte move*/
		char *d = (char *) dest, *s = (char *) src;
		while (count--)
			*d++ = *s++;
	}
	return dest;
}

