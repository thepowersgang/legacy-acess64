# AcessOS 64
# Makefile
CC = gcc
AS = nasm
LD = ld
RM = @rm -f
OBJDUMP = objdump
NEXTBUILD = ../helpers/build

AOBJ = start32.ao start64.ao idt.ao lib.ao
# kmain.ao mm.ao vgatext.ao lib.ao vfs.ao fs_devfs.ao
COBJ = main.o vgatext.o mm.o coproc.o lib.o serial.o idt.o vfs.o fs/devfs.o drv/procdev.o
BIN = ../Acess64.bin

CFLAGS = -Wall -O2 -mcmodel=large -fleading-underscore -nostdinc -fno-builtin -I./include
ASFLAGS = -felf64
LDFLAGS = -T link.ld -nostdlib -nodefaultlibs

.PHONY: all

all: $(BIN)

clean:
	$(RM) $(AOBJ) $(COBJ)

# Build Rules	
include/build.h:	$(AOBJ:%.ao=%.asm) $(COBJ:%.o=%.c)
	$(NEXTBUILD) ./include/build.h

$(BIN): link.ld $(AOBJ) $(COBJ)
#	echo $(LD) -o "$@" $(AOBJ) $(COBJ)
	$(LD) $(LDFLAGS) -o "$@" $(AOBJ) $(COBJ) -Map ../Acess64.map.txt
	$(OBJDUMP) -d $(BIN) > ../Acess64.dsm
	copy ..\Acess64.bin b:
	
$(AOBJ): %.ao: %.asm
#	@echo $(AS) -o $@ -c $<
	$(AS) $(ASFLAGS) -o $@ $<
	
$(COBJ): %.o: %.c
#	@echo $(CC) -o $@ -c $<
	$(CC) $(CFLAGS) -o $@ -c $<

# Dependencies
main.o:	include/build.h
$(COBJ):	include/acess.h
$(filter fs/%.o,$(COBJ)):	include/vfs.h
$(filter drv/%.o,$(COBJ)):	include/vfs.h include/fs/devfs.h
mm.o:	include/mm.h
