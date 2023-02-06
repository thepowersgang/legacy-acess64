; Acess64
;  Memory/Port IO Libraries
;  LIB.ASM
[BITS 64]

; CODE
[global _memcpyqa]
[global _memsetqa]
[global _memcpyda]
[global _memsetda]
_memcpyqa:
	mov rcx, rdx
	cld
	rep movsq
	ret

_memsetqa:
	mov rcx, rdx
	mov rax, rsi
	cld
	rep stosq
	ret

_memcpyda:
	mov rcx, rdx
	cld
	rep movsd
	ret

_memsetda:
	mov rcx, rdx
	mov rax, rsi
	cld
	rep stosd
	ret
