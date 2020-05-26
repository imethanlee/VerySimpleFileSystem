#include "FileManager.h"
#include <iostream>
#include <sstream>

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

    vector<string> str;
    string input;
    bool running = true;
    string currentDir = "/";
    cout << endl;
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
        else if (str[0] == "pwd") {
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
        else if (str[0] == "dir") {
            fm.listAll2();
        }
        else if (str[0] == "changeDir") {
	        if (checkCommandLength(str, 2))
	        {
				fm.changeDirectory2(str[1].data());
                stack<string> currentPath = fm.getCurrentPath();
                string curDir;
                if (currentPath.empty())
                {
                    curDir = "/";
                }
                else
                {
                    while (!currentPath.empty())
                    {
                        curDir = currentPath.top();
                        currentPath.pop();
                    }
                }
                currentDir = curDir;
	        }
        }
        else if (str[0] == "createFile") {
            if (checkCommandLength(str, 3))
            {
                if (atoi(str[2].data()) <= 351)
                {
                    fm.createFile2(str[1].data(), atoi(str[2].data()));
                }
                else
                {
                    cerr << "File size too large" << endl;
                } 
            }
        }
        else if (str[0] == "deleteFile") {
            if (checkCommandLength(str, 2))
                fm.deleteFile2(str[1].data());
        }
        else if (str[0] == "createDir") {
            if (checkCommandLength(str, 2))
                fm.createDirectory2(str[1].data());
        }
        else if (str[0] == "deleteDir") {
            if (checkCommandLength(str, 2))
                fm.deleteDirectory2(str[1].data());
        }
        else if (str[0] == "cp") {
            if (checkCommandLength(str, 3))
                fm.copyFile2(str[1].data(), str[2].data());
        }
        else if (str[0] == "sum") {
            fm.displayUsage();
        }
        else if (str[0] == "cat") {
            if (checkCommandLength(str, 2))
                fm.printFileContents2(str[1].data());
        }
        else if (str[0] == "help") {
            fm.getDisk()->welcomeMessage();
        }
        else {
            cout << "-bash: " << str[0] << ": Command not found" << endl;
        }
        str.clear();
        cout << endl;
    }
    return 0;
}