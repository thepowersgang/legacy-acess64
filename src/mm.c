/*
Acess64
 Memory Manager
*/
#include <acess.h>
#include <mboot.h>
#include <mm.h>

// === Imported Variables ===
extern void	gvKernelEnd;
extern void	gvKernelStart;
extern Uint	gpInitial_PML4[];
extern Uint	gpInitial_PDP[];

// === Global Variables ===
Uint	giMM_PageCount = 0;
Uint	giMM_PagesUsed = 0;
Uint	giMM_CurrrentCR3 = 0;
Uint	*gpMM_PhysBitmap = NULL;
Uint	*gpMM_PhysSBitmap = NULL;
Uint	*gpMM_RefCounts = (void*)MM_REFCNT_ADDR;
static Uint	siMM_BitmapLock = 0;
#define	LOCK_PBITMAPS()		SpinlockAcquire(&siMM_BitmapLock);
#define	UNLOCK_PBITMAPS()	SpinlockRelease(&siMM_BitmapLock);

// === Prototypes ===
void MM_Initialise(Uint upperMemory, tMultibootMMapEnt *memoryMap, Uint memoryMapSize);
void MM_SetProcess(Uint cr3);
void MM_FlushTLB();
Uint MM_AllocPhys();
void MM_FreePhys(Uint address);
Uint MM_FreeupPhysical();

