#include "keygenModule.h"
#include "keygenSequenceNumber.h"
#include <iostream>
using namespace std;

// declare functions
void Usage();
double GetHDTimer();

// define functions
void Usage()
{
	cout << "Usage:\n";
	cout << " httpGetRequest [prefix string]\n";
	cout << " httpGetRequest [id] [prefix string]\n";
	cout << " httpGetRequest [id] [prefix string] [begin postfix number]\n";
	cout << " httpGetRequest [id] [prefix string] [begin postfix number] pend postfix number\n";
}

int main(int argc, char* argv[])
{
	
	double sec = GetHDTimer();

	if(argc == 2) SeqNumberEntry(argv[1]);
	else if(argc == 3) SeqNumberEntry(argv[1], argv[2]);
	else if(argc == 4) SeqNumberEntry(argv[1], argv[2], atoi(argv[3]));
	else if(argc == 5) SeqNumberEntry(argv[1], argv[2], atoi(argv[3]),atoi(argv[4]));
	else {
		//printf("%d\n", findIndexDictionary('9'));
		Usage();
		return 1;
	}

	cout << (GetHDTimer()-sec);
	return 0;
}

double GetHDTimer()
{
	LARGE_INTEGER now, freq;
	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&freq);
	return now.QuadPart / (double)freq.QuadPart;
}