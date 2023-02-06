; AcessOS 64
; Device FS
; FS_DEVFS.ASM
[BITS 64]
; === Includes ===
%include "include/macros.inc.asm"
%include "include/vfs.inc.asm"
%include "include/fs_devfs.inc.asm"

; === Exports ===
[global _DevFS_Install]
[global _DevFS_Add]
[global _DevFS_Remove]

; === Imports ===
[extern _strncmp]
[extern _VFS_AddFS]

; === Variables ===
[section .data]
; struct devfs_driver *_DevFS_Devices = NULL;
_DevFS_Devices:
	dq	0
_DevFS_NameStr:
	db	"devfs",0
_DevFS_RootNode:
	istruc	vfs_node
	at vfs_node.name,	dq	_DevFS_NameStr
	at vfs_node.mode,	dw	0555
	at vfs_node.flags,	dw	VFS_FFLAG_DIRECTORY
	at vfs_node.unlink,	dq	_DevFS_Unlink
	at vfs_node.readdir,	dq	_DevFS_ReadDir
	at vfs_node.finddir,	dq	_DevFS_FindDir
	at vfs_node.mknod,	dq	_DevFS_MkNod
	iend
_DevFS_DriverInfo:
	istruc	vfs_driver
	at vfs_driver.name,	dq	_DevFS_NameStr
	at vfs_driver.mount,	dq _DevFS_FSInit
	at vfs_driver.flags,	dq	VFS_SFLAG_T_VIRT
	iend

; === Code ===
[section .text]
; --------------------
; void _DevFS_Install();
; - Register device filesystem
_DevFS_Install:
	mov	rdi, _DevFS_DriverInfo
	call	_VFS_AddFS
	ret

; --------------------
; void _DevFS_Add(struct devfs_driver *driver);
; - Register a device in DevFS
_DevFS_Add:
	INIT_FRAME 0, {rbx}
	mov	rbx, rdi
	
	mov	rsi, rbx + devfs_driver.name
	mov r8, QWORD [ QWORD _DevFS_Devices ]
.searchLoop:
	; Check loop condition
	test r8, r8
	jz	.pastLoop
	
	; Get String Address
	mov	rdi, r8 + devfs_driver.name
	
	; Compare names
	SAVE	r8	; Save r8
	mov	rdx, 32
	call	_strncmp
	RESTORE	r8	; Restore r8
	
	; If match then exit with error
	test	rax, rax
	jz		.err_exist
	
	; Load next driver structure
	mov	r8, QWORD [r8+devfs_driver.next]
	; Go to start of loop
	jmp	.searchLoop
	
	; Append to driver list
.pastLoop:
	mov r8, QWORD [ QWORD _DevFS_Devices ]
	test r8, r8
	jz	.append
	mov QWORD [rbx+devfs_driver.next], r8	; Set current top to next after new
.append:
	mov QWORD [ QWORD _DevFS_Devices ], rbx	; Slot onto top of list
	
	; Set RAX = 0
	xor rax, rax
.ret:
	CLOSE_FRAME	{rbx}
	ret

.err_exist:
	; Set RAX = -1
	mov rax, -1
	jmp	.ret
	

; --- DevFS VFS Functions ---
;
_DevFS_FSInit:
_DevFS_Unlink:
_DevFS_ReadDir:
_DevFS_FindDir:
_DevFS_MkNod:
	ret: