/*
Acess64
 Multiboot Header
 MBOOT.H
*/
#ifndef _MBOOT_H
#define _MBOOT_H

typedef struct {
	Uint32	Start, End;
	Uint32	String,	Reserved;
} tMultibootModule;

struct s_MultibootMMapEnt {
	Uint32	Size;	// (May be at offset -4)
	Uint64	Base;
	Uint64	Length;
	Uint32	Type;	//1:RAM,Else Reserved
} __attribute__ ((packed));

typedef struct s_MultibootMMapEnt tMultibootMMapEnt;

typedef struct {
	Uint32	Flags;
	Uint32	MemLower;	// in KiB
	Uint32	MemUpper;	// in KiB
	char	BootDevice[4];	// #16
	Uint32	CommandLine;
	Uint32	ModuleCount;
	Uint32	Modules;
	Uint32	SymbolInfo[4];	// #32 UNUSED
	Uint32	MMapLength;
	Uint32	MMapAddr;		// #40
} tMultiboot;

#define	MBOOT_MAGIC_VALUE	0x2BADB002

#endif
