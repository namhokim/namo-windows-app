// ascii.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

