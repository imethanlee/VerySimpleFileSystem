#pragma once
#include"DiskSystem.h"

using namespace std;

class FileManager {
public:
	FileManager();
	~FileManager();
	int getNode(const char* path, const char* type); // Get inode_id according to path
	void createFile(const char* file_name, const int file_size, const int multiplier, const int parent_inode_id); // Create a file
	void createFileHelp(const int file_size, const char* str, const char* file_name, const int parent_inode_id);
	void deleteFile(const int file_inode_id); // Delete a file
	void createDirectory(const char* dir_name, const int parent_inode_id); // Create a directory
	void deleteDirectory(const int dir_inode_id); // Delete a directory
	void changeDirectory(const int dir_inode_id); // Change current directory

	void createFile2(const char* file_name, const int file_size); // Create a file
	void deleteFile2(const char* file_name); // Delete a file
	void createDirectory2(const char* dir_name, bool nested = false); // Create a directory
	void deleteDirectory2(const char* dir_name); // Delete a directory
	void changeDirectory2(const char* dir_name); // Change current directory
	void copyFile2(const char* file_name_1, const char* file_name_2); // Copy a file
	stack<string> getCurrentPath(); // Get current working absolute path
	void listAll2(); // List all the files and sub-directories under current working directory
	void printFileContents2(const char* file_name); // Print out the file contents
	
	void listAll(const int dir_inode_id); // List all the files and sub-directories under current working directory
	void copyFile(const int file1_inode_id, const char* file_name_2); // Copy a file
	string readFileHelp(const int inode_id);
	void displayUsage(); // Display the usage of storage space
	void printFileContents(const int inode_id); // Print out the file contents
	DiskSystem* getDisk();
private:
	DiskSystem disk;
	int curr_dir_inode_id;
};

