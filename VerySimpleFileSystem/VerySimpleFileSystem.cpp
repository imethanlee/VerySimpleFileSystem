#include "FileManager.h"

int main() {
	FileManager fm;
	/*cout << fm.getDisk()->inodes[0].getName() << endl;
	cout << fm.getDisk()->inodes[0].getType() << endl;
	cout << fm.getDisk()->inodes[0].getSize() << endl;
	cout << fm.getDisk()->inodes[0].getimeCreated() << endl;*/
	
	fm.createFile("fileName", 12,1024);
	cout << "data bitmap(create file1): " << endl;
	int count = 0;
	for (int i = 0; i < 20; i++)
	{
		if (fm.getDisk()->data_bitmap[i] == '1')
		{
			count++;
		}
	}
	cout << count << endl;
	cout << '\n';
	cout << "inode bitmap(create file1): " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << fm.getDisk()->inode_bitmap[i];
	}

	fm.displayUsage();
	cout << "data bitmap(delete): " << endl;
	int count1 = 0;
	for (int i = 0; i < 20;i++)
	{
		if (fm.getDisk()->data_bitmap[i] == '1')
		{
			count1++;
		}
	}
	cout << count1 << endl;
	cout << '\n';
	cout << "inode bitmap(delete): " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << fm.getDisk()->inode_bitmap[i];
	}
	
	system("pause");
}