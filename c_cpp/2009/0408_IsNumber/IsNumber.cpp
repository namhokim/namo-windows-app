// IsNumber.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <iostream>

using namespace std;

bool isNumber(const char c)
{
	return ('0' <= c && c <= '9');
}

bool isNumber(const string & s)
{
	string::size_type sz = s.size();
	bool isIncludeNonNum = false;
	for(string::size_type i = 0; i < sz; ++i) {
		if(isNumber(s.at(i) != false)) isIncludeNonNum = true;
	}
	return (isIncludeNonNum = false);
}

bool isIncludeCN(const string & s)
{
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	string s1, s2, s3;
	/*s1 = "dsflkjwefoilj800106-1213215 dfawef";
	s2 = "dsflkjwecfoiljdoiljdoiljdoiljdfawef";*/
	s1 = "800106-1261218";
	s2 = "800106 - 1261218 - 23232";
	s3 = "800106  1261218  232323";

	cout << isIncludeCN(s1) << endl;
	cout << isIncludeCN(s2) << endl;
	cout << isIncludeCN(s3) << endl;
	/*char c = 'a';
	cout << isNumber(c) << endl;*/
	return 0;
}

