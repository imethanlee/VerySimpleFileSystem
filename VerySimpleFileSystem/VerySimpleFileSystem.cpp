#include "util.h"

class DiskSystem {
public:
	DiskSystem() {
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
		char* tmp_data_bitmap = new char[NUM_BLOCKS];
		memcpy(data_bitmap, tmp_data_bitmap, NUM_BLOCKS * sizeof(char));
		memset(data_bitmap, '0', NUM_BLOCKS * sizeof(char));
		delete[] tmp_data_bitmap;


		/* 初始化i-node */
		inodes = (INode*)(system_start_addr + OFFSET_INODES * BLOCK_SIZE);
		INode* tmp_inodes = new INode[NUM_INODES];
		memcpy(inodes, tmp_inodes, NUM_INODES * sizeof(INode));
		delete[] tmp_inodes;

		/* 设置data_block起始地址 */
		data_start_addr = system_start_addr + OFFSET_DATA * BLOCK_SIZE;
	}

	~DiskSystem() {
		// 备份系统

		// 释放空间
		free(system_start_addr);
	}

	void systemReload() {
		// 从备份文件中读取
	}

	char* getSystemStartAddr() {
		return system_start_addr;
	}

	void getSuperBlock() {

	}

	char* getDataBlockAddrFromID(int block_id) {
		return data_start_addr + (block_id * BLOCK_SIZE);
	}

	int getDataBlockIDFromAddr(char* block_addr) {
		return (block_addr - data_start_addr) / BLOCK_SIZE;
	}

	int getFreeINode() {
		return 0;
	}

public:
	char* system_start_addr;	// 整个系统起始地址
	SuperBlock* super_block;	// super block地址
	char* inode_bitmap;			// i-node bitmap地址
	char* data_bitmap;			// data block bitmap地址
	INode* inodes;				// i-node地址
	char* data_start_addr;		// data block地址
};


class FileManager {
public:
	FileManager() {
		curr_ptr = disk.getSystemStartAddr();
	}

	~FileManager() {

	}

	void createFile(const char* file_name, const int file_size, const int multiplier) // Create a file
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

		/* 写入 */
		// memcpy_s(a, sizeof(a), &b, 2);
	}

	void deleteFile(const char* file_name); // Delete a file

	void createDirectory(const char* dir_name); // Create a directory

	void deleteDirectory(const char* dir_name); // Delete a directory

	void changeDirectory(const char* dir_name); // Change current directory

	void listAll(); // List all the files and sub-directories under current working directory

	void copyFile(); // Copy a file

	void displayUsage(); // Display the usage of storage space

	void printFileContents(); // 
private:
	DiskSystem disk;
	char* curr_ptr;
};


int main() {
	srand(time(0));
	DiskSystem ds;
	FileManager fm;



	system("pause");
}