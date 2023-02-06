/*
AcessOS 64 - Stage 3
Linker Script
LINK.LD
*/

OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

SECTIONS
{
	. = 0x100000;
	.text :
	{
		code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.data :
	{
		data = .;
		*(.data)	
		. = ALIGN(4096);
	}

	.bss :
	{
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	end = .;
	_kEnd = .;
	_kEndPA = (. + 0xFFF)&0xFFFFF000;

	/DISCARD/ :
	{
		*(.comment)
	}
}
