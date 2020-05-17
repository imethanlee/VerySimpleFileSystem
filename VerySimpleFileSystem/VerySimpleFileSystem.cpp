#include "FileManager.h"

int main() {
	srand(time(0));
	DiskSystem ds;
	FileManager fm;

	string curr_time = getCurrTime();
	const char* time = curr_time.data();
	cout << time << endl;

	system("pause");
}