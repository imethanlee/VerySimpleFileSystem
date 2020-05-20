#include "FileManager.h"


FileManager::FileManager()
{
	curr_dir_inode = 0;
}

FileManager::~FileManager()
{
}

void FileManager::createFileHelp(const int file_size,const char* str, const char* file_name, const int parent_inode_id)
{
	
	int totalBlockNum = (int)ceil((double)file_size / (double)BLOCK_SIZE);
	int lastBlockSpace = file_size % BLOCK_SIZE;
	cout << "!!!" << lastBlockSpace << endl;
	/* 获取文件创建时间 */
	string curr_time_str = getCurrTime();
	char curr_time[20];
	strcpy_s(curr_time, curr_time_str.c_str());

	//read inode bitmap for free iniode block
	int freeINid = disk.getFreeINodeID();
	disk.initINode(freeINid, "FILE", file_name, curr_time, file_size, parent_inode_id);
	INode* inptr = disk.getINodeAddrByID(freeINid);
	cout << "find inode: " << freeINid << endl;

	//build indirect pointer in inode
	int indirDBid = 0;
	char* indirPtr = NULL;
	if (totalBlockNum > 10)
	{

		//find a data block for indirect table
		indirDBid = disk.getFreeDataBlockID();
		indirPtr = disk.getDataBlockAddrByID(indirDBid);
		inptr->setIndirect(indirDBid);
		disk.setDataBlockBitmap(indirDBid, 1);
		cout << "find a data block for indirect table, data block id is: " << indirDBid << endl;
	}

	//modify inode bitmap

	disk.setINodeBitmap(freeINid, 1);
	cout << "modify inode bitmap" << endl;
	cout << '\n' << endl;

	for (int i = 0; i < totalBlockNum; i++)
	{

		//read data bitmap for free data block
		int freeDBid = disk.getFreeDataBlockID();
		char* ptr = disk.getDataBlockAddrByID(freeDBid);
		cout << "find a free data block: " << freeDBid << endl;
		//save in data block
		if (lastBlockSpace != 0)
		{
			if (i < totalBlockNum - 1)
			{
				memcpy(ptr, str + i * BLOCK_SIZE, BLOCK_SIZE);
			}
			else
			{
				memcpy(ptr, str + i * BLOCK_SIZE, lastBlockSpace);
			}
		}
		else
		{	
				memcpy(ptr, str + i * BLOCK_SIZE, BLOCK_SIZE);
		}
		cout << "save in data block " << freeDBid << endl;
		//modify data bitmap
		disk.setDataBlockBitmap(freeDBid, 1);
		cout << freeDBid << " modify data bitmap" << endl;

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
			memcpy(phyAddr + (i - 10) * 3 * sizeof(char), db_ch, 3 * sizeof(char));
			cout << i << " save into indirect" << endl;
		}

		cout << '\n' << endl;
	}
}

void FileManager::createFile(const char* file_name, const int file_size, const int multiplier)
{
	//同目录下，查重名文件（待实现）
	//file_name及inode各参数未设置
	int parent_inode_id = 0;
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
	//str[file_size * multiplier - 1] = '\0';
	cout << str << endl;

	
	//cout << curr_time << endl;
	//system("pause");

	createFileHelp(file_size_byte, str,file_name,parent_inode_id);
	
}

void FileManager::deleteFile(const int file_inode_id)
{
	//use filename to find file-id	
	disk.setINodeBitmap(file_inode_id, 0);
	
	for (int i = 0; i < 11; i++)
	{
		if (i < 10) 
		{
			int file_data_id = disk.inodes[file_inode_id].getDirect(i);
			disk.setDataBlockBitmap(file_data_id, 0);
		}
		if (i >= 10)
		{
			int indir_db_id = disk.inodes[file_inode_id].getIndirect();
			char* phyAddr = disk.getDataBlockAddrByID(indir_db_id);

			for (int j = 0; j < (int)ceil(BLOCK_SIZE / 3); j++)
			{
				char ch[3];
				memcpy(ch, phyAddr + j * 3 * sizeof(char), 3 * sizeof(char));
				int db_id = getIntFromChar(ch);
				disk.setDataBlockBitmap(db_id, 0);
			}
			disk.setDataBlockBitmap(indir_db_id, 0);
		}
	}
}

void FileManager::createDirectory(const char* dir_name, const int parent_inode_id)
{
	int dir_inode_id = disk.getFreeINodeID();
	disk.initINode(dir_inode_id, "DIR", dir_name, getCurrTime().c_str(), -1, parent_inode_id);
}

void FileManager::deleteDirectory(const int dir_inode_id)
{
	// 递归删除（树状)
	stack<int> s;
	s.push(dir_inode_id);

	while (!s.empty()) {
		int curr = s.top();
		s.pop();

		// 处理文件
		if (disk.inodes[curr].getType() == "FILE") {
			// deleteFile();
		}
		// 处理目录
		else if (disk.inodes[curr].getType() == "DIR") {
			// 目录使用inode和data_block的置0
			disk.setINodeBitmap(curr, 0); // inode
			/*
			for (int i = 0; i < ADDR_PER_INODE; ++i) { // direct
				int direct = disk.inodes[curr].getDirect(i);
				if (direct == -1) {
					break;
				}
				else {
					disk.setDataBlockBitmap(direct, 0);
				}
			}
			int indirect = disk.inodes[curr].getIndirect(); // indirect
			if (indirect != -1) {
				disk.setDataBlockBitmap(indirect, 0);
				const char* data_block_addr = disk.getDataBlockAddrByID(indirect);
				for (int i = 0; i < BLOCK_SIZE / 3; ++i) { // ?
					int id = getIntFromChar(data_block_addr + i * 3);
					if (id == -1) {
						break;
					}
					else {
						disk.setDataBlockBitmap(id, 0);
					}
				}
			}
			*/

			// 目录下的文件/子目录入栈
			for (int i = 0; i < NUM_INODES; ++i) {
				if (disk.getINodeBitmap(i) == 1 && disk.inodes[i].getParentINodeID() == curr) {
					s.push(i);
				}
			}
		}
	}
}

