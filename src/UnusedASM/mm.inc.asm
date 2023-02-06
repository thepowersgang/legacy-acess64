; AcessOS 64
; Memory Manager - HEADER
; MM.INC.ASM
[BITS 64]


; NOTES::::
; 0xFFFF 80?? ???? ????
; ...00 0000 0000 - Kernel code/data (4Gb)
; ...01 0000 0000 - Modules (12Gb)
; ...04 0000 0000 - Kernel Heap (8Gb)
; ...06 0000 0000 - VFS File Handles (4 Gb)
; ...08 0000 0000 - Kernel Stack Minimum (32 Gb)
; ...10 0000 0000 - Kernel Stack Maximum (Thread 0 Initial RSP)
; ...10 0000 0000 - Hardware Mappings (64 Gb)
; ...20 0000 0000 - Fractal Mappings (2 Gb, 1 Gb Each)
; ...20 8000 0000 - Page Directories (62Gb, ~infinite entries)
; ...30 0000 0000 - Unallocated (Until end of address space)
; 1 PML4 Entry = 0x0000 0080 0000 0000 (512GiB)
; 1 PDP Entry  = 0x0000 0000 4000 0000 (1GiB)
; 1 PD Entry   = 0x0000 0000 0020 0000 (2MiB)
; 1 PT Entry   = 0x0000 0000 0000 1000 (4KiB)

KERNEL_BASE_ADDRESS	equ	0xFFFF800000000000
 MM_OFS_CODE	equ	0x0000000000
 MM_OFS_MODULES	equ	0x0100000000
 MM_OFS_HEAP	equ	0x0400000000