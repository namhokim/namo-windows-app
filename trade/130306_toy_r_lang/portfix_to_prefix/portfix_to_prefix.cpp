// portfix_to_prefix.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string>
#include "ToyTokenizer.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string input_1("3");
	string input_2("abc");
	string input_3("(1 2 MINUS)");

	ToyTokenizer token;
	token.setProg(input_1.c_str());


	return 0;
}

