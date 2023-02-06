; Acess64
;  Decriptor Tables
;  DESCTABS.ASM
[BITS 64]

[section .data]
ALIGN 8
_gIDT:
	times	256	dd	0x00180000,0x00008F00,0x00000000,0x00000000
_gIDTPtr:
	dw	256 * 16 - 1	; Limit
	dq	_gIDT		; Base
message:	db "[IDT ] Installing IDT",10,0
	
[section .text]
[extern _Screen_Puts]
[global _IDT_Initialise]
_IDT_Initialise:
	push rdx
	push rcx
	
	mov rdi, QWORD message
	call _Screen_Puts
	
	; Load IDT
%macro SETISR	1
	mov rdx, QWORD _Isr%1
	mov rax, rdx
	and rax, 0xFFFF
	mov	WORD [QWORD _gIDT+ %1*16], ax
	mov rax, rdx
	shr rax, 16
	mov	WORD [QWORD _gIDT+ %1*16+6], ax
	mov rax, rdx
	shr rax, 32
	mov	DWORD [QWORD _gIDT+ %1*16+8], eax
%endmacro
	%assign	i	0
	%rep 32
	SETISR	i
	%assign i i+1
	%endrep
	
	SETISR	0xAC
	
	%assign	i	0xF0
	%rep 16
	SETISR	i
	%assign i i+1
	%endrep

	mov rax, QWORD _gIDTPtr
	lidt [rax]
	
	; Remap PIC
	mov dx, 0x20
	mov al, 0x11
	out dx, al	;	Init Command
    mov dx, 0x21
	mov al, 0xF0
	out dx, al	;	Offset (Start of IDT Range)
    mov al, 0x04
	out dx, al	;	IRQ connected to Slave (00000100b) = IRQ2
    mov al, 0x01
	out dx, al	;	Set Mode
    mov al, 0x00
	out dx, al	;	Set Mode
	
	mov dx, 0xA0
	mov al, 0x11
	out dx, al	;	Init Command
    mov dx, 0xA1
	mov al, 0xF8
	out dx, al	;	Offset (Start of IDT Range)
    mov al, 0x02
	out dx, al	;	IRQ Line connected to master
    mov al, 0x01
	out dx, al	;	Set Mode
    mov dl, 0x00
	out dx, al	;	Set Mode
	
	pop rcx
	pop rdx
	
	;sti
	
	; Debug
	;int 0	; Call divide by zero error handler
	
	ret

; ===============
; = Define ISRs =
; ===============
%macro	ISR_ERRNO	1
[global _Isr%1]
_Isr%1:
	cli
	push	%1
	jmp	ErrorCommon
%endmacro
%macro	ISR_NOERR	1
[global _Isr%1]
_Isr%1:
	xchg bx, bx
	cli
	push	0
	push	%1
	jmp	ErrorCommon
%endmacro

%macro DEF_INT	1
[global _Isr%1]
_Isr%1:
	xchg bx, bx
	push	0
	push	%1
	jmp	InterruptCommon
%endmacro

%macro DEF_IRQ	1
[global _Isr%1]
_Isr%1:
	xchg bx, bx
	push	0
	push	%1
	jmp	IRQCommon
%endmacro

ISR_NOERR	0;  0: Divide By Zero Exception
ISR_NOERR	1;  1: Debug Exception
ISR_NOERR	2;  2: Non Maskable Interrupt Exception
ISR_NOERR	3;  3: Int 3 Exception
ISR_NOERR	4;  4: INTO Exception
ISR_NOERR	5;  5: Out of Bounds Exception
ISR_NOERR	6;  6: Invalid Opcode Exception
ISR_NOERR	7;  7: Coprocessor Not Available Exception
ISR_ERRNO	8;  8: Double Fault Exception (With Error Code!)
ISR_NOERR	9;  9: Coprocessor Segment Overrun Exception
ISR_ERRNO	10; 10: Bad TSS Exception (With Error Code!)
ISR_ERRNO	11; 11: Segment Not Present Exception (With Error Code!)
ISR_ERRNO	12; 12: Stack Fault Exception (With Error Code!)
ISR_ERRNO	13; 13: General Protection Fault Exception (With Error Code!)
ISR_ERRNO	14; 14: Page Fault Exception (With Error Code!)
ISR_NOERR	15; 15: Reserved Exception
ISR_NOERR	16; 16: Floating Point Exception
ISR_NOERR	17; 17: Alignment Check Exception
ISR_NOERR	18; 18: Machine Check Exception
ISR_NOERR	19; 19: Reserved
ISR_NOERR	20; 20: Reserved
ISR_NOERR	21; 21: Reserved
ISR_NOERR	22; 22: Reserved
ISR_NOERR	23; 23: Reserved
ISR_NOERR	24; 24: Reserved
ISR_NOERR	25; 25: Reserved
ISR_NOERR	26; 26: Reserved
ISR_NOERR	27; 27: Reserved
ISR_NOERR	28; 28: Reserved
ISR_NOERR	29; 29: Reserved
ISR_NOERR	30; 30: Reserved
ISR_NOERR	31; 31: Reserved

DEF_INT	0xAC

%assign i	0xF0
%rep 16
	DEF_IRQ	i
%assign i i+1
%endrep

%macro MPUSH 1-*
	%rep %0
	push %1
	%rotate 1
	%endrep
%endmacro
%macro MPOP 1-*
	%rep %0
	pop %1
	%rotate 1
	%endrep
%endmacro

%macro PUSHAQ 0
	MPUSH rax, rcx, rdx, rbx
	MPUSH rsp, rbp, rsi, rdi
	MPUSH r8, r9, r10, r11
	MPUSH r12, r13, r14, r15
%endmacro
%macro POPAQ 0
	MPOP r15, r14, r13, r12
	MPOP r11, r10, r9, r8
	MPOP rdi, rsi, rbp, rsp
	MPOP rbx, rdx, rcx, rax
%endmacro
%macro PUSHFPU 0
	sub rsp, 512
	FXSAVE	[rsp]
%endmacro
%macro POPFPU 0
	FXRSTOR	[rsp]
	add rsp, 512
%endmacro

[extern _IDT_HandleError]
[global ErrorCommon]
ErrorCommon:
	PUSHAQ
	PUSHFPU
	
	mov rdi, rsp
	call	_IDT_HandleError
	
	POPFPU
	POPAQ
	add rsp, 16
	iretq

[global InterruptCommon]
InterruptCommon:
	add rsp, 16
	iretq

[global IRQCommon]
IRQCommon:
	push rdx
	push rax
	mov al, 0x20
	mov dx, 0x20
	out dx, al
	pop rax
	pop rdx
	add rsp, 16
	iretq
