#include "FileManager.h"

FileManager::FileManager()
{
	curr_dir_inode_id = 0;

}

FileManager::~FileManager()
{

}

int FileManager::getNode(const char* path, const char* type)
{
	// Return >= NUM_INODES: No such file or directory
	// 返回值<0时, (-返回值-1)为上一个找得到的文件夹inode_id
	int current_inode_id; // 开始向下寻找的inode
	if (!strncmp(path, "/", 1)) // 输入绝对地址 /dir1
		current_inode_id = 0;
	else if (!strncmp(path, "..", 2)) // 输入相对地址 ../dir1
		current_inode_id = disk.inodes[curr_dir_inode_id].getParentINodeID();
	else current_inode_id = curr_dir_inode_id; // 输入相对地址 ./dir1或当前目录下文件
	
	// 把path中各级目录名提取出来
	char* buf;
	char tpath[NUM_INODES * 20] = "";
	strcpy_s(tpath, path);
	char* lpath[NUM_INODES]; // 存储各级目录名和文件名
	lpath[0] = strtok_s(tpath, "/", &buf);
	int length = 0; // 目录的级数
	while (lpath[length])
	{
		// cout << length << ": " << lpath[length] << endl;
		length++;
		lpath[length] = strtok_s(NULL, "/", &buf);
	}
	int last_dir = length - 1; // 记录路径中最后一个目录的位置
	if (!strcmp(type, "FILE")) last_dir = length - 2;

	// cout << "length: " << length << endl;
	for (int i = 0; i < length; ++i)
	{
		// 不处理目录为.或..
		if (!(!strcmp(lpath[i], ".") || !strcmp(lpath[i], "..")))
		{
			for (int j = 1; j < NUM_INODES; ++j)
			{
				if (disk.inodes[j].getParentINodeID() == current_inode_id && disk.getINodeBitmap(j) == 1)
				{
					// 名字相同
					if (!strcmp(disk.inodes[j].getName().data(), lpath[i]))
					{
						if (!strcmp(disk.inodes[j].getType().data(), "DIR"))
						{
							current_inode_id = j;
							break;
						}
						if (i == last_dir + 1) // 到最后一级文件名
						{
							return j;
						}
					}
				}
				else if (j == NUM_INODES - 1) // 到最后一个inode还没找到
				{
					if (i == last_dir + 1 && !strcmp(type, "FILE")) // 到最后一级文件名没有找到符合的, 返回(-1-上一级目录的id)
					{
						return -1 - current_inode_id;
					}
					if (i <= last_dir && !strcmp(type, "FILE")) // 倒数第二级或之前的目录找不到, 返回值>NUM_INODES表示路径不正确
					{
						return NUM_INODES + 1;
					}
					if (i == last_dir && !strcmp(type, "DIR")) // 到最后一级文件夹名没有找到符合的, 返回(-1-上一级目录的id)
					{
						return -1 - current_inode_id;
					}
					if (i < last_dir && !strcmp(type, "DIR")) // 倒数第二级或之前的目录找不到, 返回值>NUM_INODES表示路径不正确
					{
						return NUM_INODES + 1;
					}
				}
			}
		}
	}
	return current_inode_id;
}

