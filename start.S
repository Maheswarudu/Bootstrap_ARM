	.text
	.global _start

_start:
	/*
         * disable interrupts (FIQ and IRQ), also set the cpu to SVC32 mode,
         * except if in HYP mode already
         */
        mrs     r0, cpsr
        and     r1, r0, #0x1f           @ mask mode bits
        teq     r1, #0x1a               @ test for HYP mode
        bicne   r0, r0, #0x1f           @ clear all mode bits
        orrne   r0, r0, #0x13           @ set SVC mode
        orr     r0, r0, #0xc0           @ disable FIQ and IRQ
        msr     cpsr,r0


         /*
         * Setup CP15 registers (cache, MMU, TLBs). The I-cache is turned on unless
         * CONFIG_SYS_ICACHE_OFF is defined.
         */
        mov     r0, #0                  @ set up for MCR
        mcr     p15, 0, r0, c8, c7, 0   @ invalidate TLBs
        mcr     p15, 0, r0, c7, c5, 0   @ invalidate icache
        mcr     p15, 0, r0, c7, c5, 6   @ invalidate BP array
        mcr     p15, 0, r0, c7, c10, 4  @ DSB
        mcr     p15, 0, r0, c7, c5, 4   @ ISB

        /*
         * disable MMU stuff and caches
         */
        mrc     p15, 0, r0, c1, c0, 0
        bic     r0, r0, #0x00002000     @ clear bits 13 (--V-)
        bic     r0, r0, #0x00000007     @ clear bits 2:0 (-CAM)
        orr     r0, r0, #0x00000002     @ set bit 1 (--A-) Align
        orr     r0, r0, #0x00000800     @ set bit 11 (Z---) BTB
#define CONFIG_SYS_ICACHE_OFF
#ifdef CONFIG_SYS_ICACHE_OFF
        bic     r0, r0, #0x00001000     @ clear bit 12 (I) I-cache
#else
        orr     r0, r0, #0x00001000     @ set bit 12 (I) I-cache
#endif
        mcr     p15, 0, r0, c1, c0, 0
	#bl relocate
        bl sys_init                    @ Cortex A5 CPU initilization

.global _TEXT_BASE                     @ Code relocation
_TEXT_BASE: .word 0x00010000

.global _bootstrap_start
_bootstrap_start:   .word _start

.global _bootstrap_end
_bootstrap_end: .word bootstrap_end

.global _bootstrap_bss_end
_bootstrap_bss_end: .word bootstrap_bss_end

#_start_bootstrap:   .word start_bootstrap
