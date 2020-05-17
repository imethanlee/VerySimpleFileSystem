#pragma once
#include"DiskSystem.h"

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

	void printFileContents(int inode_id) // Print out the file contents
	{

	}

private:
	DiskSystem disk;
	char* curr_ptr;
};

