#include "FileManager.h"

int main() {
	FileManager fm;
	/*cout << fm.getDisk()->inodes[0].getName() << endl;
	cout << fm.getDisk()->inodes[0].getType() << endl;
	cout << fm.getDisk()->inodes[0].getSize() << endl;
	cout << fm.getDisk()->inodes[0].getimeCreated() << endl;*/
	
	// fm.createFile("hello.dat", 1, 1024);
	// fm.createDirectory("folder1", 0);
	// fm.createDirectory("folder2", 2);
	fm.createFile("a", 1, 1024, 0);//1
	fm.createDirectory("a", 0);//2
	fm.createFile("b", 1, 1024, 0);//3
	fm.createFile("a", 1, 1024, 2);//4
	fm.createDirectory("a", 2);//5
	fm.createFile("b", 1, 1024, 2);//6
	fm.createFile("a", 1, 1024, 5);//7
	fm.createDirectory("a", 5);//8
	fm.createFile("b", 1, 1024, 5);//9
	fm.createFile("a", 1, 1024, 8);//10
	fm.createDirectory("a", 8);//11
	fm.createFile("b", 1, 1024, 8);//12
	fm.listAll(0);
	fm.listAll(2);
	// int result = fm.getNode("/a/a/a/d", "FILE");
	// cout << "Get Node: " << -result - 1 << endl;
	fm.changeDirectory2("/a/a/a");
	fm.changeDirectory2("..");
	fm.listAll(8);
}