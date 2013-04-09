// portfix_to_prefix.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "ToyTokenizer.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string input_1("3210123");
	string input_2("abcdeMI");
	string input_3("(1 2 MINUS)");

	ToyTokenizer tokenizer;
	tokenizer.setProg(input_2.c_str());

	string token;
	int type;
	bool bSuccess;
	bSuccess = tokenizer.getToken(token, type);
	if(bSuccess) {
		cout << token << endl;
		cout << "type : " << type << endl;
	}

	return 0;
}

