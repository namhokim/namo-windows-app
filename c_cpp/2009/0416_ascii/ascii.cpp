// ascii.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	char c;
	std::cout << "input one character >> ";
	std::cin >> c;
	std::cout << "the value of '" << c << "' is " << int(c) << '\n';
	return 0;
}

