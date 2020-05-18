#include "DiskSystem.h"

DiskSystem::DiskSystem()
{
	/* 系统分配空间 */
	system_start_addr = (char*)malloc(SYSTEM_SIZE * sizeof(char));

	/* 设置super_block地址 */
	super_block = (SuperBlock*)system_start_addr;

	/* 设置inode_bitmap地址 */
	inode_bitmap = system_start_addr + OFFSET_INODE_BITMAP * BLOCK_SIZE;
	char* tmp_inode_bitmap = new char[NUM_INODES];
	memcpy(inode_bitmap, tmp_inode_bitmap, NUM_INODES * sizeof(char));
	memset(inode_bitmap, '0', NUM_INODES * sizeof(char));
	delete[] tmp_inode_bitmap;

	/* 设置data_bitmap地址 */
	data_bitmap = system_start_addr + OFFSET_DATA_BITMAP * BLOCK_SIZE;
	char* tmp_data_bitmap = new char[NUM_BLOCKS];
	memcpy(data_bitmap, tmp_data_bitmap, NUM_BLOCKS * sizeof(char));
	memset(data_bitmap, '0', NUM_BLOCKS * sizeof(char));
	delete[] tmp_data_bitmap;


	/* 初始化i-node */
	inodes = (INode*)(system_start_addr + OFFSET_INODES * BLOCK_SIZE);
	INode* tmp_inodes = new INode[NUM_INODES];
	memcpy(inodes, tmp_inodes, NUM_INODES * sizeof(INode));
	delete[] tmp_inodes;

	/* 设置data_block起始地址 */
	data_start_addr = system_start_addr + OFFSET_DATA * BLOCK_SIZE;
}

DiskSystem::~DiskSystem()
{
	// 备份系统(Data block)

	// 释放空间
	free(system_start_addr);
}

void DiskSystem::systemReload()
{
	// 从备份文件中读取
}

char* DiskSystem::getSystemStartAddr()
{
	return system_start_addr;
}

void DiskSystem::getSuperBlock()
{
	
}


char* DiskSystem::getDataBlockAddrByID(int block_id)
{
	return data_start_addr + (block_id * BLOCK_SIZE);
}

char* DiskSystem::getINodeAddrByID(int inode_id)
{
	return nullptr;
}

int DiskSystem::getDataBlockIDByAddr(char* block_addr)
{
	return (block_addr - data_start_addr) / BLOCK_SIZE;
}

int DiskSystem::getFreeINodeID()
{
	int free_node_id = -1;
	for (int i = 0; i < NUM_INODES; ++i) {
		if (inode_bitmap[i] == '0') {
			free_node_id = i;
			break;
		}
	}
	return free_node_id;
}

int DiskSystem::getFreeDataNodeID()
{
	return 0;
}
