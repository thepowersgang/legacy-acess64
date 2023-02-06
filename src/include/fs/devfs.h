/*
Acess64
 Device Filesystem
 FS_DEVFS.H
*/
#ifndef _FS_DEVFS_H
#define _FS_DEVFS_H

/**
 \struct sDevFS_Driver
 \brief DevFS Driver Entitity
*/
typedef struct sDevFS_Driver {
	tVFS_Node	Node;
	Uint	(*IOCtl)(tVFS_Node *Node, Uint Id, void *Arg);
	struct sDevFS_Driver	*Next;
} tDevFS_Driver;

extern int DevFS_AddFS(tDevFS_Driver *info);

#endif
