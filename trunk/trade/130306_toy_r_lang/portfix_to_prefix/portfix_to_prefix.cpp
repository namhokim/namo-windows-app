// portfix_to_prefix.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <algorithm>	// for copy
#include <iterator>		// for ostream_iterator
#include "ToyTokenizer.h"

using namespace std;

void test(const string& prog);

int _tmain(int argc, _TCHAR* argv[])
{
	string input_1("3210123");
	string input_2e("abcdeMI added plus");
	string input_31("1 2 MINUS");
	string input_32("1 2 IF");
	string input_33e("1 2 3");
	string input_34("(1 2 MINUS)");
	string input_4("((2 1 IF)3 MINUS)");
	string input_5("((1 2 MINUS)(3 4 MINUS) IF)");
	string input_6("((((1 2 MINUS)(3 4 MINUS) MINUS)5 MINUS) 6 MINUS)");
	string input_7("-3210123");
	string input_8("--3210123");
	string input_9("-3210123-");
	string input_10("((2 2 MINUS)3MINUS)");
	string input_11("abd");
	string input_12("(abd)");

	string in(input_4);

	// for prefix
	//string out;
	//if(postfix_to_prefix(in.c_str(), out)) {
	//	cout << "in  : " << in << endl;
	//	cout << "out : " << out << endl;
	//}

	// for intermediate code
	cout << "in  : " << in << endl << endl;
	std::vector<std::string> out;
	if(make_im_code(in.c_str(), out)) {
		copy(out.begin(), out.end(), ostream_iterator<string>(cout, "\n"));

		cout << endl;
		int ev;
		if(evaluation(out, ev)) {
			cout << "eval : " << ev << endl;
		}
		return 0;
	} else {
		cout << "failed..." << endl;
	}

	// for parse
//	int errorPosition;
//	cout << in << endl;
//	bool res = parse(in.c_str(), &errorPosition);
//	if(!res) {
//		for (int i=1; i<errorPosition; ++i) cout << " ";
//		cout << "*" << endl;
////		cout << "position : " << errorPosition << endl;
//	}
//	cout << (res ? "success" : "false") << endl;

	// for test
	//test(in);

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
