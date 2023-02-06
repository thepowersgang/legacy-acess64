/*
Acess64
 Device Filesystem
 FS_DEVFS_INT.H
*/
#ifndef _FS_DEVFS_INT_H
#define _FS_DEVFS_INT_H

/**
 \struct sDevFS_Link
 \brief DevFS Symbolic Link Structure
*/
typedef struct sDevFS_Link {
	tVFS_Node	Node;
	struct sDevFS_Link	*Next;
} tDevFS_Link;

#endif