// === Code ===
/**
 \fn void MM_Initialise(Uint upperMemory, tMultibootMMapEnt *memoryMap, Uint memoryMapSize)
 \brief Initialises the Memory Manager
 \param upperMemory	Integer - Total bytes until first memory hole
 \param memoryMap		Pointer - Multiboot memory map structure
 \param memoryMapSize	Integer - Size of memory map structure
*/
void MM_Initialise(Uint upperMemory, tMultibootMMapEnt *memoryMap, Uint memoryMapSize)
{
	tMultibootMMapEnt	*ent;
	Uint	maxAddr = 0;
	Uint	physBmSize = 0;
	Uint	physSBmSize = 0;
	Uint	a, b, i, j;
	
	LogF("[MM  ] Max 0x%x Concurrent Proces with %i threads each\n", MM_PID_COUNT, MM_KTHREAD_COUNT);
	
	// --- Find largest address
	ent = memoryMap;
	while( (Uint)ent < (Uint)memoryMap + memoryMapSize )
	{
		// Adjust for size
		ent->Size += 4;
		// If entry is RAM and is above `maxAddr`, change `maxAddr`
		if(ent->Type == 1 && ent->Base + ent->Length > maxAddr)
			maxAddr = ent->Base + ent->Length;
		// Go to next entry
		ent = (tMultibootMMapEnt*)( (Uint)ent + ent->Size );
	}
	
	// --- Get Page Count
	giMM_PageCount = maxAddr >> 12;
	
	// --- Startup Logging (_NOT_ Debug)
	LogF("[MM  ] %i Accessable Physical Pages (%i MiB)\n", giMM_PageCount, giMM_PageCount >> 8);
	
	// --- Create Tables
	physBmSize = (giMM_PageCount / 8 + 7)&~7;
	physSBmSize = (giMM_PageCount / (8*64) + 7)&~7;
	
	if(physBmSize + physSBmSize < 0x100000)	// - Size is below 1Mb - Fits in (1Mib-2MiB) hole
	{
		gpMM_PhysBitmap = (void*)0xFFFF800000100000;
		gpMM_PhysSBitmap = (void*)( 0xFFFF800000100000 + physBmSize );
	}
	else		// - Size of tables is above 1 Mb - Place above kernel
	{
		gpMM_PhysBitmap = &gvKernelEnd;
		gpMM_PhysSBitmap = (void*)( (Uint)&gvKernelEnd + physBmSize );
	}
	
	// --- Clear Bitmaps
	memsetqa(gpMM_PhysBitmap, 0, physBmSize / 8);
	memsetqa(gpMM_PhysSBitmap, 0, physSBmSize / 8);
	
	// --- Fill bitmap from memory map
	ent = memoryMap;
	while( (Uint)ent < (Uint)memoryMap + memoryMapSize )
	{		
		if(ent->Type != 1)
		{
			a = ent->Base >> 12;
			b = ent->Size >> 12;
			for( j = a; j < a + b; j++ )
				gpMM_PhysBitmap[j/64] |= 1 << j%64;
			giMM_PagesUsed += b;
		}
		
		ent = (tMultibootMMapEnt*)( (Uint)ent + ent->Size );
	}
	
	// --- Fill bitmap from Kernel
	for( i = (Uint)&gvKernelStart >> 12; i < (Uint)&gvKernelEnd >> 12; i++ )
	{
		gpMM_PhysBitmap[i/64] |= 1 << i%64;
	}
	giMM_PagesUsed += i - ((Uint)&gvKernelStart >> 12);
	
	// --- Fill bitmap from Bitmaps
	for(
		i = ((Uint)gpMM_PhysBitmap&0xFFFFFFFF) >> 12;
		i < (((Uint)gpMM_PhysSBitmap&0xFFFFFFFF)+physSBmSize+0xFFF) >> 12;
		i++ )
	{
		gpMM_PhysBitmap[i/64] |= 1 << i%64;
	}
	giMM_PagesUsed += (physSBmSize + physBmSize + 0xFFF) >> 12;
	
	// --- Fill Super Page Bitmap
	for( i = 0; i < physBmSize/4; i ++)
	{
		if(gpMM_PhysBitmap[i] == -1)
			gpMM_PhysSBitmap[i/64] |= 1 << (i % 64);
	}
	
	// --- Startup Logging
	LogF("[MM  ] Bitmaps Created (0x%x bytes)\n", giMM_PageCount, giMM_PageCount >> 8);
	
	// --------------------
	//  Create Paging Structures
	// --------------------
	giMM_CurrrentCR3 = (Uint) &gpInitial_PML4;
	// Zero Unused (Including boot ID mapping)
	for( i = 0; i < PDP_ID(KERNEL_BASE); i++ )	gpInitial_PML4[i] = 0;
	for( i = PDP_ID(KERNEL_BASE)+1; i < 512; i++ )	gpInitial_PML4[i] = 0;
	// Set Kernel
	gpInitial_PML4[ PDP_ID(KERNEL_BASE) ] = (Uint)gpInitial_PDP | 3;
	// Set Fractal Mapping
	gpInitial_PML4[ PDP_ID(MM_FRACT_ADDR) ] = (Uint)gpInitial_PML4 | 3;
	
	LogF("[MM  ] Base kernel and Fractals set\n");
	
	// Setup Reference Counter
	if( PML4( PDP_ID(MM_REFCNT_ADDR) ) == 0 ) {
		//LogF(" MM_Initialise: Set PML4 Entries for Reference Counter\n");
		PML4( PDP_ID(MM_REFCNT_ADDR) ) = MM_AllocPhys() | 3;
		MM_FlushTLB();
		memsetqa( &PDP( PDP_ID(MM_REFCNT_ADDR), 0), 0, 512 );
	}
	
	// Set PDP Entries
	j = (giMM_PageCount*4+0x3FFFFFFF) >> (12+9+9);
	//LogF(" MM_Initialise: Set PDP Entries for Reference Counter (%i)\n", j);
	for( i = 0; i < j; i++ ) {
		PDP( PDP_ID(MM_REFCNT_ADDR), DIR_ID(MM_REFCNT_ADDR)+i ) = MM_AllocPhys() | 3;
	//	LogF(" MM_Initialise: Setting Dir #%i (0x%x)\n", i, (MM_REFCNT_ADDR + (i << (12+9+9))));
	}
	MM_FlushTLB();
	memsetqa( &DIR_A( MM_REFCNT_ADDR ), 0, 512*j );
	
	// Set PD Entries
	//LogF(" MM_Initialise: Set PD Entries for Reference Counter\n");
	for( i = 0; i < (giMM_PageCount*4+0x1FFFFF) >> (12+9); i++ ) {
		DIR_A( MM_REFCNT_ADDR + (i << (12+9)) ) = MM_AllocPhys() | 3;
	//	LogF(" MM_Initialise: Setting Table #%i (0x%x)\n", i, (MM_REFCNT_ADDR + (i << (12+9))));
	}
	MM_FlushTLB();
	memsetqa( &TABLE_A( MM_REFCNT_ADDR ), 0, 512*i );
	
	// Set Table Entries
	//LogF(" MM_Initialise: Set Table Entries for Reference Counter\n");
	for( i = 0; i < (giMM_PageCount*4+0xFFF) >> (12); i++ ) {
	//	LogF(" MM_Initialise: TABLE_A = 0x%016x\n", &TABLE_A( MM_REFCNT_ADDR + (i << 12) ) );
	//	LogF(" MM_Initialise: Setting Page #%i (0x%x)\n", i, (MM_REFCNT_ADDR + (i << 12)));
		TABLE_A( MM_REFCNT_ADDR + (i << 12) ) = MM_AllocPhys() | 3;
	}
	MM_FlushTLB();
	memsetqa( (void*)MM_REFCNT_ADDR, 0, 512*i );
	
	// Set Counts
	//LogF(" MM_Initialise: Set page reference counts\n");
	memsetda( gpMM_RefCounts, 1, giMM_PagesUsed);
	
	LogF("[MM  ] Page reference counters set up\n");
	LogF("[MM  ] %i KiB of paging memory avaliable\n", (giMM_PageCount - giMM_PagesUsed) * 4);
}

