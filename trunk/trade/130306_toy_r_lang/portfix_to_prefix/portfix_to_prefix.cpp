// portfix_to_prefix.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "ToyTokenizer.h"

using namespace std;

void test(const string& prog);
bool parse(const string& prog);

int _tmain(int argc, _TCHAR* argv[])
{
	string input_1("3210123");
	string input_2e("abcdeMI added plus");
	string input_31("1 2 MINUS");
	string input_32("1 2 IF");
	string input_33e("1 2 3");
	string input_34("(1 2 MINUS)");
	string input_4("((2 1 MINUS)3 MINUS)");
	string input_5("((1 2 MINUS)(3 4 MINUS) IF)");
	string input_6("((((1 2 MINUS)(3 4 MINUS) MINUS)5 MINUS) 6 MINUS)");
	string input_7("-3210123");
	string input_8("--3210123");
	string input_9("-3210123-");
	string input_10("((2 1MINUS)3MINUS))");

	string in(input_10);
	//test(input_10);
	cout << "original : " << in << endl << endl;
	cout << (parse(in) ? "sucess" : "false") << endl;

	return 0;
}

void test(const string& prog)
{
	ToyTokenizer tokenizer;
	tokenizer.setProg(prog.c_str());

	cout << prog << endl << endl;

	string token;
	int type;
	while( tokenizer.getToken(token, type) ) {
		cout << token << endl;
		cout << "type : " << type
			<< " (" << TokenTypeToString(type) << ")" << endl << endl;
	}
}
