; AcessOS 64
; VGA Text Mode Driver
; VGATEXT.ASM
[BITS 64]
%include "include/macros.inc.asm"

; === Exports ===
[global _Screen_Clear]
[global _Screen_Putc]
[global _Screen_Puts]

; === Variables ===
[section .data]
_Screen_Pos:	dw	0
_Screen_Attrib:	db	0x07

; === Code ===
[section .text]
; void Screen_Clear(char c)
; - Wipe Screen
_Screen_Clear:
	INIT_FRAME 0, {}
	cld
	mov rcx, 80*25/4
	mov rdi, 0xFFFF8000000B8000
	mov rax, 0x0
	rep stosq
	CLOSE_FRAME {}
	ret

; void Screen_Putc(char c)
; - Print a character to screen
_Screen_Putc:
	INIT_FRAME 0, {}
	mov ax, di	; Get Argument 1
	
	mov r8, _Screen_Pos
	; Get Screen Position
	xor rdi, rdi
	mov di, WORD [r8]
	; Check for printable characters
	cmp al, 0x20
	jl	.special
	; Set Attribute
	mov rdx, _Screen_Attrib
	mov ah, BYTE [rdx]
	; Print Character
	mov rdx, 0xFFFF8000000B8000
	mov WORD [rdx+rdi*2], ax
	; Increment Position
	inc di

.updatepos:
	mov WORD [r8], di
	
	; Check for scroll
	cmp di, 80*25
	jl .ret
	; Scroll
	cld
	mov rsi, 0xFFFF8000000B8000+80*2
	mov rdi, 0xFFFF8000000B8000
	mov rcx, 80*24*2/8
	rep movsq
	
	; Shift pointer back
	sub di, 80
	mov WORD [r8], di
	
.ret:
	CLOSE_FRAME {}
	ret
; Special Charaters
.special:
	cmp	al, 13	;\r
	jz	.carrage_return
	cmp al, 10	;\n
	jz	.newline
	jmp	.ret
; Newline
.newline:
	add di, 80
; Carrage Return
.carrage_return:
	mov ax, di
	mov cx, 80
	div cx
	mul cx
	mov di, ax
	jmp .updatepos
	
; void Screen_Puts()
; - Print a string
_Screen_Puts:
	INIT_FRAME 0, {}
.loop:
	xor rax, rax
	mov al, BYTE [rdi]
	test al, al
	jz .out
	CCALL	_Screen_Putc, rax
	inc rsi
	jmp	.loop
.out:
	CLOSE_FRAME {}
	ret

; void Screen_SetPos(int x, int y)
; - Set Screen position
_Screen_SetPos:
	INIT_FRAME 0, {}
	mov rax, rsi	;Argument 2
	mov dx, 80
	mul dx
	add rax, rdi
	mov rdx, _Screen_Pos
	mov WORD [rdx], ax
	CLOSE_FRAME {}
	ret
