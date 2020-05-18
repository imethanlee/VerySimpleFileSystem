#pragma once
#include<iostream>
#include<random>
#include<ctime>
#include<string>

using namespace std;

const int SYSTEM_SIZE = 16 * 1024 * 1024;		// System总大小
const int BLOCK_SIZE = 1 * 1024;				// Block空间大小
const int NUM_BLOCKS = 16 * 1024;				// Block总个数
const int NUM_INODES = 62;						// i-node总个数
const int ADDR_PER_INODE = 10;					// i-node中direct的个数

const int OFFSET_INODE_BITMAP = 1;
const int OFFSET_DATA_BITMAP = 2;
const int OFFSET_INODES = 18;
const int OFFSET_DATA = 24;

const int NUM_DATA_BLOCKS = NUM_BLOCKS - OFFSET_DATA;

class INode {
public:
	char type;
	char name[20];
	char time_created[20];
	int direct[ADDR_PER_INODE];
	int indirect = 999;
};

class SuperBlock {
public:
	int magic;
	int nblocks;
	int ninodeblocks;
	int ninodes;
};

static string getCurrTime() {
	time_t time_seconds = time(0);
	struct tm now_time;
	localtime_s(&now_time, &time_seconds);

	string time = to_string(now_time.tm_year + 1900)
		+ "/"
		+ to_string(now_time.tm_mon + 1)
		+ "/"
		+ to_string(now_time.tm_mday)
		+ " "
		+ to_string(now_time.tm_hour) + ":"
		+ to_string(now_time.tm_min) + ":"
		+ to_string(now_time.tm_sec);
	return time;
}

static int getIntFromChar(const char ch[3]) {
	char ch_int[4];
	int x = 0;
	memset(ch_int, 0, 4 * sizeof(char));
	memcpy(ch_int, ch, 3 * sizeof(char));
	memcpy(&x, ch_int, 3 * sizeof(char));
	return x;
}

static char* getCharFromInt(const int x) {
	return nullptr;
}