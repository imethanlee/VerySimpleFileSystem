#include "FileManager.h"

int main() {
	FileManager fm;
	cout << fm.getDisk()->inodes[0].getName() << endl;
	cout << fm.getDisk()->inodes[0].getType() << endl;
	cout << fm.getDisk()->inodes[0].getSize() << endl;
	cout << fm.getDisk()->inodes[0].getimeCreated() << endl;
	/*
	fm.createFile("fileName", 11, 1024);
	cout << "data bitmap: " << endl;
	int count = 0;
	for (int i = 0; i < 20;i++)
	{
		if (fm.getDisk()->data_bitmap[i] == '1')
		{
			count++;
		}
	}
	cout << count << endl;
	cout << '\n';
	cout << "inode bitmap: " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << fm.getDisk()->inode_bitmap[i];
	}
	*/
	system("pause");
}