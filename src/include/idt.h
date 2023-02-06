/*
Acess64
 IDT Header
 IDT.H
*/
#ifndef _IDT_H
#define _IDT_H

typedef struct {
	Uint32	Base;
	Uint16	Limit;
} tIDTPtr;

typedef struct {
	Uint16	OfsLow, Sel;
	Uint16	Flags, OfsMid;
	Uint32	OfsHigh;
	Uint32	Reserved;
} tIDTEnt;

#endif
