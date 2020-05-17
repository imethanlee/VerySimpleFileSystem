#pragma once
#include"DiskSystem.h"

class FileManager {
public:
	FileManager();
	~FileManager();
	void createFile(const char* file_name, const int file_size, const int multiplier); // Create a file
	void deleteFile(const char* file_name); // Delete a file
	void createDirectory(const char* dir_name); // Create a directory
	void deleteDirectory(const char* dir_name); // Delete a directory
	void changeDirectory(const char* dir_name); // Change current directory
	void listAll(); // List all the files and sub-directories under current working directory
	void copyFile(); // Copy a file
	void displayUsage(); // Display the usage of storage space
	void printFileContents(int inode_id); // Print out the file contents

private:
	DiskSystem disk;
	char* curr_ptr;
};

