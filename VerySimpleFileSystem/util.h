#pragma once
#include<iostream>
#include<random>
#include<ctime>
#include<string>
#include<stack>

using namespace std;

const int SYSTEM_SIZE = 16 * 1024 * 1024;		// System总大小
const int BLOCK_SIZE = 1 * 1024;				// Block空间大小
const int NUM_BLOCKS = 16 * 1024;				// Block总个数
const int NUM_INODES = 62;						// i-node总个数
const int ADDR_PER_INODE = 10;					// i-node中direct的个数
const int DIRECTORY_TABLE_SIZE = NUM_INODES;	// Direcotry table的大小

const int OFFSET_INODE_BITMAP = 1;
const int OFFSET_DATA_BITMAP = OFFSET_INODE_BITMAP + 1;
const int OFFSET_INODES = OFFSET_DATA_BITMAP + 16;
// const int OFFSET_DITRCORY_TABLE = OFFSET_INODES + 3;
const int OFFSET_DATA = OFFSET_INODES + 6;

const int NUM_DATA_BLOCKS = NUM_BLOCKS - OFFSET_DATA;

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

static void getCharFromInt(const int x, char* ch_int) {
	char ch[4];
	memset(ch, 0, 4 * sizeof(char));
	memcpy(ch, &x, 4 * sizeof(char));
	memcpy(ch_int, ch, 3 * sizeof(char));
}

class INode {
public:
	void setType(const string str) {
		if (str == "FILE") {
			type = '0';
		}
		else if (str == "DIR") {
			type = '1';
		}
		else {
			cout << "Input type error" << endl;
		}
	}
	string getType() {
		string str;
		if (type == '0') {
			str = "FILE";
		}
		else if (type == '1') {
			str = "DIR";
		}
		return str;
	}
	void setName(const char* ch) {
		memcpy(name, ch, 20 * sizeof(char));
	}
	string getName() {
		string str = name;
		return str;
	}
	void setTimeCreated(const char* ch) {
		memcpy(time_created, ch, 20 * sizeof(char));
	}
	string getimeCreated() {
		string str = time_created;
		return str;
	}
	void setSize(const int s) {
		size = s;
	}
	int getSize() {
		return size;
	}
	void setParentINodeID(const int inode_id) {
		parent_inode_id = inode_id;
	}
	int getParentINodeID() {
		return parent_inode_id;
	}
	void setDirect(const int i, const int block_id) {
		char ch[3];
		getCharFromInt(block_id, ch);
		memcpy(addr + i * 3 * sizeof(char), ch, 3 * sizeof(char));
	}
	int getDirect(const int i) {
		int addr = getIntFromChar(this->addr + i * 3 * sizeof(char));
		return addr;
	}
	void setIndirect(const int block_id) {
		char ch[3];
		getCharFromInt(block_id, ch);
		memcpy(addr + ADDR_PER_INODE * 3 * sizeof(char), ch, 3 * sizeof(char));
	}
	int getIndirect() {
		int addr = getIntFromChar(this->addr + ADDR_PER_INODE * 3 * sizeof(char));
		return addr;
	}

private:
	char type;
	char name[20];
	char time_created[20];
	int size;
	int parent_inode_id;
	char addr[(ADDR_PER_INODE + 1) * 3];
};

class DirectoryTable {
	int inode_id = -1;
	char type;
	char name[20];
	int size;
	int parent_inode_id;
};

class SuperBlock {
public:
	int magic;
	int nblocks;
	int ninodeblocks;
	int ninodes;
};