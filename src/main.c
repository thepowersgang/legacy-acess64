/*
Acess64
 Kernel Main
*/
#include <acess.h>
#include <mboot.h>
#include <build.h>

extern Uint32	giMultibootMagic;
extern tMultiboot	*gpMultibootStruct;

/**
 \fn void kmain()
 \brief Kernel Main
*/
void kmain()
{
	Serial_Initialise();
	Coprocessor_Initialise();
	// Clear Screen
	Screen_Clear();
	Screen_HideCursor();
	
	// Place Header
	Screen_Puts("Acess64 Version 1\n");
	Screen_Puts("Build "EXPAND2STR(ACESS_BUILD)", Timestamp "__TIMESTAMP__"\n");
	
	// Check for Multiboot Compliant Bootloader
	if(giMultibootMagic != MBOOT_MAGIC_VALUE)
	{
		Panic("[BOOT] ERROR: Multiboot compilant bootloader required!\n");
	}
	Screen_Puts("[BOOT] Multiboot Compliant Boot-Loader Used\n");
	
	// Install IDT
	IDT_Initialise();
	
	// Set TSS
	//__asm__ __volatile__ ("xchg %bx,%bx;\n\tmov $0x28, %ax;\n\tltr %ax");
	
	// Initialise Memory Manager
	MM_Initialise(
		gpMultibootStruct->MemUpper,
		(void*)(KERNEL_BASE|gpMultibootStruct->MMapAddr),
		gpMultibootStruct->MMapLength );
	
	// Install Filesystems / Drivers
	DevFS_Initialise();		// Device FS
	LogF("[BOOT] DevFS Installed\n");
	ProcDev_Initialise();	// System Information
	LogF("[BOOT] Proc Installed\n");
	
	Screen_Puts("[BOOT] Kernel Loaded, Halting.\n");
	__asm__ __volatile__ ("cli");
	for(;;) __asm__ __volatile__ ("hlt");
}