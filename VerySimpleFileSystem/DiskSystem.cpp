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
	char* tmp_data_bitmap = new char[NUM_DATA_BLOCKS];
	memcpy(data_bitmap, tmp_data_bitmap, NUM_DATA_BLOCKS * sizeof(char));
	memset(data_bitmap, '0', NUM_DATA_BLOCKS * sizeof(char));
	delete[] tmp_data_bitmap;


	/* 初始化i-node */
	inodes = (INode*)(system_start_addr + OFFSET_INODES * BLOCK_SIZE);
	INode* tmp_inodes = new INode[NUM_INODES];
	memcpy(inodes, tmp_inodes, NUM_INODES * sizeof(INode));
	delete[] tmp_inodes;

	/* 设置data_block起始地址 */
	data_start_addr = system_start_addr + OFFSET_DATA * BLOCK_SIZE;

	/* 额外初始化 */
	/* 初始化根目录/root/ */
	int root_inode_id = getFreeINodeID();
	// cout << "root" << root_inode_id << endl;
	setINodeBitmap(root_inode_id, 1);
	initINode(root_inode_id, "DIR", "root", getCurrTime().c_str(), -1, -1);
	
	/* 尝试从文件中恢复备份 */
	systemReload();

	/* 欢迎界面 */
	welcomeMessage();
}

DiskSystem::~DiskSystem()
{
	// 备份系统(All blocks)
	FILE* fp;
	errno_t err = fopen_s(&fp, "backup.dat", "w");
	fwrite(system_start_addr, sizeof(char), SYSTEM_SIZE, fp);
	fclose(fp);

	// 释放空间
	free(system_start_addr);
}

void DiskSystem::systemReload()
{
	// 从备份文件中读取
	FILE* fp;
	errno_t err = fopen_s(&fp, "backup.dat", "r");
	if (err == 0) {
		fread(system_start_addr, sizeof(char), SYSTEM_SIZE, fp);
		fclose(fp);
	}
}

char* DiskSystem::getSystemStartAddr()
{
	return system_start_addr;
}

char* DiskSystem::getDataBlockAddrByID(int block_id)
{
	return data_start_addr + (block_id * BLOCK_SIZE);
}

INode* DiskSystem::getINodeAddrByID(int inode_id)
{
	return inodes + inode_id;
}

int DiskSystem::getDataBlockIDByAddr(char* block_addr)
{
	return (block_addr - data_start_addr) / BLOCK_SIZE;
}

void DiskSystem::setINodeBitmap(const int id, const int value)
{
	if (value == 0) {
		inode_bitmap[id] = '0';
	}
	else if (value == 1) {
		inode_bitmap[id] = '1';
	}
}

int DiskSystem::getINodeBitmap(const int id)
{
	if (inode_bitmap[id] == '0') {
		return 0;
	}
	else if (inode_bitmap[id] == '1') {
		return 1;
	}
}

void DiskSystem::setDataBlockBitmap(const int id, const int value)
{
	if (value == 0) {
		data_bitmap[id] = '0';
	}
	else if (value == 1) {
		data_bitmap[id] = '1';
	}
}

int DiskSystem::getDataBlockBitmap(const int id)
{
	if (data_bitmap[id] == '0') {
		return 0;
	}
	else if (data_bitmap[id] == '1') {
		return 1;
	}
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

int DiskSystem::getFreeDataBlockID()
{
	int free_data_block_id = -1;
	for (int i = 0; i < NUM_DATA_BLOCKS; ++i) {
		if (data_bitmap[i] == '0') {
			free_data_block_id = i;
			break;
		}
	}
	return free_data_block_id;
}

void DiskSystem::initINode(const int inode_id, string type, const char* name, const char* time_created, const int size, const int parent_inode_id)
{
	inodes[inode_id].setType(type);
	inodes[inode_id].setName(name);
	inodes[inode_id].setTimeCreated(time_created);
	inodes[inode_id].setSize(size);
	inodes[inode_id].setParentINodeID(parent_inode_id);
	for (int i = 0; i < ADDR_PER_INODE; ++i) {
		inodes[inode_id].setDirect(i, -1);
	}
	inodes[inode_id].setIndirect(-1);
	setINodeBitmap(inode_id, 1);
}

void DiskSystem::welcomeMessage()
{
	cout << "*****************************************************************************" << endl;
	cout << "*                          Very Simple File System                          *" << endl;
	cout << "*                     Developer: Yuexin Li 201764621160                     *" << endl;
	cout << "*                     Developer: Jing Deng 201730700165                     *" << endl;
	cout << "*                     Developer: Liyao Li  201730613274                     *" << endl;
	cout << "*                    Copyright 2020, All Rights Reserved                    *" << endl;
	cout << "*****************************************************************************" << endl;
	cout << "*               1. Create a file: createFile <fileName> <fileSize>          *" << endl;
	cout << "*               2. Delete a file: deleteFile <fileName>                     *" << endl;
	cout << "*               3. Create a directory: createDir <dirName>                  *" << endl;
	cout << "*               4. Delete a directory: deleteDir <dirName>                  *" << endl;
	cout << "*               5. Change directory: changeDir <dirName>                    *" << endl;
	cout << "*               6. List all the files: dir                                  *" << endl;
	cout << "*               7. Copy a file: cp <exsitedFile> <newFile>                  *" << endl;
	cout << "*               8. Display usage: sum                                       *" << endl;
	cout << "*               9. Print file content: cat <filename>                       *" << endl;
	cout << "*               10. Show current absolute path: pwd                         *" << endl;
	cout << "*               11. Show this message: help                                 *" << endl;
	cout << "*               12. Shut down this system: exit                             *" << endl;
	cout << "*****************************************************************************" << endl;
}
