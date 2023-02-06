/*
AcessOS 64
ELF64 Executable Loader

ELF64.H
*/
#ifndef _ELF64_H
#define _ELF64_H

typedef unsigned char	Uint8;
typedef unsigned short	Uint16;
typedef unsigned long	Uint32;
typedef unsigned long long	Uint64;

typedef struct {
	union {
		struct {
			Uint32	IdStr;
			Uint8	FileClass;
			Uint8	DataEncoding;
			Uint8	FileVersion;
			Uint8	AbiId;
			Uint8	AbiVer;
		};
		Uint8	Ident[16];
	};
	Uint16	Type;
	Uint16	Machine;
	Uint32	Version;
	Uint64	Entry;
	Uint64	PhOfs;
	Uint64	ShOfs;
	Uint32	Flags;
	Uint16	HdrSize;
	Uint16	PhEntSize;
	Uint16	PhEntNum;
	Uint16	ShEntSize;
	Uint16	ShEntNum;
	Uint16	StrTabIndx;
} Elf64_Header;

typedef struct {
	Uint32	Name;
	Uint32	Type;
	Uint64	Flags;
	Uint64	VAddr;
	Uint64	Offs;
	Uint64	Size;
	Uint32	Link;
	Uint32	Info;
	Uint64	Align;
	Uint64	EntSize;
} Elf64_ShEnt;

enum {
	SHT_NULL,
	SHT_PROGBITS,
	SHT_SYMTAB,
	SHT_STRTAB,
	SHT_RELA,
	SHT_HASH,
	SHT_DYNAMIC,
	SHT_NOTE,
	SHT_NOBITS,
	SHT_REL,
	SHT_SHLIB,
	SHT_DYNSYM
};


#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXEINSTR	0x4

#endif /*_ELF64_H*/
