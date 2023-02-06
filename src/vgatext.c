/*
Acess64
 VGA Text Mode Debugging
 VGATEXT.C
*/
#include <acess.h>

// === GLOBALS ===
Uint16	*gpVGAText_Screen = (Uint16*)(KERNEL_BASE|0xB8000);
 int	giVGAText_Pos = 0;
Uint16	giVGAText_Attrib = 0x0F00;

// === FUNCTIONS ===
/**
 \fn void Screen_Clear()
 \brief Clears the screen
*/
void Screen_Clear()
{
	memsetqa( (void*)(KERNEL_BASE|0xB8000), 0, 80*25/4 );
}

/**
 \fn void Screen_HideCursor()
 \brief Hides VGA Cursor
*/
void Screen_HideCursor()
{
	// Set Cursor to not appear (Pos. 0x7D1, just off screen)
	outb(0x3D4, 14);	outb(0x3D5, 0x7);
	outb(0x3D4, 15);	outb(0x3D5, 0xD1);
}

/**
 \fn void Screen_Scroll()
 \brief Scrolls the screen up one line
*/
void Screen_Scroll()
{
	memcpyqa( (void*)(KERNEL_BASE|0xB8000), (void*)((KERNEL_BASE|0xB8000)+80*2), 80*24/4);
	memsetqa( (void*)((KERNEL_BASE|0xB8000)+80*24*2), 0, 80/4);
	giVGAText_Pos -= 80;
}

/**
 \fn void Screen_Putc(char ch)
 \brief Prints a character
*/
void Screen_Putc(char ch)
{
	switch( ch )
	{
	// Backspace
	case '\b':
		giVGAText_Pos --;
		while((gpVGAText_Screen[ giVGAText_Pos ] & 0xFF) == 0 && (giVGAText_Pos & 7))
			giVGAText_Pos --;
		break;
	
	// Tab
	case '\t':
		do {
			gpVGAText_Screen[ giVGAText_Pos ] = giVGAText_Attrib | 0;
			giVGAText_Pos ++;
		} while(giVGAText_Pos & 7);
		break;
	
	// Newline/Carrage Return
	case '\n':
		giVGAText_Pos += 80;
	case '\r':
		giVGAText_Pos /= 80;
		giVGAText_Pos *= 80;
		break;
	
	// Printable Character
	default:
		if(ch < 0x20)	break;
		gpVGAText_Screen[ giVGAText_Pos ] = giVGAText_Attrib | ch;
		giVGAText_Pos ++;
		break;
	}
	
	// Send to debugging console (serial)
	Serial_Send(ch);
	
	// Scroll if needed
	if(giVGAText_Pos > 25*80)
		Screen_Scroll();
}

/**
 \fn void Screen_Puts(char *str)
 \brief Prints a string to the terminal
 \param str	String to print
*/
void Screen_Puts(char *str)
{
	while(*str)	Screen_Putc(*str++);
}

static const char cUCDIGITS[] = "0123456789ABCDEF";
void Screen_PutHex(Uint number)
{
	char num[16];	//Max 16 Digits (64 Bits)
	int pos=0;
	while(number >> 4)
	{
		num[pos] = cUCDIGITS[number&0xF];
		pos++;
		number >>= 4;		//Shift {number} right 1 digit
	}
	num[pos] = cUCDIGITS[number&0xF];
	for(;pos >= 0;pos--) Screen_Putc(num[pos]);	//Reverse the order of characters
}

/**
 \fn void Warning(char *fmt, ...)
 \brief Prints yellow text to the screen
 \param fmt	Format String
*/
void Warning(char *fmt, ...)
{
	va_list	args;
	Uint16	attr = giVGAText_Attrib;
	giVGAText_Attrib = 0xE00;
	
	va_start(args, fmt);
	vcprintf( Screen_Putc, fmt, args );
	va_end(args);
	
	giVGAText_Attrib = attr;
}

/**
 \fn void Panic(char *fmt, ...)
 \brief Prints red text to the screen and then halts
 \param fmt	Format String
*/
void Panic(char *fmt, ...)
{
	va_list	args;
	giVGAText_Attrib = 0xC00;
	
	va_start(args, fmt);	
	vcprintf( Screen_Putc, fmt, args );	
	va_end(args);
	
	__asm__ __volatile__ ("cli");
	for(;;)
		__asm__ __volatile__ ("hlt");
}

/**
 \fn void LogF(char *fmt, ...)
 \brief Prints a formatted string to the screen
 \param fmt	String format
*/
void LogF(char *fmt, ...)
//#if _USE_VCPRINTF
{
	va_list	args;
	
	va_start(args, fmt);
	
	vcprintf( Screen_Putc, fmt, args );
	
	va_end(args);
}
/*
#else
{
	va_list	args;
	char	c, pad = ' ';
	 int	minSize = 0;
	Uint	i = 0;
	char	tmpBuf[65];	// For Integers
	char	*p = NULL;
	
	va_start(args, fmt);
  
	while((c = *fmt++) != 0)
	{
		if (c != '%')
			Screen_Putc(c);
		else
		{
			i = va_arg( args, Uint );
			c = *fmt++;
			// Padding
			if(c == '0') {
				pad = '0';
				c = *fmt++;
			} else
				pad = ' ';
			minSize = 0;
			if('1' <= c && c <= '9')
			{
				while('0' <= c && c <= '9')
				{
					minSize *= 10;
					minSize += c - '0';
					c = *fmt++;
				}
			}
			p = tmpBuf;
			switch (c) {
			case 'd':
			case 'i':
				if(i < 0) {
					Screen_Putc('-');
					i = -i;
				}
				itoa(p, i, 10, minSize, pad);
				goto printString;
			case 'u':
				itoa(p, i, 10, minSize, pad);
				goto printString;
			case 'x':
				itoa(p, i, 16, minSize, pad);
				goto printString;
			case 'o':
				itoa(p, i, 8, minSize, pad);
				goto printString;
			case 'b':
				itoa(p, i, 2, minSize, pad);
				goto printString;

			case 'B':	//Boolean
				if(i)	p = "True";
				else	p = "False";
				break;
				
            case 's':
				p = (char*)i;
			printString:
				if(!p)		p = "(null)";
				while(*p)	Screen_Putc(*p++);
				break;

			case '%':	Screen_Putc('%');	break;
				
			default:	Screen_Putc(i);	break;
            }
        }
    }
	va_end(args);
}
#endif
*/
