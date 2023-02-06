; AcessOS 64
; Core Macros
; MACROS.INC
[BITS 64]

;Registers rdi, rsi, rdx, rcx, r8, r9 are 1st 6 arguments
;Registers rbp, rbx, r12-r15 must be preserved

%macro GET_ARG	2
	mov %1, [rbp + 0x8 * (%2+1)]
%endmacro

%macro INIT_FRAME 2
	push	rbp
	mov		rbp, rsp
	SAVE	%2
	sub		rsp, %1*0x8	; Local Arguments
%endmacro

%macro CLOSE_FRAME 1
	RESTORE	%1
	mov		rsp, rbp
	pop		rbp
%endmacro

%macro SAVE	1-*
	%rep %0
	push	%1
	%rotate 1
	%endrep
%endmacro

%macro RESTORE	1-*
	%rep %0
	%rotate -1
	push	%1
	%endrep
%endmacro

%macro CCALL	1-*
	%rep %0-1
	push	%2
	%rotate	1
	%endrep
	%rotate	-1
	call	%1
	%if %0-1
	add	rsp, ( %0 - 1 ) * 0x8
	%endif
%endmacro
