/*
AcessOS 64 - Stage 3 Loader
*/
#include "elf64.h"

typedef unsigned int	Uint;

// === PROTOTYPES ===
extern void	kEnd;
extern void memcpyda(void *dst, void *src, int num);
extern void memsetda(void *dst, Uint val, int num);
extern void StartImage(Uint entry);
void Panic(char *error);
void DumpHex32(Uint val);
void Write(Uint8 attr, char *string);

// === CONSTANTS ===
const char HexDigits[] = "0123456789abcdef";

// === GLOBALS ===
Uint16	*gScreen = (Uint16*) 0xB8000;

//=== CODE ===
void LoadElf64(void *base, Uint end)
{
	Elf64_Header	*hdr;
	Elf64_ShEnt		*sectTab;
	char	*strTab;
	 int	i;
	
	// Parse Header
	hdr = base;
	DumpHex32(end);
	
	*gScreen++ = 0x0700 | (hdr->IdStr&0xFF);
	*gScreen++ = 0x0700 | ((hdr->IdStr>>8)&0xFF);
	*gScreen++ = 0x0700 | ((hdr->IdStr>>16)&0xFF);
	*gScreen++ = 0x0700 | ((hdr->IdStr>>24)&0xFF);
	*gScreen++ = 0x0720;
	
	if(hdr->IdStr != ('\x7F'|('E'<<8)|('L'<<16)|('F'<<24)))	// Validate Magic
		Panic("Magic Number not '\\x7FELF'");
	
	if(hdr->FileClass != 2)	// Elf64
		Panic("File not Elf64\n");
	
	// Read Section Table
	sectTab = base + hdr->ShOfs;
	strTab = base + sectTab[ hdr->StrTabIndx ].Offs;
	
	DumpHex32( hdr->ShOfs );
	DumpHex32( hdr->ShEntNum );
	for( i = 0; i < hdr->ShEntNum; i++ )
	{	
		if(sectTab[i].Flags & SHF_ALLOC)
		{
			Write(0x7, strTab + sectTab[i].Name);
			DumpHex32(sectTab[i].VAddr);
			DumpHex32(sectTab[i].Type);
			//Panic("");
			if(sectTab[i].VAddr < end || sectTab[i].VAddr < (Uint)&kEnd)
				Panic("Image overwrites itself");
			if(sectTab[i].Type != SHT_NOBITS)
				memcpyda( (void*)(Uint32)sectTab[i].VAddr,
					(void*)(base + (Uint32)sectTab[i].Offs),
					sectTab[i].Size/4 );
			else
				memsetda( (void*)(Uint32)sectTab[i].VAddr, 0, sectTab[i].Size/4 );
		}
	}
	
	StartImage( hdr->Entry );
	
	Panic("Executable Loaded");
}

void Panic(char *error)
{
	Uint16	*Screen = (Uint16*) 0xB8000;
	Screen += 10*80;
	*Screen++ = 0x1700 | 'P';	*Screen++ = 0x1700 | 'A';
	*Screen++ = 0x1700 | 'N';	*Screen++ = 0x1700 | 'I';
	*Screen++ = 0x1700 | 'C';	*Screen++ = 0x1700 | ':';
	*Screen++ = 0x1700 | ' ';
	
	while(*error)
		*Screen++ = 0x1700 | *error++;
	__asm__ __volatile__ ("cli");
	for(;;) __asm__ __volatile__ ("hlt");
}

void Write(Uint8 attr, char *string)
{	
	while(*string)
		*gScreen++ = (attr<<8) | *string++;
	*gScreen++ = (attr << 8) | ' ';
}

void DumpHex32(Uint val)
{
	*gScreen++ = 0x700 | '0';
	*gScreen++ = 0x700 | 'x';
	*gScreen++ = 0x700 | HexDigits[(val>>28)&0xF];
	*gScreen++ = 0x700 | HexDigits[(val>>24)&0xF];
	*gScreen++ = 0x700 | HexDigits[(val>>20)&0xF];
	*gScreen++ = 0x700 | HexDigits[(val>>16)&0xF];
	*gScreen++ = 0x700 | HexDigits[(val>>12)&0xF];
	*gScreen++ = 0x700 | HexDigits[(val>>8)&0xF];
	*gScreen++ = 0x700 | HexDigits[(val>>4)&0xF];
	*gScreen++ = 0x700 | HexDigits[(val>>0)&0xF];
	*gScreen++ = 0x700 | '|';
}
