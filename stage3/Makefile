# AcessOS 64 - Stage 3 Loader
# Makefile
CC = gccX.exe
AS = nasm.exe
LD = ldX.exe
RM = @rm -f

AOBJ = start.ao
COBJ = loader.o
BIN = ../A64St3.bin

CFLAGS = -Wall -O -fstrength-reduce -nostdinc -fno-builtin -nostdlib -nodefaultlibs
ASFLAGS = -felf
LDFLAGS = -T link.ld

.PHONY: all

all: $(BIN)

clean:
	$(RM) start32.ao $(AOBJ) $(COBJ)

$(BIN): link.ld $(AOBJ) $(COBJ)
	$(LD) $(LDFLAGS) -o "$@" $(AOBJ) $(COBJ) -M > Acess64_Stage3.map.txt

$(AOBJ): %.ao: %.asm
	$(AS) $(ASFLAGS) -o $@ $<
	
$(COBJ): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
