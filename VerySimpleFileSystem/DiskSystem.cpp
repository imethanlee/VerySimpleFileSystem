#include "DiskSystem.h"

DiskSystem::DiskSystem()
{
	/* ϵͳ����ռ� */
	system_start_addr = (char*)malloc(SYSTEM_SIZE * sizeof(char));

	/* ����super_block��ַ */
	super_block = (SuperBlock*)system_start_addr;

	/* ����inode_bitmap��ַ */
	inode_bitmap = system_start_addr + OFFSET_INODE_BITMAP * BLOCK_SIZE;
	char* tmp_inode_bitmap = new char[NUM_INODES];
	memcpy(inode_bitmap, tmp_inode_bitmap, NUM_INODES * sizeof(char));
	memset(inode_bitmap, '0', NUM_INODES * sizeof(char));
	delete[] tmp_inode_bitmap;

	/* ����data_bitmap��ַ */
	data_bitmap = system_start_addr + OFFSET_DATA_BITMAP * BLOCK_SIZE;
	char* tmp_data_bitmap = new char[NUM_BLOCKS];
	memcpy(data_bitmap, tmp_data_bitmap, NUM_BLOCKS * sizeof(char));
	memset(data_bitmap, '0', NUM_BLOCKS * sizeof(char));
	delete[] tmp_data_bitmap;


	/* ��ʼ��i-node */
	inodes = (INode*)(system_start_addr + OFFSET_INODES * BLOCK_SIZE);
	INode* tmp_inodes = new INode[NUM_INODES];
	memcpy(inodes, tmp_inodes, NUM_INODES * sizeof(INode));
	delete[] tmp_inodes;

	/* ����data_block��ʼ��ַ */
	data_start_addr = system_start_addr + OFFSET_DATA * BLOCK_SIZE;
}

DiskSystem::~DiskSystem()
{
	// ����ϵͳ

	// �ͷſռ�
	free(system_start_addr);
}

void DiskSystem::systemReload()
{
	// �ӱ����ļ��ж�ȡ
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
