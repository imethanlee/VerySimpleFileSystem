#pragma once
#include"DiskSystem.h"

using namespace std;

class FileManager {
public:
	FileManager();
	~FileManager();
	int getNode(const char* path, const char* type); // Get inode_id according to path
	void createFileHelp(const int file_size, const char* str, const char* file_name, const int parent_inode_id);
	void deleteFileHelp(const int file_inode_id); // Delete a file
	void createFile(const char* file_name, const int file_size); // Create a file
	void deleteFile(const char* file_name); // Delete a file
	void createDirectory(const char* dir_name, bool nested = false); // Create a directory
	void deleteDirectory(const char* dir_name); // Delete a directory
	void changeDirectory(const char* dir_name); // Change current directory
	void copyFile(const char* file_name_1, const char* file_name_2); // Copy a file
	stack<string> getCurrentPath(); // Get current working absolute path
	void listAll(); // List all the files and sub-directories under current working directory
	void printFileContents(const char* file_name); // Print out the file contents
	string readFileHelp(const int inode_id);
	void displayUsage(); // Display the usage of storage space
	DiskSystem* getDisk();
private:
	DiskSystem disk;
	int curr_dir_inode_id;
};

