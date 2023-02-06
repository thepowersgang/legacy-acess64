/*
Acess64
 Kernel Library Functions
 LIB.C
*/
#include <acess.h>

// === Constants ===
static const char cUCDIGITS[] = "0123456789ABCDEF";

// === Code ===
/**
 \fn Uint strlen(char *str)
 \brief Get the length of a string
*/
Uint strlen(char *str)
{
	register Uint length=0;
	while(*str)	str++,length++;
	return length;
}
/**
 \fn int strcmp(char *str1, char *str2)
 \brief Compares two strings
*/
int strcmp(char *str1, char *str2)
{
	while(*str1 == *str2 && *str1)	str1++,str2++;
	return *str1 - *str2;
}

/**
 \fn void itoa(char *buf, Uint num, Uint base, int minLength, char pad)
 \brief Converts a number to a string
 \param buf	Target Buffer
 \param num	Number to convert
 \param base	Base to convert to (16 for hex etc)
 \param minLength	Minimum length of final string
 \param pad	Padding character to bring string to target length
*/
void itoa(char *buf, Uint num, Uint base, int minLength, char pad)
{
	char tmpBuf[64];
	int pos=0,i;

	if(!buf)	return;
	if(base > 16) {
		buf[0] = 0;
		return;
	}
	
	while(num > base-1 && pos < 64) {
		tmpBuf[pos] = cUCDIGITS[ num % base ];
		num = (long) num / base;		//Shift {number} right 1 digit
		pos++;
	}

	tmpBuf[pos++] = cUCDIGITS[ num % base ];		//Last digit of {number}
	i = 0;
	minLength -= pos;
	while(minLength-- > 0)	buf[i++] = pad;
	while(pos-- > 0)		buf[i++] = tmpBuf[pos];	//Reverse the order of characters
	buf[i] = 0;
}

/**
 \fn int vcprintf( void (*putc)(char ch), char *fmt, void **args)
 \brief Prints a formatted string using the specified callback
 \param putc	Callback function
 \param fmt	Format String
 \param args	Pointer to stack at first argument
*/
int vcprintf(void (*putc)(char ch), char *fmt, va_list args)
{
	char	c, pad = ' ';
	 int	minSize = 0;
	Uint	i = 0;
	char	tmpBuf[65];	// For Integers
	char	*p = NULL;
	 int	ret = 0;
  
	while((c = *fmt++) != 0)
	{
		if (c != '%')
			putc(c), ret++;
		else
		{
			c = *fmt++;
			if(c == '%') {
				putc('%');
				ret ++;
				continue;
			}
			i = va_arg(args, Uint);
			// Padding
			if(c == '0') {
				pad = '0'; c = *fmt++;
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
					putc('-');
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
				while(*p) {
					ret++;
					putc(*p++);
				}
				break;
				
			default:
				ret ++;
				putc(i);
				break;
            }
        }
    }
	return ret;
}