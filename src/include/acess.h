/*
Acess64
 Kernel Core Header
 ACESS.H
*/
#ifndef _ACESS_H
#define _ACESS_H

//=== CONSTANTS
#define NULL	((void*)0)
#define	KERNEL_BASE	0xFFFF800000000000

//=== CORE TYPES
typedef unsigned char	Uint8;
typedef unsigned short	Uint16;
typedef unsigned int	Uint32;
typedef unsigned long long	Uint64;
typedef Uint64	Uint;

//=== GLOBAL INCLUDES
// --- Helper Macros (includes memset/memcpy)
#include <macros.h>
// --- Muliboot Definitions
#include <mboot.h>
#include <va_args.h>
#include <types.h>

//=== FUNCTIONS
// --- VGA Text Mode Driver
extern void Screen_Clear();
extern void Screen_HideCursor();
extern void Screen_Putc(char ch);
extern void Screen_Puts(char *str);
extern void Screen_PutHex(Uint number);
extern void Warning(char *fmt, ...);
extern void Panic(char *fmt, ...);
extern void LogF(char *fmt, ...);
// --- Library Functions
extern int	vcprintf(void (*putc)(char ch), char *fmt, va_list args);
extern void	itoa(char *buf, Uint num, Uint base, int minLength, char pad);
extern Uint	strlen(char *str);
extern int	strcmp(char *str1, char *str2);
extern void	memsetqa(void *dst, Uint val, Uint cnt);
extern void	memsetda(void *dst, Uint32 val, Uint cnt);
extern void	memcpyqa(void *dst, void *src, Uint cnt);
extern void	memcpyda(void *dst, void *src, Uint cnt);
// --- Memory Manager
extern void MM_Initialise(Uint upperMemory, tMultibootMMapEnt *memoryMap, Uint memoryMapMax);
// --- Serial Ports
extern void Serial_Initialise();
extern void Serial_Send(char ch);
// --- IDT
extern void	IDT_Initialise();
// --- Misc Drivers
extern void DevFS_Initialise();
extern void	ProcDev_Initialise();
extern void	Coprocessor_Initialise();

#endif
