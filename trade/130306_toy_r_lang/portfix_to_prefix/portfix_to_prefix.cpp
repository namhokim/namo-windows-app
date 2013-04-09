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
	string input_2("abcdeMI added plus");
	string input_3("(1 2 MINUS)");

	ToyTokenizer tokenizer;
	tokenizer.setProg(input_3.c_str());

	string token;
	int type;
	while( tokenizer.getToken(token, type) ) {
		cout << token << endl;
		cout << "type : " << type
			<< " (" << TokenTypeToString(type) << ")" << endl << endl;
	}

	return 0;
}

