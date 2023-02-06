; AcessOS 64
; Device FS
; FS_DEVFS.ASM
[BITS 64]
; === Includes ===
%include "include/macros.inc.asm"
%include "include/vfs.inc.asm"

; === Exports ===
[global _VFS_Install]
[global _VFS_AddFS]
[global _VFS_Open]

; === Imports ===
[extern _strcmp]

; === Variables ===
[section .data]
_VFS_FileSystems:	dq	0	; Filesystem Linked list
_VFS_Mounts:	dq	0	; Mount Point linked list

; === Code ===
[section .text]
; --------------------
; void VFS_Install();
; - Install the VFS Layer
_VFS_Install:
	ret

; --------------------
; int VFS_AddFS(struct vfs_driver *info);
; - Register a filesystem with the VFS
_VFS_AddFS:
	INIT_FRAME 0, {rbx}
	mov rdx, rdi
	test rdx, rdx, 
	jz	.err_ret
	
	mov rdi, QWORD [rdx+vfs_driver.name]
	mov rbx, QWORD [QWORD _VFS_FileSystems]
	; Search for collision with names
.loop:
	test rbx, rbx
	jz	.add
	
	mov rsi, QWORD [rbx+vfs_driver.name]
	SAVE	rdx
	call	_strcmp	; strcmp(rsi, rdi)
	RESTORE	rdx
	; Check strings
	test rax, rax
	jz	.err_ret
	; Get next entry
	mov rbx, QWORD [rbx+vfs_driver.next]
	jmp .loop

.endloop:
	mov rbx, QWORD [QWORD _VFS_FileSystems]
	test rbx, rbx
	jz .add
.add:
	mov QWORD [QWORD _VFS_FileSystems], rdx
.ret:
	CLOSE_FRAME	{rbx}
	ret
.err_ret:
	mov rax, -1
	jmp .ret
	
; --------------------
; int VFS_Open(char *path, Uint flags);
; - Open a file
_VFS_Open:
	ret
