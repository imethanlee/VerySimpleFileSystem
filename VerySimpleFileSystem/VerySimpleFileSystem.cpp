#include "FileManager.h"

int main() {
	FileManager fm;
	//fm.createFile("fileName", 1, 1024);
	INode inode;
	inode.setType("FILE");
	cout << inode.getType() << endl;

	system("pause");
}