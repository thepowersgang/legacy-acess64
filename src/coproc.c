/*
Acess64
 Coprocessor Support
 COPROC.C
*/
#include <acess.h>

void Coprocessor_Initialise()
{
	Uint	cr4;
	Uint16	mode = 0x33F;	// Control Word Value (Mode)

	__asm__ __volatile__("mov %%cr4, %%rax" : "=a" (cr4));	// Get CR4 Value
	cr4 |= 0x200;	// Set OSFXSR (bit 9)
	__asm__ __volatile__("mov %%rax, %%cr4;" : : "a"(cr4));	// Reload CR4
	__asm__ __volatile__("finit" : : "r"(cr4));	// Do FINIT (FPU Initialise)
	__asm__ __volatile__("fldcw %0" : : "m" (mode));	// Set FPU Mode
}