void FileManager::createFileHelp(const int file_size,const char* str, const char* file_name, const int parent_inode_id)
{
	
	int totalBlockNum = (int)ceil((double)file_size / (double)BLOCK_SIZE);
	int lastBlockSpace = file_size % BLOCK_SIZE;

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

void FileManager::createFile2(const char* file_name, const int file_size)
{
	/* TODO: 同目录下，查重名文件 END*/
	/* 参考思路
	** 根据curr_dir_inode_id，查找当前目录下的存在的文件
	** 当前目录下的文件满足3个条件:
	** 1. disk.inodes[inode_id].getParentINodeID() == curr_dir_inode_id
	** 2. disk.getINodeBitmap(inode_id) == 1
	** 3. disk.inodes[inode_id].getType() == "FILE"
	** 在符合以上3个条件的inode中，使用getName()获取name
	** 最后查重即可
	*/
	
	const int result = getNode(file_name, "FILE");
	if (result >= NUM_INODES)
	{
		// 找不到路径中的文件夹名
		cout << "createFile: " << file_name << ": Unsuccessful operation. No such directory" << endl;
	}
	else if (result >= 0)
	{
		// 文件已经存在
		cout << "createFile: " << file_name << ": Unsuccessful operation. File has been existed" << endl;
	}
	else
	{
		// 把file_name中各级目录名提取出来
		char* buf;
		char tpath[NUM_INODES * 20] = "";
		strcpy_s(tpath, file_name);
		char* lpath[NUM_INODES]; // 存储各级目录名和文件名
		lpath[0] = strtok_s(tpath, "/", &buf);
		int length = 0; // 目录的级数
		while (lpath[length])
		{
			cout << length << ": " << lpath[length] << endl;
			length++;
			lpath[length] = strtok_s(NULL, "/", &buf);
		}

		const int parent_inode_id = -result - 1;
		
		//file_name及inode各参数未设置

		const int multiplier = 1024;
		// int parent_inode_id = curr_dir_inode_id;
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

		createFileHelp(file_size_byte, str, lpath[length - 1], parent_inode_id);
		cout << "createFile: " << file_name << ": Create file successfully" << endl;
	}
}

void FileManager::deleteFile2(const char* file_name)
{
	/* TODO: 文件名称 -> file_inode_id 的映射 END*/
	/* 参考思路
	** 思路基本和createFile一致
	** 也是在满足3个条件的inode里面去查name
	*/

	const int result = getNode(file_name, "FILE");
	if (result <= 0)
	{
		// 找不到文件
		cout << "deleteFile: " << file_name << ": Unsuccessful operation. No such file" << endl;
	}
	else if (result >= NUM_INODES)
	{
		// 找不到路径中的文件夹名
		cout << "deleteFile: " << file_name << ": Unsuccessful operation. No such directory" << endl;
	}
	else
	{
		const int file_inode_id = result;
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
		cout << "deleteFile: " << file_name << ": Delete file successfully" << endl;
	}
}

void FileManager::createDirectory2(const char* dir_name)
{
	/* TODO: 同目录下，查重名子目录 END*/
	/* 参考思路
	** 根据curr_dir_inode_id，查找当前目录下的存在的子目录
	** 当前目录下的文件满足3个条件:
	** 1. disk.inodes[inode_id].getParentINodeID() == curr_dir_inode_id
	** 2. disk.getINodeBitmap(inode_id) == 1
	** 3. disk.inodes[inode_id].getType() == "DIR"
	** 在符合以上3个条件的inode中，使用getName()获取name
	** 最后查重即可
	*/
	const int result = getNode(dir_name, "DIR");
	if (result >= NUM_INODES)
	{
		// 找不到路径中的文件夹名
		cout << "createDir: " << dir_name << ": Unsuccessful operation. No such directory" << endl;
	}
	else if (result >= 0)
	{
		// 文件夹已经存在
		cout << "createDir: " << dir_name << ": Unsuccessful operation. Directory has been existed" << endl;
	}
	else
	{
		// 把dir_name中各级目录名提取出来
		char* buf;
		char tpath[NUM_INODES * 20] = "";
		strcpy_s(tpath, dir_name);
		char* lpath[NUM_INODES]; // 存储各级目录名和文件名
		lpath[0] = strtok_s(tpath, "/", &buf);
		int length = 0; // 目录的级数
		while (lpath[length])
		{
			cout << length << ": " << lpath[length] << endl;
			length++;
			lpath[length] = strtok_s(NULL, "/", &buf);
		}
		
		const int parent_inode_id = -result - 1;
		int dir_inode_id = disk.getFreeINodeID();
		disk.initINode(dir_inode_id, "DIR", lpath[length - 1], getCurrTime().c_str(), -1, parent_inode_id);
		cout << "createDir: " << dir_name << ": Create directory successfully" << endl;
	}
}

void FileManager::deleteDirectory2(const char* dir_name)
{
	/* TODO: dir_name -> dir_inode_id 的映射 END */
	/* 参考思路
	** 思路基本和createDirectory一致
	** 也是在满足3个条件的inode里面去查name
	*/
	const int result = getNode(dir_name, "DIR");
	if (result == 0)
	{
		// 根目录
		cout << "deleteDir: " << dir_name << ": Unsuccessful operation. Cannot delete directory /" << endl;
	}
	else if (result == curr_dir_inode_id)
	{
		// 当前工作目录
		cout << "deleteDir: " << dir_name << ": Unsuccessful operation. Cannot delete current working directory" << endl;
	}
	else if (result >= NUM_INODES || result < 0)
	{
		// 找不到路径中的文件夹名
		cout << "deleteDir: " << dir_name << ": Unsuccessful operation. No such directory" << endl;
	}
	else
	{
		const int dir_inode_id = result;

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
				// 目录使用inode的置0
				disk.setINodeBitmap(curr, 0); // inode

				// 目录下的文件/子目录入栈
				for (int i = 0; i < NUM_INODES; ++i) {
					if (disk.getINodeBitmap(i) == 1 && disk.inodes[i].getParentINodeID() == curr) {
						s.push(i);
					}
				}
			}
		}
		cout << "deleteDir: " << dir_name << ": Delete directory successfully" << endl;
	}
}

