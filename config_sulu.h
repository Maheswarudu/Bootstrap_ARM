#ifndef __CONFIG_SULU_H
#define __CONFIG_SULU_H
#include "types.h"

extern ulong _bootstrap_start;                 /* code start */
extern ulong _bootstrap_end;                   /* code end */ 
extern ulong _bootstrap_bss_end;               /* BSS end */ 
extern ulong IRQ_STACK_START;                   /* top of IRQ stack */
extern ulong FIQ_STACK_START;                   /* top of FIQ stack */
extern ulong _bootstrap_real_end;              /* first usable RAM address */

#define DDR_MEMORY
/* Stack sizes */
#define CONFIG_STACKSIZE	(64*1024)	/* regular stack */
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */

/* Printf options  */
#define	CFG_LONGHELP				/* undef to save memory		*/
#define	CFG_PROMPT		"SULU# "	/* Monitor Command Prompt	*/
#define	CFG_CBSIZE		1024		/* Console I/O Buffer Size	*/
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16) /* Print Buffer Size */

/***************************************
 System configuration registers 
****************************************/
#define CONF_REG_BASE       0x40000000

#define SIC_OFFSET          0x00264000
#define GPIO_OFFSET         0x00280000
#define EEPIO_OFFSET        0x000D4000
#define UART_OFFSET         0x00290000
#define SYSCONF_OFFSET	    0x0026c000

#define GPIO_BASE           (CONF_REG_BASE + GPIO_OFFSET)
#define EEPIO_BASE          (CONF_REG_BASE + EEPIO_OFFSET)
#define UART_BASE           (CONF_REG_BASE + UART_OFFSET)
#define SIC_BASE            (CONF_REG_BASE + SIC_OFFSET)
#define SYSCONF_BASE	    (CONF_REG_BASE + SYSCONF_OFFSET)

/***************************************
 Macros for input/output operations 
****************************************/
#define wr_d(a,b) *((volatile unsigned int *) (a)) = ((unsigned int) (b))
#define rd_d(a) *((volatile unsigned int *) (a))
#define wr_w(a,b) *((volatile unsigned short *) (a)) = ((unsigned short) (b))
#define rd_w(a) *((volatile unsigned short *) (a))
#define wr_b(a,b) *((volatile unsigned char *) (a)) = ((unsigned char) (b))
#define rd_b(a) *((volatile unsigned char *) (a))

/***************************************
 Macros related to UART
****************************************/
#define BAUD_RATE_DEFAULT 115200
#define SERIAL_PORT_CLK   (50000000/16)

#define __UART_REGS_RBR (UART_BASE + 0x00)
#define __UART_REGS_THR (UART_BASE + 0x00)
#define __UART_REGS_DLL (UART_BASE + 0x00)
#define __UART_REGS_DLH (UART_BASE + 0x04)
#define __UART_REGS_IER (UART_BASE + 0x04)
#define __UART_REGS_IIR (UART_BASE + 0x08)
#define __UART_REGS_FCR (UART_BASE + 0x08)
#define __UART_REGS_LCR (UART_BASE + 0x0C)
#define __UART_REGS_MCR (UART_BASE + 0x10)
#define __UART_REGS_LSR (UART_BASE + 0x14)
#define __UART_REGS_MSR (UART_BASE + 0x18)
#define __UART_REGS_SCR (UART_BASE + 0x1C)

#define LSR_Data_Ready        0x01
#define LSR_Tx_Empty          0x20
#define LSR_Transmitter_Empty 0x40

#define LCR_8D       0x03
#define LCR_7D       0x02
#define LCR_6D       0x01
#define LCR_5D       0x00
#define LCR_1S       0x00
#define LCR_2S       0x04
#define LCR_PE_ODD   0x08
#define LCR_PE_EVEN  0x18
#define LCR_BREAK    0x40
#define LCR_DLEN     0x80

/***************************************
 Macros related to interrupt handling
****************************************/
#define __clear_irq(x)     *((volatile unsigned int *) (SIC_BASE + 0x00)) = (((unsigned int) (1))<<(x))
#define __clear_fiq(x)     __clear_irq(x)
#define __mask_irq(x)      *((volatile unsigned int *) (SIC_BASE + 0x20)) = (__read_irq_mask) & ~(((unsigned int) (1))<<(x))
#define __unmask_irq(x)    *((volatile unsigned int *) (SIC_BASE + 0x20)) = (__read_irq_mask) | (((unsigned int) (1))<<(x))
#define __set_irq_mask(x)  *((volatile unsigned int *) (SIC_BASE + 0x20)) = ((unsigned int) (x))
#define __read_irq_mask    *((volatile unsigned int *) (SIC_BASE + 0x20))
#define __read_irq_status  *((volatile unsigned int *) (SIC_BASE + 0x40))
#define __set_fiq_mask(x)  *((volatile unsigned int *) (SIC_BASE + 0x24)) = ((unsigned int) (x))
#define __read_fiq_mask    *((volatile unsigned int *) (SIC_BASE + 0x24))
#define __read_fiq_status  *((volatile unsigned int *) (SIC_BASE + 0x44))
#define __set_irq(x)     *((volatile unsigned int *) (SIC_BASE + 0x04)) = (((unsigned int) (1))<<(x))

/**************************************************
Flag to disable SPI support
***************************************************/
#undef ENABLE_SPI

//#define ETH_DEBUG

#endif	/* __CONFIG_SULU_H */
