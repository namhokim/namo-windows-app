// portfix_to_prefix.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "ToyTokenizer.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string input_1("3210123");
	string input_2("abcdeMI added");
	string input_3("(1 2 MINUS)");

	ToyTokenizer tokenizer;
	tokenizer.setProg(input_2.c_str());

	string token;
	int type;
	while( tokenizer.getToken(token, type) ) {
		cout << token << endl;
		cout << "type : " << type << endl;
	}

	return 0;
}

