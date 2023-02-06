; AcessOS 64
; Boot Strapper
; START32.ASM
[BITS 64]
KERNEL_BASE equ 0xFFFF800000000000

; Exports
[global _start64]
[global _kstack_temp]
[global _kstack_int]

; Imports
[extern _kmain]

; Code
[section .text]
_start64:
	cli				;Interupts are disabled because no IDT has been set up
	
	mov rsp, QWORD _kstack_temp	; Set Stack to temporary location
	
	; Set TSS Entry
	mov rax, _gTSS+0xFFFF800000000000
	mov WORD [QWORD _gpGDT.tssent+2], ax
	shr rax, 16
	mov BYTE [QWORD _gpGDT.tssent+4], al
	shr rax, 8
	mov BYTE [QWORD _gpGDT.tssent+7], al
	shr rax, 8
	mov DWORD [QWORD _gpGDT.tssent+8], eax
	
	; Reload GDT
	xchg bx, bx
	mov rax, QWORD _gpGDTP
	lgdt [rax]
	;lgdt [QWORD _gpGDTP]
	;db 0x0F, 0x01, 0x10
	;dq _gpGDTP
	
	call _kmain	; Call Kernel main
	
	cli		; Halt System
	hlt
	jmp $

[section .data]
[global _gpGDT]
[global _gpGDT.tssent]
_gpGDT:
	; 00000000, 0x00001F00
	dd 0x00000000, 0x00000000	; NULL Entry
	dd 0, 0
	dd 0x0000FFFF, 0x00CF9800	; Code 32
	;dd 0, 0
	dd 0x0000FFFF, 0x00AF9800	; Code 64
	dd 0x0000FFFF, 0x008F9200	; Data 32
	dd 0, 0
	dd 0x0000FFFF, 0x00AF9800	; Code 64
	dd 0, 0
	dd 0x0000FFFF, 0x008F9200	; Data 64
	dd 0, 0
.tssent:
	dw	14*8-1, 0x0000, 0x8900, 0x0000
	dd	0x00000000, 0x00000000
_gpGDTP:
	dw $-_gpGDT-1	; Size/Limit
	dq _gpGDT - 0xFFFF800000000000	; Canoical Address (No Sign Extend)

_gTSS:
	dd	0
	dq	_kstack_int	; RSP0
	dq	_kstack_int	; RSP1
	dq	_kstack_int	; RSP2
	dq	0
	dq	0,0,0,0,0,0,0,0	; IST0-7
	dq	0
	dd	0	; Resvd + IO Map Base
	
; Uninitialised Data
[section .bss]
	resb	0x8000
_kstack_temp:
	resb	0x8000
_kstack_int:
