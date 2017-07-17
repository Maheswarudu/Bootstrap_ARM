#Source and destination memory locations.
DDR_SOURCE_ADDR : .word 0x40000000
DDR_DESTINATION_ADDR : .word 0x40000400
DDR_DEST_END_ADDR : .word 0x40000800

.global relocate
relocate:
	ldr	r0, DDR_SOURCE_ADDR		@ r0 <- current position of code
	ldr	r1, DDR_DESTINATION_ADDR	@ r1 <- destination address
	ldr	r2, DDR_DEST_END_ADDR		@ r2 <- destination address
        mov	r11, #0x0			@ number of loops

copy_loop:
	add	r11, r11,#0x1		@ increment loop count
        ldmia	r0!, {r3-r10}           @ r0 = source address
        stmia	r1!, {r3-r10}           @ r1 = target address
        cmp	r1, r2                  @ r2 = source end address
        ble	copy_loop
	mov	pc, lr 
.end
