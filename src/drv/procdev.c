/*
Acess64
 DRIVER: System State
 DRV/PROCDEV.C
*/
#include <acess.h>
#include <vfs.h>
#include <fs/devfs.h>
#include <mboot.h>

enum eProcDevFiles {
	PROCDEV_FILE_NULL,
	
	PROCDEV_FILE_PROCESS,
	
	PROCDEV_FILE_MEM_USAGE,
	PROCDEV_FILE_MEM_KMAP,
	
	PROCDEV_FILE_LAST
};

#define DEF_NODE_DIR(name, contents, length)	\
	{name,0,(Uint)&gProcDev_DrvInfo,contents, 0111, VFS_FFLAG_DIRECTORY, length,0,0,0, 0,0, NULL,NULL,NULL, ProcDev_ReadDir,ProcDev_FindDir,NULL, NULL}
#define DEF_NODE_FILE(name, id)	\
	{name,id,(Uint)&gProcDev_DrvInfo,NULL, 0555,0, 0,0,0,0, 0,0, ProcDev_ReadFile,ProcDev_WriteFile,NULL, NULL,NULL,NULL, NULL}

// === Prototypes ===
Uint	ProcDev_ReadFile(tVFS_Node *node, Uint offset, Uint length, void *buffer);
Uint	ProcDev_WriteFile(tVFS_Node *node, Uint offset, Uint length, void *buffer);
tVFS_Node	*ProcDev_ReadDir(tVFS_Node *node, Uint id);
tVFS_Node	*ProcDev_FindDir(tVFS_Node *node, char *name);

// === Locals ===
// - Root of Driver
tDevFS_Driver	gProcDev_DrvInfo = {
	{	// VFS Node
		"ProcDev", 0, 0, 0,
		0111, VFS_FFLAG_DIRECTORY,
		0, 0, 0, 0,
		0, 0,
		NULL, NULL, NULL,
		ProcDev_ReadDir, ProcDev_FindDir, NULL,
		NULL
	},
	NULL,	// No IOCtl Function
	NULL
};
// - Process List
tVFS_Node	*gpProcDev_Procs = NULL;
// - Memory Information
tVFS_Node	gaProcDev_Memory[] = {
	DEF_NODE_FILE("Usage", PROCDEV_FILE_MEM_USAGE),
	DEF_NODE_FILE("KernelMap", PROCDEV_FILE_MEM_KMAP),
};
tVFS_Node	gaProcDev_Sections[] = {
	DEF_NODE_DIR("Procs", NULL, 0),
	DEF_NODE_DIR("Memory", &gaProcDev_Memory, ARR_SIZE(gaProcDev_Memory))
};

// === Code ===
/**
 \fn void ProcDev_Initialise()
 \brief Installs System State Driver
*/
void ProcDev_Initialise()
{
	LogF("ProcDev_Initialise: ()\n");
	gProcDev_DrvInfo.Node.DataPtr = &gaProcDev_Sections;
	gProcDev_DrvInfo.Node.Size = ARR_SIZE(gaProcDev_Sections);
	DevFS_AddFS(&gProcDev_DrvInfo);
}

/**
 \fn Uint ProcDev_ReadFile(tVFS_Node *node, Uint offset, Uint length, void *buffer)
*/
Uint ProcDev_ReadFile(tVFS_Node *node, Uint offset, Uint length, void *buffer)
{
	return 0;
}

/**
 \fn Uint ProcDev_WriteFile(tVFS_Node *node, Uint offset, Uint length, void *buffer)
*/
Uint ProcDev_WriteFile(tVFS_Node *node, Uint offset, Uint length, void *buffer)
{
	return 0;
}

/**
 \fn tVFS_Node *ProcDev_ReadDir(tVFS_Node *node, Uint id)
*/
tVFS_Node *ProcDev_ReadDir(tVFS_Node *node, Uint id)
{
	return NULL;
}

/**
 \fn tVFS_Node *ProcDev_FindDir(tVFS_Node *node, char *name)
*/
tVFS_Node *ProcDev_FindDir(tVFS_Node *node, char *name)
{
	return NULL;
}

