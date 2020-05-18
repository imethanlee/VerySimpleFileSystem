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
	char* getINodeAddrByID(int inode_id);
	int getDataBlockIDByAddr(char* block_addr);
	int getFreeINodeID();
	int getFreeDataNodeID();

public:
	char* system_start_addr;	// 整个系统起始地址
	SuperBlock* super_block;	// super block地址
	char* inode_bitmap;			// i-node bitmap地址
	char* data_bitmap;			// data block bitmap地址
	INode* inodes;				// i-node地址
	char* data_start_addr;		// data block地址
};
