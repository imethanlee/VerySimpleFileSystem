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
	double file_size_byte = file_size * multiplier;
	/* 随机生成字符串 */
	char* str = new char[file_size * multiplier];
	for (int i = 0; i < file_size * multiplier - 1; ++i) {
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
	char* curr_time = new char[20];
	//strcpy(curr_time, curr_time_str.c_str());

	DiskSystem ds;
	int totalBlockNum = int (file_size_byte / BLOCK_SIZE) + 1;
	int lastBlockSpace = int(file_size_byte - (totalBlockNum - 1)) * BLOCK_SIZE;
	cout << "totalBlockNum: "<<totalBlockNum << endl;
	cout << "lastBlockSpace: " << lastBlockSpace << endl;
	cout << "sizeof(str): " << sizeof(str) << endl;
	for (int i = 0; i < totalBlockNum; i++)
	{
		//read data bitmap for free data block
		int freeDBid = ds.getFreeDataNodeID();
		char* ptr = ds.getDataBlockAddrByID(freeDBid);
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
		ds.data_bitmap[freeDBid] = 1;
		//read inode bitmap for free iniode block
		int freeINid = ds.getFreeINodeID();
		INode* inptr = ds.getINodeAddrByID(freeINid);
		//struct inode(pointers inode_block->data bolck)
		//inptr = ptr;
		//modify inode bitmap
		ds.data_bitmap[freeINid] = 1;
	}
	
	
}

void FileManager::deleteFile(const char* file_name)
{
	//bitmaps 1->0
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
