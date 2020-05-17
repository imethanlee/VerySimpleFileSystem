#include "FileManager.h"

FileManager::FileManager()
{
	curr_ptr = disk.getSystemStartAddr();
}

FileManager::~FileManager()
{
}

void FileManager::createFile(const char* file_name, const int file_size, const int multiplier)
{
	/* 随机生成字符串 */
	char* str = new char[file_size * multiplier];
	for (int i = 0; i < file_size * multiplier - 1; ++i) {
		switch (rand() % 3)
		{
		case 0:
			str[i] = '0' + rand() % 10;
			break;
		case 1:
			str[i] = 'a' + rand() % 26;
			break;
		case 2:
			str[i] = 'A' + rand() % 26;
			break;
		default:
			cout << "create file ERROR!" << endl;
		}
	}
	str[file_size * multiplier - 1] = '\0';
	cout << str << endl;

	/* 获取文件创建时间 */
	string curr_time_str = getCurrTime();
	char* curr_time = new char[20];
	strcpy(curr_time, curr_time_str.c_str());

	/* 写入 */
	// memcpy_s(a, sizeof(a), &b, 2);
	cout << 123 << endl;
}
