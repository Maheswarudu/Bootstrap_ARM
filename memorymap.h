#ifndef MEMORYMAP_H_
#define MEMORYMAP_H_

/* AHB ADRESSES */
#define SULU_UMR_AHB_BASE           0x00400000
#define SULU_WLAN_AHB_BASE          0x30000000
#define SULU_SDRAM_AHB_BASE         0x60000000

#define SULU_WARP_AHB_BASE(x)       ((x==0) ? 0xac000000 : 0xa8000000)

#define SULU_USB_AHB_BASE           0xb0000000
#define SULU_USB_AHB_END            0xb0FFFFFF
#define SULU_SIF_AHB_BASE           0xb1000000
  #define SULU_SIF_UART_AHB_BASE(x) (0xb1000000 + (x * 0x1000))
  #define SULU_SIF_SPI_AHB_BASE       0xb1002000
  #define SULU_SIF_NAND_AHB_BASE      0xb1003000
  #define SULU_SIF_SDIO_AHB_BASE      0xb1004000

#define SULU_NULL_AHB_BASE          0xb2000000
#define SULU_AV_AHB_BASE            0xb4000000
#define SULU_DMA_AHB_BASE           0xb5000000
#define SULU_HV_AHB_BASE            0xb8000000
#define SULU_SOL_AHB_BASE           0xd0000000


/* APB ADRESSES */
#define SULU_APB_BASE               0xe0000000
#define SULU_BOOT_APB_BASE          0xe0000000
#define SULU_RTC_APB_BASE           0xe0004000
#define SULU_OTP_APB_BASE           0xe0006000
#define SULU_TGEN_APB_BASE          0xe0007000
#define SULU_AV_APB_BASE(port)     (0xe0008000 + port*0x1000)   // port = 0,2,3,4 
#define SULU_AVCOMMON_APB_BASE      0xe0010000
#define SULU_AVCURAM_APB_BASE       0xe0011000
#define SULU_AVPTU_APB_BASE         0xe0012000
#define SULU_SWMTX_APB_BASE         0xe0013000
#define SULU_WARPDEBUG_APB_BASE(x) (0xe0014000 + x*0x1000)      // x = 0(SecureWarp), 1(SolWarp)
#define SULU_ETHERNET_APB_BASE      0xe0030000
#define SULU_UMR_APB_BASE           0xe0018000
#define SULU_SWITCH_APB_BASE        0xe0019000
#define SULU_CTR_APB_BASE           0xe001b000
#define SULU_I2C_APB_BASE	    0xe001c000
#define SULU_SDRAM_APB_BASE         0xe001d000
#define SULU_DAC_APB_BASE           0xe001e000
#define SULU_HV_APB_BASE            0xe0020000
#define SULU_HVCOMPTU_APB_BASE      0xe0024000
#define SULU_IRQ_APB_BASE           0xffff0000

#endif /*MEMORYMAP_H_*/
