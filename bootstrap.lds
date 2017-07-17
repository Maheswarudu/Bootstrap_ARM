OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	/* Start of text section. */
	. = 0x00010000;

	. = ALIGN(4);
	.text	:
	{
		start.o		(.text)
		*(.text)
	}

	/* Read only data section. */	
	. = ALIGN(4);
	.rodata	:
	{
		*(.rodata)
	}

	/* Data section. */
	. = ALIGN(4);
	.data	:
	{
		*(.data)
	}

	. = ALIGN(4);
	.got	:
	{
		*(.got)
	}

	/* BSS section. */
	. = ALIGN(4);
	bootstrap_end = .;
	.bss	:
	{
		*(.bss)
	}
	bootstrap_bss_end = .;
}
