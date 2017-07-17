#include <stdarg.h>

#include "config_sulu.h"

/* console.c */
/* serial stuff */
void	serial_printf (const char *fmt, ...);
int     strlen(const char * s);
int	strcmp(const char * cs, const char * ct);
char *  strcpy(char * dest,const char *src);

int      memcmp(const void * cs,const void * ct, size_t count);
void *   memcpy(void * dest,const void *src, size_t count);

/* stdin */
int	getc(void);
int	tstc(void);

/* stdout */
void	putc(const char c);
void	puts(const char *s);
void	printf(const char *fmt, ...);

/* ctrlc() */
int	ctrlc		(void);
int	had_ctrlc 	(void);
void	clear_ctrlc	(void);
int	disable_ctrlc	(int);


/* vsprintf.c */
unsigned long	simple_strtoul	(const char *cp,char **endp,unsigned int base);
long	        simple_strtol	(const char *cp,char **endp,unsigned int base);
int		is_numeric	(const char *c);

//void	        panic		(const char *fmt, ...);
int		sprintf		(char * buf, const char *fmt, ...);
int 	        vsprintf	(char *buf, const char *fmt, va_list args);

