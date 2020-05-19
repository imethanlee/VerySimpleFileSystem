#pragma once
#include"DiskSystem.h"

class FileManager {
public:
	FileManager();
	~FileManager();
	void createFile(const char* file_name, const int file_size, const int multiplier); // Create a file
	void deleteFile(const int file_inode_id); // Delete a file
	void createDirectory(const char* dir_name, const int parent_inode_id); // Create a directory
	void deleteDirectory(const int dir_inode_id); // Delete a directory
	void changeDirectory(const char* dir_name); // Change current directory
	void listAll(const int dir_inode_id); // List all the files and sub-directories under current working directory
	void copyFile(const char* file_name_1, const char* file_name_2); // Copy a file
	void displayUsage(); // Display the usage of storage space
	void printFileContents(const int inode_id); // Print out the file contents
	DiskSystem* getDisk();
private:
	DiskSystem disk;
	int curr_dir_inode;
};

