#include "FileManager.h"


FileManager::FileManager()
{
	curr_ptr = disk.getSystemStartAddr();
}

FileManager::~FileManager()
{
}

void FileManager::createFile(const char* file_name, const int file_size, const int multiplier)
{
	const int file_size_byte = file_size * multiplier;
	/* 随机生成字符串 */
	char* str = new char[file_size * multiplier];
	for (int i = 0; i < file_size * multiplier; ++i) {
		switch (rand() % 3)
		{
		case 0:
			str[i] = '0' + rand() % 10;
			break;
		case 1:
			str[i] = 'a' + rand() % 26;
			break;
		case 2:
			str[i] = 'A' + rand() % 26;
			break;
		default:
			cout << "create file ERROR!" << endl;
		}
	}
	str[file_size * multiplier - 1] = '\0';
	cout << str << endl;

	/* 获取文件创建时间 */
	string curr_time_str = getCurrTime();
	char curr_time[20];
	strcpy_s(curr_time, curr_time_str.c_str());
	cout << curr_time << endl;
	system("pause");

	int totalBlockNum = (int)ceil(file_size_byte / BLOCK_SIZE);
	int lastBlockSpace = file_size_byte % BLOCK_SIZE;
	cout << "totalBlockNum: "<<totalBlockNum << endl;
	cout << "lastBlockSpace: " << lastBlockSpace << endl;
	cout << "sizeof(str): " << sizeof(str) << endl;
	for (int i = 0; i < totalBlockNum; i++)
	{
		//read data bitmap for free data block
		int freeDBid = disk.getFreeDataNodeID();
		char* ptr = disk.getDataBlockAddrByID(freeDBid);
		//save in data block
		if (i < totalBlockNum - 1) 
		{
			memcpy(ptr, str + i * BLOCK_SIZE , BLOCK_SIZE);
			cout << i;
		}
		else 
		{
			memcpy(ptr, str + i * BLOCK_SIZE, lastBlockSpace);
			cout << i;
		}
		//modify data bitmap
		disk.data_bitmap[freeDBid] = 1;
		//read inode bitmap for free iniode block
		int freeINid = disk.getFreeINodeID();
		INode* inptr = disk.getINodeAddrByID(freeINid);
		//struct inode(pointers inode_block->data bolck)
		//inptr->direct[i] = disk.getDataBlockIDByAddr(ptr);
		//modify inode bitmap
		disk.data_bitmap[freeINid] = 1;
	}
}

void FileManager::deleteFile(const char* file_name)
{
	//bitmaps 1->0
}

void FileManager::createDirectory(const char* dir_name, const int parent_inode_id)
{
	int dir_inode_id = disk.getFreeINodeID();
	disk.initINode(dir_inode_id, "DIR", dir_name, getCurrTime().c_str(), -1, parent_inode_id);
}

void FileManager::deleteDirectory(const int dir_inode_id)
{
	// 递归删除（树状）
}

void FileManager::copyFile(const char* file_name1,const char* file_name2 )
{
	/*
	file1->file2:
	1.create file2
	2.read file1
	3.write file2
	*/

}

DiskSystem* FileManager::getDisk()
{
	return &disk;
}
