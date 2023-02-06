/*
Acess64
 Helper Macros
 MACROS.H
*/
#ifndef _MACROS_H
#define _MACROS_H

// === Symbols ===
#define	SYMSTR(x)	#x
#define	EXPAND2STR(x)	SYMSTR(x)
#define	ARR_SIZE(a)	(sizeof((a))/sizeof((a)[0]))

// === Port IO ===
#define outb(port, val)	__asm__ __volatile__ ("outb %%al, %%dx" : : "a"(val), "d"(port));
static inline Uint8 inb(Uint16 port) {
	Uint8	ret;
	__asm__ __volatile__ ("inb %%dx, %%al" : "=a" (ret) : "d" (port));
	return ret;
}


// === Spinlocks ===
/**
 \def SpinlockAcquire
 \brief Atomically acquires a spinlock
 \param lockPtr	Pointer - Spinlock memory location
 */
#define SpinlockAcquire(lockPtr)	\
	do{int v=1;while(v){\
	__asm__ __volatile__ ("lock cmpxchgl %%eax, (%1)":"=a"(v):"D"((lockPtr)),"a"(1));\
}}while(0)
/**
 \def SpinlockRelease
 \brief Atomically releases a spinlock
 \param lockPtr	Pointer - Spinlock memory location
 */
#define SpinlockRelease(lockPtr)	\
	do{\
	__asm__ __volatile__ ("lock andl $0, (%0)"::"D"((lockPtr)));\
}while(0)

#endif
