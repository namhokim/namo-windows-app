// ArgToFile.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <sys/stat.h>

using namespace std;

void ArgsToOut(ofstream& f, int argc, char* argv[], bool isAppend);
bool IsAppend(const char* filename);

const char* OutFilename = "args.txt";

int main(int argc, char* argv[])
{
	// Check is new or append
	bool isAppend = IsAppend(OutFilename);

	// Open for write
	ofstream f(OutFilename, ios::out|ios::app);
	if(!f.fail()) {
		ArgsToOut(f, argc, argv, isAppend);
	} else {
		cerr << "Cannot create file 'args.txt'" << endl;
	}
	return 0;
}

void ArgsToOut(ofstream& f, int argc, char* argv[], bool isAppend)
{
	if(isAppend) {
		f << endl;
	}

	f << "arguments : " << argc << endl;
	for(int i=0; i<argc; ++i) {
		f << "argv[" << i << "] : " << argv[i] << endl;
	}
}

bool IsAppend(const char* filename)
{
	struct _stat buf;
	int result;

	result = _stat( filename, &buf );

	if(0==result && buf.st_size!=0) return true;
	else return false;
}