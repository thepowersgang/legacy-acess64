; AcessOS 64
; Kernel Core
; KMAIN.ASM
[BITS 64]
%include "include/macros.inc.asm"

; === Exports ===
[global _kmain]

; === Imports ===
[extern _Screen_Clear]
[extern _Screen_Puts]
[extern _MM_Install]
[extern _VFS_Install]
[extern _DevFS_Install]

; === Code ===
[section .text]
; void kmain();
; - Kernel Entrypoint
_kmain:
	; Clear Screen
	call _Screen_Clear
	
	xchg bx, bx
	
	mov rax, _Acess_Header
	push rax
	call _Screen_Puts
	add rsp, 8
	
	mov rdi, 0x400000
	call _MM_Install
	
	call _VFS_Install
	call _DevFS_Install
	
	ret

; === Constants ===
[section .rodata]
_Acess_Header:
	db "Acess64 Version 1",13
	db 0
	