// TestString.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string s;
	cout << s.size() << endl;
	s.assign("abcdef");
	cout << s.size() << endl;
	stringstream ss;
	ss << 'b' << '\0' << '\t' << 'c' << endl;
	s.assign(ss.str());
	cout << s.size() << endl;
	cout << s << endl;
	cout << s.c_str() << endl;
	return 0;
}

