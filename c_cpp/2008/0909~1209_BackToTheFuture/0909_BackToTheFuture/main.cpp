#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

const int defaultDuration = 10;


INT64 FILETIMEtoINT64(const LPFILETIME filetime)
{
	INT64 _i64 = 0;
	_i64 = filetime->dwHighDateTime;
	_i64 = _i64 << 32;
	_i64 = _i64 | filetime->dwLowDateTime;
	return _i64;
}

FILETIME INT64toFILETIME(const PINT64 int64)
{
	FILETIME filetime;
	filetime.dwHighDateTime = (DWORD)(*int64 >> 32);
	filetime.dwLowDateTime  = (DWORD)(*int64 & 0xffffffff);
	return filetime;
}

void SetToTheOffsetSecond(int seconds)
{
	FILETIME currentFileTime, newFileTime;
	SYSTEMTIME newSystemTime;
	INT64 currentTime, addTime, newTime;

	addTime = (INT64)seconds * 10000000;
	GetSystemTimeAsFileTime(&currentFileTime);
	currentTime = FILETIMEtoINT64(&currentFileTime);

	newTime = currentTime + addTime;
	newFileTime = INT64toFILETIME(&newTime);

	FileTimeToSystemTime(&newFileTime, &newSystemTime);
	SetSystemTime(&newSystemTime);
}

void excuteEntry(int offsetToFuture, int durationTime)
{
	SetToTheOffsetSecond(+offsetToFuture);
	Sleep(durationTime*1000);
	SetToTheOffsetSecond(-offsetToFuture);
}

void Usage()
{
	cout << "Usage)" << endl
		<< "   BackToTheFuture [offset to future(sec)] \t-> duration is default 60sec" << endl
		<< "   BackToTheFuture [offset to future(sec)] [duration time(sec)]" << endl;
}


int main(int argc, char* argv[])
{
	switch (argc) {
		case 2:
			excuteEntry(atoi(argv[1]),defaultDuration);
			break;
		case 3:
			excuteEntry(atoi(argv[1]),atoi(argv[2]));
			break;
		case 1:
		default:
			Usage();
	}
	return 0;
}