void FileManager::listAll(const int dir_inode_id)
{
	vector<string> name_list;
	vector<string> type_list;
	vector<string> size_list;
	vector<string> time_created_list;

	/* 将父目录添加进Vector */
	if (dir_inode_id != 0){
		int parent = disk.inodes[dir_inode_id].getParentINodeID();
		name_list.push_back("..");
		type_list.push_back(disk.inodes[parent].getType());
		size_list.push_back("-");
		time_created_list.push_back(disk.inodes[parent].getimeCreated());
	}

	/* 搜索当前目录下的所有文件 */
	for (int i = 0; i < NUM_INODES; ++i) {
		if (disk.getINodeBitmap(i) == 1 && 
			disk.inodes[i].getParentINodeID() == dir_inode_id) {
			name_list.push_back(disk.inodes[i].getName());
			type_list.push_back(disk.inodes[i].getType());
			if (disk.inodes[i].getType() == "FILE") {
				string str = to_string(disk.inodes[i].getSize());
				size_list.push_back(str);
			}
			else if (disk.inodes[i].getType() == "DIR") {
				size_list.push_back("-");
			}
			time_created_list.push_back(disk.inodes[i].getimeCreated());
		}
	}
	
	
}

void FileManager::copyFile(const int file1_inode_id,const char* file2_name )
{
	/*
	file1->file2:
	1.read file1
	2.create and write file2
	*/
	int parent_inode_id = 0;
	string str = readFileHelp(file1_inode_id);
	const char* ch = str.c_str();
	int file2_size = disk.inodes[file1_inode_id].getSize();
	createFileHelp(file2_size, ch,file2_name,parent_inode_id);
}

string FileManager::readFileHelp(const int inode_id)
{
	string str;
	int file_size = disk.inodes[inode_id].getSize();
	int db_num = (int)ceil((double)file_size / (double)BLOCK_SIZE);
	int lastBlockSpace = file_size % BLOCK_SIZE;
	for (int i = 0; i < db_num; i++)
	{
		if (i < 10)
		{
			int file_data_id = 0;
			char* phyAddr = NULL;

			file_data_id = disk.inodes[inode_id].getDirect(i);
			phyAddr = disk.getDataBlockAddrByID(file_data_id);
			if (lastBlockSpace != 0)
			{
				if (i < db_num - 1) 
				{
					char ch[1025];
					memcpy(ch, phyAddr, 1024);
					ch[1024] = '\0';
					str += ch;
				}
				else {
					char* ch = new char[lastBlockSpace + 1];
					memcpy(ch, phyAddr, lastBlockSpace);
					ch[lastBlockSpace] = '\0';
					str += ch;
					delete[] ch;
				}
			}
			else
			{
				char ch[1025];
				memcpy(ch, phyAddr, 1024);
				ch[1024] = '\0';
				str += ch;
			}
			
		}
		else
		{
			
			int indir_db_id = disk.inodes[inode_id].getIndirect();
			char* phyAddr = disk.getDataBlockAddrByID(indir_db_id);
			for (int j = 0; j < db_num-10; j++)
			{
				int indir_db_id = getIntFromChar(phyAddr + j * 3);
				char* indir_db_addr = disk.getDataBlockAddrByID(indir_db_id);
				if (lastBlockSpace != 0)
				{
					if (j < db_num - 11) 
					{
						char ch[1025];
						memcpy(ch, indir_db_addr, 1024);
						ch[1024] = '\0';
						str += ch;
					}
					else 
					{
						char* ch = new char[lastBlockSpace + 1];
						memcpy(ch, indir_db_addr, lastBlockSpace);
						ch[lastBlockSpace] = '\0';
						str += ch;
						delete[] ch;
					}
				}
				else
				{
					char ch[1025];
					memcpy(ch, indir_db_addr, 1024);
					ch[1024] = '\0';
					str += ch;
				}
				
			}
		}
	}
	cout << endl;
	cout << "string in file1: " << endl;
	cout << str << endl;
	return str;
}
	
		
void FileManager::displayUsage()
{
	int count = 0;
	for (int i = 0; i < NUM_DATA_BLOCKS;i++)
	{
		if (disk.getDataBlockBitmap(i) == 1) count++;
	}

	double usage = (double)(count+OFFSET_DATA) / (double)NUM_BLOCKS;
	cout << endl;
	cout << "usage is: "<<usage * 100 << "%" << endl;
}

void FileManager::printFileContents(int inode_id)
{
	string str = readFileHelp(inode_id);
	cout << str << endl;
}

DiskSystem* FileManager::getDisk()
{
	return &disk;
}
