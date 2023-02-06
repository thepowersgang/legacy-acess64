; AcessOS 64
; Memory Manager
; MM.ASM
[BITS 64]
%include "include/macros.inc.asm"
%include "include/mm.inc.asm"

; === Exports ===
[global _MM_Install]

; === Imports ===
[extern _end_pa]

; === Variables ===
[section .data]
_MM_TotalPages:	dq	0
_MM_PhysBitmap:	dq	0
_MM_PhysSuperBitmap:	dq	_end_pa

; === Code ===
[section .text]
; void MM_Install(Uint totalMemory)
; - Initialise Memory Manager
; - ARG {totalMemory} - RAM size in bytes
_MM_Install:
	INIT_FRAME 0, {rbx}
	
	; --- Get Total number of physical pages
	mov rax, rdi	; Get First Argument
	shr rax, 12	; Total Pages
	mov QWORD [QWORD _MM_TotalPages], rax
	
	; --- Create Physical Memory Bitmap
	shr	rax, 3	; Page Bitmap byte count
	mov rbx, rax
	shr rbx, 6	; Get Superpage Byte count
	add rbx, rax	; Get Total size
	cmp rbx, 0x100000	; Compare with 1 Mb
	jg	.makebm_TotalAbove1Mb
	; Both bitmaps fit in unused 1Mb below kernel
	mov r8, 0x100000
	mov qword [QWORD _MM_PhysSuperBitmap], r8
	shr rax, 3
	add rax, 0x100000
	mov QWORD [QWORD _MM_PhysBitmap], rax
	jmp	.makebm_out
	; Total Byte count of both bitmaps combined is above 1Mb
.makebm_TotalAbove1Mb:
	; Put bitmaps above kernel
	mov r8, _end_pa
	mov QWORD [QWORD _MM_PhysSuperBitmap], r8
	shr rax, 3
	add rax, _end_pa
	mov QWORD [QWORD _MM_PhysBitmap], rax
.makebm_out:
	; --- END
	
	; --- Populate Bitmap
	; Get Pointer
	mov rdi, QWORD [QWORD _MM_PhysBitmap]
	mov rdx, QWORD [QWORD _MM_PhysSuperBitmap]
	shr rdx, 12-3
	add rdi, rdx
	; Fill Bitmap
	GET_ARG rbx, 1
	shr rbx, 15	; Bytes in Page bitmap
	mov rax, rbx
	shr rax, 6	; Bytes in superpage bitmap
	add rbx, rax
	shr rbx, 12	; Pages in page bitmap and superpage bitmap
	; Set Full QWords
	mov rcx, rbx
	shr rcx, 6
	mov rax, -1
	cld
	rep stosq
	; Set Partial QWords
	mov rcx, rbx
	and rcx, 0x3F
	mov rax, 1
.pbm_loop_qwords:
	or QWORD [rdi], rax
	shl rax, 1
	dec rcx
	jnz .pbm_loop_qwords
	; Set Super Page Bitmap
	mov rdi, QWORD [QWORD _MM_PhysSuperBitmap]
	mov rdx, rdi
	shr rdx, 12-3+6
	add rdi, rdx
	; Set Full QWords
	mov rcx, rbx
	shr rcx, 6
	mov rax, -1
	cld
	rep stosq
	; Set Partial QWords
	mov rcx, rbx
	and rcx, 0x3F
	mov rax, 1
.spbm_loop_qwords:
	or	QWORD [rdi], rax
	shl rax, 1
	dec rcx
	jnz .spbm_loop_qwords
	; --- END
	
	CLOSE_FRAME	{rbx}
	ret
