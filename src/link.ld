/*
AcessOS 64
Linker Script
LINK.LD
*/

KERNEL_LOW = 0x200000;
KERNEL_BASE = 0xFFFF800000000000;

OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start32)

SECTIONS
{
	. = KERNEL_LOW;	/* Low Memory Address = 2Mb */
	_gvKernelStart = .;
	
	.bootstrap : AT(ADDR(.bootstrap)) {
		start32.ao (.text)
		. = ALIGN(0x1000);
		start32.ao (.data)
	}
	
	. += KERNEL_BASE;
	. = ALIGN(0x1000);
	
	.text : AT(ADDR(.text) - KERNEL_BASE) {
		_code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.data : AT(ADDR(.data) - KERNEL_BASE) {
		_data = .;
		*(.data)	
		. = ALIGN(4096);
	}

	.ehframe : AT(ADDR(.ehframe) - KERNEL_BASE)	{
		_ehframe = .;
		*(.ehframe)
		. = ALIGN(4096);
	}

	.bss : AT(ADDR(.bss) - KERNEL_BASE)	{
		_bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	_end = .;
	_end_pa = . - KERNEL_BASE;
	_gvKernelEnd = (. - KERNEL_BASE + 0xFFF) & 0xFFFFF000;

	/DISCARD/ :
	{
		*(.comment)
	}
}
