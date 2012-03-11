// neatnesh.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

bool isOk(string& str);
bool isOk2(string& str);

int _tmain(int argc, _TCHAR* argv[])
{
	// test string
	string s1 = "213156123156z";
	string s2 = "213156123156";
	string s3 = "213156zzzzzzz";
	string s4 = "213156zzzzzzz12";
	string s5 = "z";

	// performance vars
	LARGE_INTEGER t1, t2, fr, diff;
	::QueryPerformanceFrequency(&fr);
	::QueryPerformanceCounter(&t1);		// lab1
	double ㎲ = 0;

	// test1
	cout << s1 << " : " << isOk(s1) << endl;
	cout << s2 << " : " << isOk(s2) << endl;
	cout << s3 << " : " << isOk(s3) << endl;
	cout << s4 << " : " << isOk(s4) << endl;
	cout << s5 << " : " << isOk(s5) << endl;

	::QueryPerformanceCounter(&t2);		// lab2

	// calculate & print
	diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
	㎲ = (double)diff.QuadPart / ((double)fr.QuadPart);
	fprintf(stdout, "수행시간 : %f ㎳\n", ㎲*1000);

	::QueryPerformanceCounter(&t1);		// lab1

	// test2
	cout << s1 << " : " << isOk2(s1) << endl;
	cout << s2 << " : " << isOk2(s2) << endl;
	cout << s3 << " : " << isOk2(s3) << endl;
	cout << s4 << " : " << isOk2(s4) << endl;
	cout << s5 << " : " << isOk2(s5) << endl;

	::QueryPerformanceCounter(&t2);		// lab2

	// calculate & print
	diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
	㎲ = (double)diff.QuadPart / ((double)fr.QuadPart);
	fprintf(stdout, "수행시간 : %f ㎳\n", ㎲*1000);


	return 0;
}

bool isOk(string& str)
{
	string::size_type size = str.size();

	if(size>0) {
		return (str[size-1] == 'z');
	} else {
		return false;
	}
}

bool isOk2(string& str)
{
	if(!str.empty()) {
		string::iterator it = --str.end();
		return (*it=='z');
	} else {
		return false;
	}
}
