#include "config.h"
#include "config_sulu.h"
#include "memorymap.h"

/* Using UART port 1 */
#define SULU_UART1 SULU_SIF_UART_AHB_BASE(1)

/* uart Register offsets */
#define SULU_UART_RX_DATA                   0xe00
#define SULU_UART_TX_DATA                   0xe80
#define SULU_UART_RX_FLUSH                  0xf00
#define SULU_UART_TX_FLUSH                  0xf04

#define SULU_UART_RX_FIFO_FILL_LEVEL_THD    0xf10
#define SULU_UART_TX_FIFO_EMPTY_LEVEL_THD   0xf14

#define SULU_UART_RX_FILL_LEVEL             0xf18
#define SULU_UART_TX_FREE_LEVEL             0xf1c

#define SULU_UART_FIFO_TIMER                0xf20
#define SULU_UART_FIFO_TIMER_THD            0xf24

#define SULU_UART_FIFO_CFG_SET              0xf30
#define SULU_UART_FIFO_CFG_CLR              0xf34

#define SULU_UART_FIFO_STAT_SET             0xf40
#define SULU_UART_FIFO_STAT_CLR             0xf44
#define SULU_UART_FIFO_IRQ_MASK_SET         0xf50
#define SULU_UART_FIFO_IRQ_MASK_CLR         0xf54
    #define SULU_UART_FIFO_RX_TIMEOUT            0x100
    #define SULU_UART_FIFO_RX_NEAR_FULL          0x200
    #define SULU_UART_FIFO_TX_ALMOST_EMPTY       0x400

#if 0
void serial_setbrg(int baudrate)
{
    int baud_rate_divisor;
	
    baud_rate_divisor = (SERIAL_PORT_CLK)/(baudrate);
	
    wr_b(__UART_REGS_LCR, LCR_DLEN);                 /* Enable DLL/DLH registers */
    wr_b(__UART_REGS_DLL, baud_rate_divisor & 0xFF); /* Set baud rate into DLL & DLH */
    wr_b(__UART_REGS_DLH, baud_rate_divisor >>8);
    wr_b(__UART_REGS_LCR, LCR_8D | LCR_1S );         /* Serial port set to 8-N-1 ; Disable DLL/DLH registers */ 
}
#endif
              
void serial_init()
{
#if 0
    int baud_rate_divisor;
    int sdram_freq;

    baud_rate_divisor = 2;  /* i.e ((50MHz)/16/27)/BAUD_RATE_DEFAULT */  /* Expected baudrate is 57600 */

    wr_b(__UART_REGS_LCR, LCR_DLEN);                 /* Enable DLL/DLH registers */
    wr_b(__UART_REGS_DLL, baud_rate_divisor & 0xFF); /* Set baud rate into DLL & DLH */
    wr_b(__UART_REGS_DLH, baud_rate_divisor >>8);
    wr_b(__UART_REGS_LCR, LCR_8D | LCR_1S );         /* Serial port set to 8-N-1 ; Disable DLL/DLH registers *///data length 8 bit and no stop bit 
    wr_b(__UART_REGS_IER, 0);                        /* Disable all interrupts */
    wr_b(__UART_REGS_FCR, 0);                        /* Disable FIFO's */
#endif
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is sucessful, the character read is
 * written into its argument c.
 */
int serial_getc(void)
{
    //while (rd_b(SULU_UART_RX_FILL_LEVEL) == 0);//polls till data not received
  
    return rd_b(SULU_UART1+SULU_UART_RX_DATA);//reads receive buffer register
}

/*
 * Output a single byte to the serial port.
 */
void serial_putc(const char c)
{
	while (rd_b(SULU_UART_TX_FREE_LEVEL) != 0);//polls till data not received

	wr_b((SULU_UART1+SULU_UART_TX_DATA),c);
	
#if 0
    while ((rd_b(__UART_REGS_LSR) & LSR_Tx_Empty) == 0);//polls on transmit holding register
    wr_b(__UART_REGS_THR, c);//wites transmit holding register

    if(c == '\n')
    serial_putc('\r');
#endif
}

/*
 * Test whether a character is in the RX buffer
 */
int serial_tstc(void)
{
    return rd_b(SULU_UART_RX_FILL_LEVEL)?1:0; //returns 1 if data recieved otherwise 0
}

/*
 * Implementation of printf  
 */

void serial_trace(char *str)
{
#if 0
    int i=0;

    while(str[i] != '\0')
     {
       serial_putc(str[i]);
       i++;
     }
#endif
}