void FileManager::changeDirectory2(const char* dir_name)
{
	/* TODO: 目录名称 -> dir_inode_id的映射 END*/
	/* 参考思路
	** 和createDirectory基本一致
	** 根据输入的目录名称查是否存在此目录
	*/
	const int result = getNode(dir_name, "DIR");
	if (result >= NUM_INODES || result < 0)
	{
		// 找不到路径中的文件夹名
		cout << "changeDir: " << dir_name << ": Unsuccessful operation. No such directory" << endl;
	}
	else
	{
		const int dir_inode_id = result;
		curr_dir_inode_id = dir_inode_id;
		cout << "changeDir: " << dir_name << ": Change current working directory successfully" << endl;
		cout << "Current working directory: " << curr_dir_inode_id << endl;
	}
}

void FileManager::createFile(const char* file_name, const int file_size, const int multiplier, const int parent_inode_id)
{
	/* TODO: 同目录下，查重名文件 */
	/* 参考思路
	** 根据curr_dir_inode_id，查找当前目录下的存在的文件
	** 当前目录下的文件满足3个条件:
	** 1. disk.inodes[inode_id].getParentINodeID() == curr_dir_inode_id
	** 2. disk.getINodeBitmap(inode_id) == 1
	** 3. disk.inodes[inode_id].getType() == "FILE"
	** 在符合以上3个条件的inode中，使用getName()获取name
	** 最后查重即可
	*/

	//file_name及inode各参数未设置

	// int parent_inode_id = curr_dir_inode_id;
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

	createFileHelp(file_size_byte, str, file_name, parent_inode_id);
}

void FileManager::deleteFile(const int file_inode_id)
{
	/* TODO: 文件名称 -> file_inode_id 的映射 */
	/* 参考思路
	** 思路基本和createFile一致
	** 也是在满足3个条件的inode里面去查name
	*/


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
	/* TODO: 同目录下，查重名子目录 */
	/* 参考思路
	** 根据curr_dir_inode_id，查找当前目录下的存在的子目录
	** 当前目录下的文件满足3个条件:
	** 1. disk.inodes[inode_id].getParentINodeID() == curr_dir_inode_id
	** 2. disk.getINodeBitmap(inode_id) == 1
	** 3. disk.inodes[inode_id].getType() == "DIR"
	** 在符合以上3个条件的inode中，使用getName()获取name
	** 最后查重即可
	*/
	int dir_inode_id = disk.getFreeINodeID();
	disk.initINode(dir_inode_id, "DIR", dir_name, getCurrTime().c_str(), -1, parent_inode_id);
}

void FileManager::deleteDirectory(const int dir_inode_id)
{
	/* TODO: dir_name -> dir_inode_id 的映射 */
	/* 参考思路
	** 思路基本和createDirectory一致
	** 也是在满足3个条件的inode里面去查name
	*/

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
			// 目录使用inode的置0
			disk.setINodeBitmap(curr, 0); // inode

			// 目录下的文件/子目录入栈
			for (int i = 0; i < NUM_INODES; ++i) {
				if (disk.getINodeBitmap(i) == 1 && disk.inodes[i].getParentINodeID() == curr) {
					s.push(i);
				}
			}
		}
	}
}

void FileManager::changeDirectory(const int dir_inode_id)
{
	/* TODO: 目录名称 -> dir_inode_id的映射 */
	/* 参考思路
	** 和createDirectory基本一致
	** 根据输入的目录名称查是否存在此目录
	*/
	curr_dir_inode_id = dir_inode_id;
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
	
	/* 格式化输出 */
	cout << endl;
	cout << left
		<< setw(20) << "Name"
		<< setw(10) << "Type"
		<< setw(10) << "Size"
		<< "Time Created"
		<< endl;
	for (int i = 0; i < 59; ++i) {
		cout << "-";
	}
	cout << endl;
	for (int i = 0; i < name_list.size(); ++i) {
		cout << left
			<< setw(20) << name_list[i]
			<< setw(10) << type_list[i]
			<< setw(10) << size_list[i]
			<< time_created_list[i]
			<< endl;
	}
}

void FileManager::copyFile(const int file1_inode_id, const char* file2_name )
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
	//cout << endl;
	//cout << "string in file1: " << endl;
	//cout << str << endl;
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
