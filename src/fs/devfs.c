/*
Acess64
 Device Filesystem
 FS_DEVFS.C
*/
#include <acess.h>
#include <vfs.h>
#include <fs/devfs.h>
#include <fs/devfs_int.h>

// === Prototypes ===
tVFS_Node	*DevFS_Mount(char *Device, int ArgCount, char *Args[]);
tVFS_Node	*DevFS_ReadDir(tVFS_Node *node, Uint id);
tVFS_Node	*DevFS_FindDir(tVFS_Node *node, char *name);
tVFS_Node	*DevFS_MkNod(tVFS_Node *node, tVFS_Node *template);

// === Globals ===
tDevFS_Driver	*gpDevFS_Drivers = NULL;
tDevFS_Link		*gpDevFS_Links = NULL;
tVFS_Node		gDevFS_RootNode = {
	"devfs", 0, 0, NULL,
	0111, VFS_FFLAG_DIRECTORY,
	0, 0, 0, 0,	// Size, Timestamps
	0, 0,		// Owner,Group
	NULL, NULL, NULL,	// Read, Write, Close
	DevFS_ReadDir, DevFS_FindDir, DevFS_MkNod,
	NULL	// Link
};
tVFS_Driver		gDevFS_VFSDrv = {
	"devfs", DevFS_Mount, NULL,
	VFS_SFLAG_T_VIRT, NULL
};

// === Code ===
/**
 \fn void DevFS_Initialise()
 \brief Installs DevFS into the VFS
*/
void DevFS_Initialise()
{
	LogF("DevFS_Initialise: ()\n");
	VFS_AddFS(&gDevFS_VFSDrv);
	LogF("DevFS_Initialise: RETURN\n");
}

/**
 \fn int DevFS_AddFS(tDevFS_Driver *info)
 \brief Adds a filesystem
 \return 0 on Sucess and negative on failure
*/
int DevFS_AddFS(tDevFS_Driver *info)
{
	tDevFS_Driver	*drv;
	
	LogF("DevFS_AddFS: (info=0x%x)\n", info);
	
	if(info == NULL || info->Node.Name == NULL)
		return -1;
	
	info->Next = NULL;
	if(gpDevFS_Drivers)
	{
		drv = gpDevFS_Drivers;
		do {
			if(strcmp(drv->Node.Name, info->Node.Name) == 0)
				return -2;
		} while(drv->Next && (drv = drv->Next));
		drv->Next = info;
	}
	else
		gpDevFS_Drivers = info;
	LogF("[DEV ] Added Driver '%s'\n", info->Node.Name);
	return 0;
}

/**
 \fn tVFS_Node *DevFS_Mount(char *device, int optionCount, char *options[])
 \brief Returns the DevFS Root
 \param devices	String - Device name (checked with "devfs")
 \param optionCount	Integer - Count of passed arguments
 \param options	String Array - Passed Options
*/
tVFS_Node *DevFS_Mount(char *device, int optionCount, char *options[])
{
	if(strcmp(device, "devfs") != 0)
		return NULL;
	return &gDevFS_RootNode;
}

/**
 \fn tVFS_Node *DevFS_ReadDir(tVFS_Node *node, Uint id)
*/
tVFS_Node *DevFS_ReadDir(tVFS_Node *node, Uint id)
{
	return NULL;
}

/**
 \fn tVFS_Node *DevFS_FindDir(tVFS_Node *node, char *name)
*/

tVFS_Node *DevFS_FindDir(tVFS_Node *node, char *name)
{
	return NULL;
}

/**
 \fn tVFS_Node *DevFS_MkNod(tVFS_Node *node, tVFS_Node *template)
*/
tVFS_Node *DevFS_MkNod(tVFS_Node *node, tVFS_Node *template)
{
	return NULL;
}
