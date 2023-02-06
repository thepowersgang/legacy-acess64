; AcessOS 64
;  Stage 3 Boot Loader
;  By thePowersGang

; Exported Symbols
[global start]
[global _memcpyda]
[global _memsetda]
[global _StartImage]

; Imported Symbols
[extern code]
[extern bss]
[extern end]
[extern _LoadElf64]


[BITS 32]
[section .text]
; === Multiboot Header ===
ALIGN 4
mboot:
	MBOOT_PAGE_ALIGN	equ 1<<0	; Align Kernel to page boundries
	MBOOT_MEMORY_INFO	equ 1<<1	; Provide Memory Information
	MBOOT_MAGIC	equ 0x1BADB002
	MBOOT_FLAGS	equ MBOOT_PAGE_ALIGN|MBOOT_MEMORY_INFO
	MBOOT_CHECKSUM	equ -(MBOOT_MAGIC+MBOOT_FLAGS)
	
	dd	MBOOT_MAGIC
	dd	MBOOT_FLAGS
	dd	MBOOT_CHECKSUM
	dd	mboot	; Multiboot Header Location
	dd	code	; Start of .text
	dd	bss	; Start of .bss
	dd	end	; End of kernel image

; === Kernel Entry ===
start:
	cli
	mov esp, _stack
	
	cmp eax, 0x2BADB002
	jnz	.nonMBoot
	
	;cld
	; Save Muliboot Information
	;mov ecx, 10
	;mov esi, ebx
	;mov edi, _kEndPA
	;rep movsd
	
	
	mov DWORD [giMultibootSaved], ebx	; Save Multiboot Info
	mov ebx, DWORD [ebx+24]	; Get kernel image
	push DWORD [ebx+4]	; Get kernel image end
	push DWORD [ebx]	; Get kernel image base
	
	call	_LoadElf64

	cli
.hltLoop:
	hlt
	jmp .hltLoop
; --- Not Mulitboot ---
.nonMBoot:
	mov eax, 0x606F604E ;'No'
	mov [0xB8000], eax
	mov eax, 0x60206074 ;'t '
	mov [0xB8004], eax
	mov eax, 0x6042604D ;'MB'
	mov [0xB8008], eax
	mov eax, 0x606F606F ;'oo'
	mov [0xB800C], eax
	mov eax, 0x60216074 ;'t!'
	mov [0xB8010], eax
	jmp .hltLoop

; === Memory Copy ===
_memcpyda:
	push ebp
	mov ebp, esp
	push esi
	push edi
	
	mov edi, DWORD [ebp+8]
	mov esi, DWORD [ebp+12]
	mov ecx, DWORD [ebp+16]
	
	cld
	rep movsd
	
	pop edi
	pop esi
	pop ebp
	ret

; === Memory Copy ===
_memsetda:
	push ebp
	mov ebp, esp
	push edx
	push edi
	
	mov edi, DWORD [ebp+8]
	mov edx, DWORD [ebp+12]
	mov ecx, DWORD [ebp+16]
	
	cld
	rep stosd
	
	pop edi
	pop esi
	pop ebp
	ret
	
; === Start Image ===
_StartImage:
	mov eax, 0x2BADB002
	mov ebx, DWORD [giMultibootSaved]
	mov ecx, DWORD [esp+4]
	jmp ecx
	ret

[section .data]
giMultibootSaved:	dd	0
; Stack
[section .bss]
	resb 0x20
_stack:
