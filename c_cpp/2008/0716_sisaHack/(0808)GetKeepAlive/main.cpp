#include <windows.h>
#include <iostream>
#include "httpModule.h"
using namespace std;

// declare functions
double GetHDTimer();

int main(int argc, char* argv[])
{
//	double sec = GetHDTimer();

	if(argc == 2) {
		HTTPDown(argv[1]);
		CleanUpConnectResources();
	}
//	cout << "Performed Time" << (GetHDTimer()-sec) << endl;
	return 0;
}

double GetHDTimer()
{
	LARGE_INTEGER now, freq;
	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&freq);
	return now.QuadPart / (double)freq.QuadPart;
}