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
	//cout << str << endl;

	/* 获取文件创建时间 */
	string curr_time_str = getCurrTime();
	char curr_time[20];
	strcpy_s(curr_time, curr_time_str.c_str());
	//cout << curr_time << endl;
	//system("pause");

	int totalBlockNum = (int)ceil(file_size_byte / BLOCK_SIZE);
	int lastBlockSpace = file_size_byte % BLOCK_SIZE;
	cout << "totalBlockNum: "<<totalBlockNum << endl;
	cout << "lastBlockSpace: " << lastBlockSpace << endl;
	cout << "sizeof(str): " << sizeof(str) << endl;

	//read inode bitmap for free iniode block
	int freeINid = disk.getFreeINodeID();
	INode* inptr = disk.getINodeAddrByID(freeINid);
	//build indirect pointer in inode
	int indirDBid = 0;
	char* indirPtr = NULL;
	if (totalBlockNum > 10)
	{
		
		//find a data block for indirect table
		indirDBid = disk.getFreeDataBlockID();
		indirPtr = disk.getDataBlockAddrByID(indirDBid);
		inptr->setIndirect(indirDBid);
		disk.setDataBlockBitmap(indirDBid,1);
		cout << "find a data block for indirect table, data block id is: "<<indirDBid << endl;
	}
	
	//modify inode bitmap
	disk.setINodeBitmap(freeINid,1);
	cout << "modify inode bitmap" << endl;
	cout << '\n' << endl;

	for (int i = 0; i < totalBlockNum; i++)
	{
		//read data bitmap for free data block
		int freeDBid = disk.getFreeDataBlockID();
		char* ptr = disk.getDataBlockAddrByID(freeDBid);
		cout << "find a free data block: " << freeDBid << endl;
		//save in data block
		if (i < totalBlockNum - 1) 
		{
			memcpy(ptr, str + i * BLOCK_SIZE , BLOCK_SIZE);
		}
		else 
		{
			memcpy(ptr, str + i * BLOCK_SIZE, lastBlockSpace);
		}
		cout << "save in data block " << freeDBid << endl;
		//modify data bitmap
		disk.setDataBlockBitmap(freeDBid, 1);
		cout << freeDBid<<" modify data bitmap" << endl;

		//struct inode(pointers inode_block->data bolck
		//direct:
		if (i < 10)
		{
			inptr->setDirect(i, freeDBid);
			cout << i << " save into direct" << endl;
		}
		//indirct:
		else
		{
			char* phyAddr = disk.getDataBlockAddrByID(indirDBid);
			char db_ch[3];
			getCharFromInt(freeDBid, db_ch);
			memcpy(phyAddr + (i-10) * 3 * sizeof(char), db_ch, 3 * sizeof(char));
			cout << i << " save into indirect" << endl;
		}
		cout << '\n' << endl;
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
