; AcessOS 64
; Boot Strapper
; START32.ASM
[BITS 32]

KERNEL_BASE equ 0xFFFF800000000000

; Exports
[global _gpGDT32]
[global _gpGDTP32]
[global _gTSS]
[global _start32]
[global _gpInitial_PML4]
[global _gpInitial_PDP]
[global _gpInitial_PD]
[global _gpInitial_PT1]
[global _gpInitial_PT2]
[global _giMultibootMagic]
[global _gpMultibootStruct]

; Imports
[extern _start64]

; Code
[section .text]
_start32:
	mov DWORD [_giMultibootMagic], eax
	mov DWORD [_gpMultibootStruct], ebx

	; Check if the CPU is 64 bits
	mov eax, 0x80000001
	cpuid
	test edx, 0x20000000	;1<<29
	jz _non64bit

	; Enable PAE + PGE
	mov eax, cr4
	or eax, 0x20|0x80
	mov cr4, eax
	
	; Set Page Dir Ptr
	mov eax, _gpInitial_PML4
	mov cr3, eax
	
	; Enable Long Mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 0x00000100
	wrmsr
	
	; Enable Paging
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	
	; Load 64-Bit GDT
	lgdt [_gpGDTP32]
	mov ax, 0x20
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	
	; Jump to 64 Bit Segment
	jmp 0x18:_start64 - KERNEL_BASE

_non64bit:
	mov eax, 0x606F604E ;'No'
	mov [0xB8000], eax
	mov eax, 0x60206074 ;'t '
	mov [0xB8004], eax
	mov eax, 0x60346036 ;'64'
	mov [0xB8008], eax
	mov eax, 0x60426020 ;' B'
	mov [0xB800C], eax
	mov eax, 0x60746069 ;'it'
	mov [0xB8010], eax
	cli
.hlt:
	hlt
	jmp .hlt
	
[section .data]
_giMultibootMagic:	dd	0, 0
_gpMultibootStruct:	dd	0, 0xFFFF8000

ALIGN 0x8
_gpGDT32:
	dd 0x00000000, 0x00000000	; NULL Entry
	dd 0x0000FFFF, 0x00CF9800	; Code 32
	dd 0x0000FFFF, 0x008F9200	; Data 32
	dd 0x0000FFFF, 0x00AF9800	; Code 64
	dd 0x0000FFFF, 0x008F9200	; Data 64
	
_gpGDTP32:
	dw $-_gpGDT32-1	; Size/Limit
	dq _gpGDT32+0xFFFF800000000000		; GDT Base
	
ALIGN 0x1000
_gpInitial_PML4:	; Page Map Level 4 (Ref 256TiB)
	dq	_gpInitial_PDP + 3	; Identity Map Low 4Mb
	times 255 dq 0			;             0x00000080 00000000
	dq	_gpInitial_PDP + 3	; Also Map to 0xFFFF8000 00000000
	times 255 dq 0

_gpInitial_PDP:		; Page Directory Pointer Table (Ref 512Gb [0x8000000000]), 1 Ent = 0x40000000
	dq _gpInitial_PD + 3
	times 510 dq 0			;               
	dq _gpInitial_PML4 + 3	;Fractal Map at 0xFF C0000000, 0xFFFF80FF C0000000

_gpInitial_PD:		; Page Directory (Ref 1GiB)
	dq _gpInitial_PT1 + 3
	dq _gpInitial_PT2 + 3
	times 510 dq 0

_gpInitial_PT1:		; Page Table 1
	%assign i 0
	%rep 512
	dq	i*4096+3
	%assign i i+1
	%endrep

_gpInitial_PT2:		; Page Table 2
	%assign i 512
	%rep 512
	dq	i*4096+3
	%assign i i+1
	%endrep
