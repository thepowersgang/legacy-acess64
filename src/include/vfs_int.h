/*
Acess64
 Virtual Filesystem Layer
 VFS_INT.H
*/
#ifndef _VFS_INT_H
#define _VFS_INT_H

/**
 \struct sVFS_Mount
 \brief VFS Mount Point
 
 NOTE: Mount points need not be directories
*/
typedef struct sVFS_Mount {
	char	*Path;		//!< Actual Mount Point
	char	*Device;	//!< Device Mounted at point
	char	*Filesystem;	//!< Filesystem Name
	char	*OptionsString;	//!< Options String Passed at mount-time
	tVFS_Node	*Root;		//!< Root Node
	struct sVFS_Mount	*Next;	//!< Next Mount Point in list
} tVFS_Mount;

/**
 \struct sVFS_Handle
 \brief VFS File Handle
*/
typedef struct sVFS_Handle {
	Uint	Flags;	//!< Open Flags [ NBRWX (No-Link, Binary, Read, Write, Execute) ]
	tVFS_Node	*Node;
	Uint	Position;
	struct sVFS_Handle	*Next;
} tVFS_Handle;

#define VFS_OPENFLAG_EXEC	0x01
#define VFS_OPENFLAG_WRITE	0x02
#define VFS_OPENFLAG_READ	0x04
#define VFS_OPENFLAG_BINARY	0x08
#define VFS_OPENFLAG_NOLINK	0x10

#endif
