#pragma once
#include<iostream>
#include<random>
#include<ctime>
#include<string>

using namespace std;

#define SYSTEM_SIZE 16 * 1024 * 1024		// System总大小
#define BLOCK_SIZE 1 * 1024					// Block空间大小
#define NUM_BLOCKS 16 * 1024				// Block总个数
#define NUM_INODES	62						// i-node总个数
#define ADDR_PER_INODE 10					// i-node中direct的个数

#define OFFSET_INODE_BITMAP 1
#define OFFSET_DATA_BITMAP 2
#define OFFSET_INODES 18
#define OFFSET_DATA 24

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