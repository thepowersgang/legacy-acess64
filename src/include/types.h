/*
Acess64
 Core Types
 TYPES.H
*/
#ifndef _TYPES_H
#define _TYPES_H

typedef struct {
	Uint64	Low, Hi;
} Uint128;
typedef struct {
	Uint16	ControlWord, StatusWord;
	Uint16	TagWord, Opcode;
	Uint64	IP;	// In IA-32e Mode (Bottom 32: IP, Top 32: CS)
	Uint64	DP;	// In IA-32e Mode (Bottom 32: DP, Top 32: DS)
	Uint32	MXCSR, MXCSR_Mask;
	Uint128	ST0, ST1, ST2, ST3;	// Only bottom 80 bits valid
	Uint128	ST4, ST5, ST6, ST7;	// Only bottom 80 bits valid
	Uint128	XMM0, XMM1, XMM2, XMM3;
	Uint128	XMM4, XMM5, XMM6, XMM7;
	Uint128	XMM8, XMM9, XMM10, XMM11;
	Uint128	XMM12, XMM13, XMM14, XMM15;
	Uint128	Resvd[4];
} tFPURegs;

typedef struct {
	tFPURegs	fpu;
	Uint64	r15, r14, r13, r12, r11, r10, r9, r8;
	Uint64	rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
	Uint64	IntNum, ErrorCode;
	Uint64	rip, cs, eflags, useresp, ss;
} tRegs;

#endif
