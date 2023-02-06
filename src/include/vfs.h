/*
Acess64
 Virtual File System Layer
 VFS.H
*/
#ifndef _VFS_H
#define _VFS_H

/**
 \struct sVFS_Node
 \brief Filesystem Node Structure
*/
typedef struct sVFS_Node {
	char	*Name;	//!< Name Pointer
	Uint	Inode;	//!< Inode ID
	Uint	Impl;	//!< Implementaion Specific Data
	void	*DataPtr;	//!< Data Pointer (SymLinks)
	
	Uint16	Mode;	//!< File Mode (Permissions)
	Uint16	Flags;	//!< File Flags (Dir, RO, etc..)
	
	Uint	Size;	//!< File Size
	Uint	ATime;	//!< Last Access Time
	Uint	MTime;	//!< Last Modified Time
	Uint	CTime;	//!< Creation Time
	
	Uint	Owner;	//!< Owning User UID
	Uint	Group;	//!< Owning Group GID
	
	Uint	(*Read)(struct sVFS_Node *Node, Uint Offset, Uint Length, void *Buffer);
	Uint	(*Write)(struct sVFS_Node *Node, Uint Offset, Uint Length, void *Buffer);
	Uint	(*Close)(struct sVFS_Node *Node);
	struct sVFS_Node*	(*ReadDir)(struct sVFS_Node *Node, Uint Id);
	struct sVFS_Node*	(*FindDir)(struct sVFS_Node *Node, char *Name);
	struct sVFS_Node*	(*MkNod)(struct sVFS_Node *Node, struct sVFS_Node *Template);
	
	struct	sVFS_Node	*Link;	//!< Link Node
} tVFS_Node;

// sVFS_Node.Flags Values
#define VFS_FFLAG_READONLY	0x001
#define VFS_FFLAG_DIRECTORY	0x002

/**
 \struct sVFS_Driver
 \brief VFS Driver Structure
*/
typedef struct sVFS_Driver {
	char	*Name;	//!< Name Filesystem Driver
	tVFS_Node*	(*Mount)(char *device, int optionCount, char *options[]);	//!< Mounts a device and returns the root node handle
	int		(*Format)(char *device, int optionCount, char *options[]);	//!< Formats a file/device
	Uint	Flags;	//!< Filesystem Flags (Type, Read-Only, etc...);
	struct	sVFS_Driver	*Next;
} tVFS_Driver;

// sVFS_Driver.Flags Values
#define VFS_SFLAG_TYPE_MASK	0x0F
# define VFS_SFLAG_T_DISK	0x00
# define VFS_SFLAG_T_VIRT	0x01
# define VFS_SFLAG_T_NETWK	0x02
#define VFS_SFLAG_READONLY	0x10

// === VFS Helper Functions ===
extern int VFS_AddFS(tVFS_Driver *info);

#endif
