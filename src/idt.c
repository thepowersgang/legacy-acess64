/*
Acess64
 IDT / IRQ Handler
 INTERRUPT.C
*/
#include <acess.h>
#include <idt.h>

// === Types ===
typedef void (*tIDT_Callback)(tRegs *);

// === Variables ===
tIDT_Callback	gaIDT_Callbacks[256] = {0};

// === Constants ===
#if 0
const char *cERROR_NAMES[] = {
	"Division By Zero",	"Debug", "Non Maskable Interrupt", "Breakpoint",
	"Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",

	"Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
	"Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",

	"Coprocessor Fault", "Alignment Check", "Machine Check", "Reserved",
	"Reserved", "Reserved", "Reserved", "Reserved",

	"Reserved", "Reserved", "Reserved", "Reserved",
	"Reserved", "Reserved", "Reserved", "Reserved"
};
#endif

// === Code ===
/**
 \fn void IDT_Set(Uint8 id, void (*callback)(tRegs *regs))
 \brief Sets an entry in the IDT
 \param id	Interrupt Number
 \param callback	Pointer to callback
*/
void IDT_Set(Uint8 id, tIDT_Callback callback)
{
	gaIDT_Callbacks[id] = callback;
}

/**
 \fn void IDT_HandleError(void)
 \brief Handles a CPU Error
*/
void IDT_HandleError(tRegs *regs)
{
	if(regs->IntNum >= 32)
		Warning("[IDT ] Error handler invoked for non-error\n");
	else
	{
		char	*errMsg;
		//Uint	cr2;
		//if(regs->IntNum != 14)
		//	__asm__ __volatile__ ("mov %%cr2, %0" : "=r" (cr2) :);
		switch(regs->IntNum)
		{
		case 0:		errMsg = "Division By Zero";	break;
		case 1:		errMsg = "Debug";		break;
		case 2:		errMsg = "Non Maskable Interrupt";	break;
		case 3:		errMsg = "Breakpoint";		break;
		case 4:		errMsg = "Into Detected Overflow";	break;
		case 5:		errMsg = "Out of Bounds";	break;
		case 6:		errMsg = "Invalid Opcode";	break;
		case 7:		errMsg = "No Coprocessor";	break;
		case 8:		errMsg = "Double Fault";	break;
		case 9:		errMsg = "Coprocessor Segment Overrun";	break;
		case 10:	errMsg = "Bad TSS";		break;
		case 11:	errMsg = "Segment Not Present";	break;
		case 12:	errMsg = "Stack Fault";	break;
		case 13:	errMsg = "General Protection Fault";	break;
		case 14:	errMsg = "Page Fault";	break;
		case 15:	errMsg = "Unknown Interrupt";	break;
		case 16:	errMsg = "Coprocessor Fault";	break;
		case 17:	errMsg = "Alignment Check";	break;
		case 18:	errMsg = "Machine Check";	break;
		default:	errMsg = "Reserved";	break;
		}
		Panic(
			"[IDT ] %s\n"
			"CPU State:\n"
			"CS:RIP 0x%02x:%016x SS:RSP 0x%02x:%016x\n"
			"Error Code 0x%02x\n"
			"RAX 0x%08x:0x%08x RDX 0x%08x:0x%08x\n"
			"RCX 0x%08x:0x%08x RBX 0x%08x:0x%08x\n"
			"RSI 0x%08x:0x%08x RDI 0x%08x:0x%08x\n"
			"RSP 0x%08x:0x%08x RBP 0x%08x:0x%08x\n"
			"R8  0x%08x:0x%08x R9  0x%08x:0x%08x\n"
			"R10 0x%08x:0x%08x R11 0x%08x:0x%08x\n"
			"R12 0x%08x:0x%08x R13 0x%08x:0x%08x\n"
			"R14 0x%08x:0x%08x R15 0x%08x:0x%08x\n"
			,
			errMsg,
			regs->cs, regs->rip, regs->ss, regs->rsp,
			regs->ErrorCode,
			regs->rax>>32, regs->rax&0xFFFFFFFF, regs->rdx>>32, regs->rdx&0xFFFFFFFF,
			regs->rcx>>32, regs->rcx&0xFFFFFFFF, regs->rbx>>32, regs->rbx&0xFFFFFFFF,
			regs->rsi>>32, regs->rsi&0xFFFFFFFF, regs->rdi>>32, regs->rdi&0xFFFFFFFF,
			regs->rsp>>32, regs->rsp&0xFFFFFFFF, regs->rbp>>32, regs->rbp&0xFFFFFFFF,
			regs->r8>>32,  regs->r8&0xFFFFFFFF,  regs->r9>>32,  regs->r9&0xFFFFFFFF,
			regs->r10>>32, regs->r10&0xFFFFFFFF, regs->r11>>32, regs->r11&0xFFFFFFFF,
			regs->r12>>32, regs->r12&0xFFFFFFFF, regs->r13>>32, regs->r13&0xFFFFFFFF,
			regs->r14>>32, regs->r14&0xFFFFFFFF, regs->r15>>32, regs->r15&0xFFFFFFFF
			);
	}
}
