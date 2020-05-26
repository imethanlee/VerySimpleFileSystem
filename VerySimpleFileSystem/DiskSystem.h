#pragma once
#include "util.h"

using namespace std;

class DiskSystem {
public:
	DiskSystem();		// 构造函数
	~DiskSystem();		// 析构函数
	void systemReload();	// 从磁盘中恢复数据
	char* getSystemStartAddr(); // 获取系统起始地址
	char* getDataBlockAddrByID(int block_id);	// 根据ID获取Data Block地址
	int getDataBlockIDByAddr(char* block_addr);	// 根据地址获取Data BlockID
	INode* getINodeAddrByID(int inode_id);		// 根据ID获取I-Node地址
	void setINodeBitmap(const int id, const int value); // 对I-Node Bitmap赋值
	int getINodeBitmap(const int id);	// 获取I-Node BItmap的值
	void setDataBlockBitmap(const int id, const int value); // 对Data Block Bitmap赋值
	int getDataBlockBitmap(const int id);	// 获取Data Block Bitmap的值
	int getFreeINodeID();	// 获取空闲I-Node ID
	int getFreeDataBlockID(); // 获取空闲Data Block ID
	void initINode(const int inode_id, string type, const char* name, const char* time_created, const int size, const int parent_inode_id);
private:
	void welcomeMessage();
public:
	INode* inodes;				// i-node地址
private:
	char* system_start_addr;	// 整个系统起始地址
	SuperBlock* super_block;	// super block地址
	char* inode_bitmap;			// i-node bitmap地址
	char* data_bitmap;			// data block bitmap地址
	char* data_start_addr;		// data block地址
};
