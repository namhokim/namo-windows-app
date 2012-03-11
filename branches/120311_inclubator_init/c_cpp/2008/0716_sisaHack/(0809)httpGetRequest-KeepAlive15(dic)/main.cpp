#include "keygenModule.h"
#include "httpModule.h"
#include "timer.h"
#include <windows.h>
#include <iostream>
#include <iomanip>
using namespace std;

// declare functions
void Usage();


// define functions
void Usage()
{
	cout << "Usage:\n";
	cout << " httpGetRequest [start String]\n";
	cout << " httpGetRequest [id] [start String]\n";
	cout << " httpGetRequest [id] [start String] [end String]\n";
}

int main(int argc, char* argv[])
{
	double sec = GetHDTimer();

	if(argc == 2) controlEntry2("nam17845",argv[1]);
	else if(argc == 3) controlEntry2(argv[1], argv[2]);
	else if(argc == 4) controlEntry3(argv[1], argv[2], argv[3]);
	else {
		//printf("%d\n", findIndexDictionary('9'));
		Usage();
		return 1;
	}
	CleanUpConnectResources();
	cout << setprecision(2) << (GetHDTimer()-sec) << "sec" << endl;
	return 0;
}

