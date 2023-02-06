/*
Acess64
 Virtual File System Layer
 VFS.C
*/
#include <acess.h>
#include <vfs.h>
#include <vfs_int.h>

// === Gobal Variables ===
tVFS_Driver	*gpVFS_Filesystems = NULL;
tVFS_Mount	*gpVFS_Mounts = NULL;
tVFS_Handle	*gpVFS_KernelHandles = NULL;
tVFS_Handle	*gpVFS_UserHandles = NULL;

// === Code ===
/**
 \fn int VFS_AddFS(tVFS_Driver *info)
 \brief Adds a filesystem driver to list
 \param info	Pointer to driver information
 \return 0 on success, negative otherwise
 
 Appends a driver to the VFS Driver list
 
 The `info` structure must not be touched
 by the driver after the driver is registered.
 
 Returns:
  0:  Sucess
  -1: NULL Passed
  -2: Name already taken
*/
int VFS_AddFS(tVFS_Driver *info)
{
	tVFS_Driver	*drv;
	
	if(info == NULL)	return -1;
	info->Next = NULL;
	
	if(gpVFS_Filesystems)
	{
		drv = gpVFS_Filesystems;
		do {
			if(strcmp(info->Name, drv->Name) == 0)
				return -2;
		} while(drv->Next && (drv = drv->Next));
		drv->Next = info;
	} else {
		gpVFS_Filesystems = info;
	}
	
	LogF("[VFS ] Added filesystem '%s'\n", info->Name);
	return 0;
}