/**
 \fn void MM_SetProcess(Uint cr3)
 \brief Sets address space to that of the required process
 \param cr3	Adress of processes PML4 structure
*/
void MM_SetProcess(Uint cr3)
{
	__asm__ __volatile__ ("mov %%rax, %%cr3" : : "a" (cr3));
	giMM_CurrrentCR3 = cr3;
}

/**
 \fn void MM_FlushTLB()
 \brief Refereshes the TLB
*/
void MM_FlushTLB()
{
	__asm__ __volatile__ ("mov %%rax, %%cr3" : : "a" (giMM_CurrrentCR3));
}

/**
 \fn Uint MM_AllocPhys()
 \brief Allocates a physical page and returns it
 \return Physical address of allocated page
*/
Uint MM_AllocPhys()
{
	Uint	a=0, b=0, c=0;
	Uint	addr;
	
	// Check for free space
	if(giMM_PagesUsed == giMM_PageCount) {
		return MM_FreeupPhysical();
	}
	
	// Aquire Spinlock
	LOCK_PBITMAPS();
	
	// Find Free Page
	for(;gpMM_PhysSBitmap[a]==-1;a++);
	for(;gpMM_PhysSBitmap[a]&1<<b;b++);
	for(;gpMM_PhysBitmap[a*64+b]&1<<c;c++);
	
	// Mark Page
	gpMM_PhysBitmap[a*64+b] |= 1 << c;
	if( gpMM_PhysBitmap[a*64+b] == -1 )
		gpMM_PhysSBitmap[a] |= 1 << b;
	
	// Increment Used Count
	giMM_PagesUsed ++;
	
	// Free Spinlock
	UNLOCK_PBITMAPS();
	
	// Get Address
	addr = a << (6+6+12);
	addr |= b << (6+12);
	addr |= c << (12);
	
	return addr;
}

/**
 \fn void MM_FreePhys(Uint address)
 \brief Frees a physical page
 \param address	Address of page to free
*/
void MM_FreePhys(Uint address)
{
	Uint a, b, c;
	
	#if DEBUG >= 1
	if(address & 0xFFF)
		Warning("[MM  ] Non-Aligned address passed to MM_FreePhys");
	#endif
	
	// Get Address Parts
	a = address >> (6+6+12);
	b = address >> (6+12);	b &= 63;
	c = address >> (12);	c &= 63;
	
	// Acquire Spinlock
	LOCK_PBITMAPS();
	// Decrement Used Count
	giMM_PagesUsed --;
	// Free Page
	gpMM_PhysBitmap[a*64+b] &= ~(1 << c);
	if( gpMM_PhysBitmap[a*64+b] == 0 )
		gpMM_PhysSBitmap[a] &= ~(1 << b);
	// Free Spinlock
	UNLOCK_PBITMAPS();
}

/**
 \fn Uint MM_FreeupPhysical()
 \brief Frees up physical memory space by paging out to disk
 \return Address of a free page
 
 NOTE: Called by MM_AllocPhys
*/
Uint MM_FreeupPhysical()
{
	Warning("[MM  ] TODO: Implement Disk Paging\n");
	Panic("[MM  ] PANIC: Out of Physical Memory\n");
	return 0;
}
