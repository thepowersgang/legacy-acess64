; Acess64
; Virtual File Syste - Header
; VFS.INC.ASM

%ifndef _VFS_INC
%define _VFS_INC

; struct vfs_node
; - Filesystem Node information structure
struc	vfs_node
	.name	resq	1	; char* - Name Pointer
	.inode	resq	1	; Uint - Inode ID
	.impl	resq	1	; Uint - Implementation Specific Data
	.dataptr	resq	1	; void* - Data Pointer (Used for symbolic links)
	.link	resq	1	; vfs_node* - Link Node (Used for symbolic links)
	
	.mode	resw	1	; Uint16 - File Mode (Permissions)
	.flags	resw	1	; Uint16 - File Flags
	
	.size	resq	1	; Uint - File Size
	.atime	resq	1	; Uint - Accessed Time (Unix Timestamp)
	.mtime	resq	1	; Uint - Modifcation Time (Unix Timestamp)
	.ctime	resq	1	; Uint - Creation Time (Unix Timestamp)
	.uid	resq	1	; Uint - User ID
	.gid	resq	1	; Uint - Group ID
	
	; Function Pointers
	.read	resq	1	; void* - Read Data
	.write	resq	1	; void* - Write Data
	.close	resq	1	; void* - Close File
	.unlink	resq	1	; void* - Remove File
	.readdir	resq	1	; void* - Read directory entry
	.finddir	resq	1	; void* - Find named directory entry
	.mknod	resq	1	; void* - Create a new file
endstruc

; Node Flags
VFS_FFLAG_READONLY	equ	0x1
VFS_FFLAG_DIRECTORY	equ	0x2

; --------------------
; struct vfs_driver
; - Filesystem Driver Information structure
struc vfs_driver
	.name	resq	1	; char* - Name of filesyste
	.mount	resq	1	; void* - Mount Device
	.flags	resq	1	; Uint - Filesystem Flags (RO,Type,...)
	.next	resq	1	; vfs_driver* - Link to next filesystem
endstruc

; Filesystem Flags
VFS_SFLAG_TYPE_MASK	equ	0x0F
 VFS_SFLAG_T_DISK	equ	0x00	; Disk Filesystem
 VFS_SFLAG_T_VIRT	equ	0x01	; Virtual Filesystem
 VFS_SFLAG_T_NETWK	equ	0x02	; Network Filesystem
VFS_SFLAG_READONLY	equ	0x10	; Read-Only Filesystem

%endif
