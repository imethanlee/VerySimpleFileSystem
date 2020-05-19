#include "FileManager.h"

int main() {
	FileManager fm;
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
	system("pause");
}