#include "FileManager.h"

int main() {
	FileManager fm;
	//fm.createFile("fileName", 1, 1024);
	cout << "data bitmap: " << endl;
	for (int i = 0; i < NUM_DATA_BLOCKS;i++)
	{
	cout << fm.getDisk()->data_bitmap[i];
	}
	cout << '\n';
	cout << "inode bitmap: " << endl;
	for (int i = 0; i < NUM_INODES; i++)
	{
		cout << fm.getDisk()->inode_bitmap[i];
	}
	system("pause");
}