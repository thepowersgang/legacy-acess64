/*
Acess64
 Memory Manager Header
 MM.H
*/
#ifndef _MM_H
#define _MM_H

// NOTES::::
// 0xFFFF 80?? ???? ????
// ..00 0000 0000 - Kernel code/data (4Gb)
// ..01 0000 0000 - Modules (12Gb)
// ..04 0000 0000 - Kernel Heap (8Gb)
// ..05 0000 0000 - Kernel VFS File Handles (4 Gb)
// ..06 0000 0000 - User VFS File Handles (4 Gb) [[NON GLOBAL]]
// ..08 0000 0000 - Kernel Stack Minimum (32 Gb) [[NON GLOBAL]]
// ..10 0000 0000 - Kernel Stack Maximum (Thread 0 Initial RSP) [[NON GLOBAL]]
// ..10 0000 0000 - Hardware Mappings (64 Gb)
// ..20 0000 0000 - Reference Counts (8Gb Reserved)
// .022 0000 0000 - Unallocated (Until end of address space)
// FF00 0000 0000 - Current Table Fractals
// FF80 0000 0000 - Tempory Table Fractals
// 1 PML4 Entry = 0x0000 0080 0000 0000 (512GiB)
// 1 PDP Entry  = 0x0000 0000 4000 0000 (1GiB)
// 1 PD Entry   = 0x0000 0000 0020 0000 (2MiB)
// 1 PT Entry   = 0x0000 0000 0000 1000 (4KiB)

#define MM_CODE_ADDR	0xFFFF8000##00000000	// Kernel Image / Bitmaps (4Gb)
#define	MM_MODULES_ADDR	0xFFFF8001##00000000	// Kernel Modules (12Gb)
#define	MM_HEAP_ADDR	0xFFFF8004##00000000	// Kernel Heap Address (8Gb)
#define MM_KVFS_ADDR	0xFFFF8005##00000000	// Kernel VFS Handles (4Gb)
#define MM_UVFS_ADDR	0xFFFF8006##00000000	// User VFS Handles (4Gb)
#define MM_KSTACK_MIN	0xFFFF8008##00000000	// Last Kernel Stack Address
	// Stack (32Gb Allocated, 32Mb Per Thread, 1024 KThreads/Proc)
#define MM_KSTACK_MAX	0xFFFF8010##00000000	// First Kernel RSP
#define MM_HWMAP_ADDR	0xFFFF8010##00000000	// Hardware Mappings Base (64Gb) (Pleanty)
#define MM_REFCNT_ADDR	0xFFFF8020##00000000	// Reference Counts (8Gb Reserved, 4 Bytes Ea.)

#define	MM_FRACT_ADDR	0xFFFFFF00##00000000	// Tables (1Gb needed, 512GiB Allocated)
#define	MM_FRACTT_ADDR	0xFFFFFF80##00000000	// Tempory Tables (1Gb, 512GiB Allocated)

#define	MM_KSTACK_SIZE	0x2000000
#define MM_KTHREAD_COUNT	( (MM_KSTACK_MAX-MM_KSTACK_MIN) / MM_KSTACK_SIZE )
#define	MM_PID_COUNT	0x100000

#define PAGE_OFS(addr)	((addr)&0xFFF)
#define PAGE_ID(addr)	(((addr)>>12)&0x1FF)
#define TABLE_ID(addr)	(((addr)>>21)&0x1FF)
#define DIR_ID(addr)	(((addr)>>30)&0x1FF)
#define PDP_ID(addr)	(((addr)>>39)&0x1FF)

#define	TABLE(pdp, dir, tab, pg)	*(Uint64*)(MM_FRACT_ADDR+((pdp)<<30)+((dir)<<21)+((tab)<<12)+(pg)*8)
#define	DIR(pdp, dir, tab)	TABLE(PDP_ID(MM_FRACT_ADDR), (pdp), (dir), (tab))
#define	PDP(pdp, dir     )	TABLE(PDP_ID(MM_FRACT_ADDR), PDP_ID(MM_FRACT_ADDR), (pdp), (dir))
#define	PML4(pdp         )	PDP(PDP_ID(MM_FRACT_ADDR), (pdp))

#define TABLE_A( addr )	*(Uint64*)(MM_FRACT_ADDR+((((addr)&0xFFFFFFFFFFFF)>>12)*8))
//#define TABLE_A( addr )	TABLE( PDP_ID(addr), DIR_ID(addr), TABLE_ID(addr), PAGE_ID(addr) )
#define DIR_A( addr )	*(Uint64*)(MM_FRACT_ADDR+(PDP_ID(MM_FRACT_ADDR)<<30)+((((addr)&0xFFFFFFFFFFFF)>>21)*8))

#define	TABLE_T(pdp, dir, tab, pg)	*(Uint64*)(MM_FRACTT_ADDR+((pdp)<<30)+((dir)<<21)+((tab)<<12)+(pg)*8)
#define	DIR_T(pdp, dir, tab)	TABLE_T(PDP_ID(MM_FRACT_ADDR), (pdp), (dir), (tab))
#define	PDP_T(pdp, dir)			TABLE_T(PDP_ID(MM_FRACT_ADDR), PDP_ID(MM_FRACT_ADDR), (pdp), (dir))
#define	PML4_T(pdp)		PDP_T(PDP_ID(MM_FRACT_ADDR), (pdp))

#endif
