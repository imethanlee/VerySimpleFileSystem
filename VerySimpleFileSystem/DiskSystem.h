#pragma once
#include "util.h"

class DiskSystem {
public:
	DiskSystem();
	~DiskSystem();
	void systemReload();
	char* getSystemStartAddr();
	void getSuperBlock();
	char* getDataBlockAddrByID(int block_id);
	int getDataBlockIDByAddr(char* block_addr);
	int getFreeINodeID();

public:
	char* system_start_addr;	// ����ϵͳ��ʼ��ַ
	SuperBlock* super_block;	// super block��ַ
	char* inode_bitmap;			// i-node bitmap��ַ
	char* data_bitmap;			// data block bitmap��ַ
	INode* inodes;				// i-node��ַ
	char* data_start_addr;		// data block��ַ
};
