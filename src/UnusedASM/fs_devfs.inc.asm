; Acess64
; Driver FS - Header
; FS_DEVFS.INC.ASM

%ifndef _FS_DEVFS_INC
%define _FS_DEVFS_INC

struc   devfs_driver
	.name	resb	32	; Name Buffer
	.node	resq	1	; VFS Node
	.ioctl	resq	1	; IOCtl Function Pointer
	.next	resq	1	; Pointer to next entry
endstruc

%endif
