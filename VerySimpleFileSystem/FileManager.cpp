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
	strcpy(curr_time, curr_time_str.c_str());

	/* write file */
	// memcpy_s(a, sizeof(a), &b, 2);
	//read data bitmap for free data block
	//save in data block
	//modify data bitmap

	//read inode bitmap for free iniode block
	//struct inode(pointers inode_block->data bolck)
	//modify inode bitmap
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
