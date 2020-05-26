#include "FileManager.h"
#include <iostream>
#include <sstream>
#include <vector>

bool checkCommandLength(vector<string>& input,const int expected_length)
{
	const int input_length = input.size();
	if (input_length != expected_length)
	{
        cout << input[0] << ": Expected " << expected_length - 1 << " parameters, but given " << input_length - 1 << endl;
        return false;
	}
    return true;
}

int main() {
	FileManager fm;
	/*cout << fm.getDisk()->inodes[0].getName() << endl;
	cout << fm.getDisk()->inodes[0].getType() << endl;
	cout << fm.getDisk()->inodes[0].getSize() << endl;
	cout << fm.getDisk()->inodes[0].getimeCreated() << endl;*/
	
	// fm.createFile("hello.dat", 1, 1024);
	// fm.createDirectory("folder1", 0);
	// fm.createDirectory("folder2", 2);
	// fm.createFile("a", 1, 1024, 0);//1
	// fm.createDirectory("root", 0);//2
	// fm.createFile("b", 1, 1024, 0);//3
	// fm.createFile("a", 1, 1024, 2);//4
	// fm.createDirectory("root", 2);//5
	// fm.createFile("b", 1, 1024, 2);//6
	// fm.createFile("a", 1, 1024, 5);//7
	// fm.createDirectory("root", 5);//8
	// fm.createFile("b", 1, 1024, 5);//9
	// fm.createFile("a", 1, 1024, 8);//10
	// fm.createDirectory("a", 8);//11
	// fm.createFile("b", 1, 1024, 8);//12
	// int result = fm.getNode("/a/a/a/d", "FILE");
	// cout << "Get Node: " << -result - 1 << endl;
	// fm.changeDirectory2("/root");
	// fm.changeDirectory2("root");
	// fm.copyFile2("/b", "/a/a/c");
	// fm.deleteFile2("/a/a/c");
	// fm.listAll(0);
	// fm.listAll(2);
	// fm.listAll(fm.getNode("/a/a", "DIR"));
	// fm.listAll(11);
	
    vector<string> str;
    string input;
    bool running = true;
    string currentDir = "/";
    //when the str equals with the string "exit",break the cycling
    while (running) {
        //shell prompt
        cout << "[root@HDFS] " << currentDir << " $ ";
        getline(cin, input);  //enter the command
        istringstream input_stream(input);
        string item;
        while (input_stream >> item) {
            str.push_back(item);
        }
        if (str.empty()) {
            continue;
        }
        if (str[0] == "exit") {
            running = false;
        }
        if (str[0] == "pwd") {
            stack<string> currentPath = fm.getCurrentPath();
            string curDir;
            if (currentPath.empty())
            {
                cout << "/";
            }
            while (!currentPath.empty())
            {
                curDir = currentPath.top();
                currentPath.pop();
            	cout << "/" << curDir;
            }
            cout << endl;
        }
        if (str[0] == "dir") {
            fm.listAll2();
        }
        if (str[0] == "changeDir") {
	        if (checkCommandLength(str, 2))
	        {
				fm.changeDirectory2(str[1].data());
                stack<string> currentPath = fm.getCurrentPath();
                if (currentPath.empty()) currentDir = "/";
                else currentDir = currentPath.top();
	        }
        }
        if (str[0] == "createFile") {
            if (checkCommandLength(str, 3))
                fm.createFile2(str[1].data(), atoi(str[2].data()));
        }
        if (str[0] == "deleteFile") {
            if (checkCommandLength(str, 2))
                fm.deleteFile2(str[1].data());
        }
        if (str[0] == "createDir") {
            if (checkCommandLength(str, 2))
                fm.createDirectory2(str[1].data());
        }
        if (str[0] == "deleteDir") {
            if (checkCommandLength(str, 2))
                fm.deleteDirectory2(str[1].data());
        }
        if (str[0] == "cp") {
            if (checkCommandLength(str, 3))
                fm.copyFile2(str[1].data(), str[2].data());
        }
        if (str[0] == "sum") {
            fm.displayUsage();
        }
        if (str[0] == "cat") {
            if (checkCommandLength(str, 2))
                fm.printFileContents2(str[1].data());
        }
        if (str[0] == "help") {
            fm.getDisk()->welcomeMessage();
        }
        str.clear();
    }
    return 0;
}