#include "FileManager.h"

int main() {
	FileManager fm;
	/*cout << fm.getDisk()->inodes[0].getName() << endl;
	cout << fm.getDisk()->inodes[0].getType() << endl;
	cout << fm.getDisk()->inodes[0].getSize() << endl;
	cout << fm.getDisk()->inodes[0].getimeCreated() << endl;*/
	
	fm.createFile("hello.dat", 1, 1024);
	fm.createDirectory("folder");
	fm.listAll(0);
	
}