// portfix_to_prefix.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>		// for string
#include <iostream>		// for cout/endl
#include <algorithm>	// for copy
#include <iterator>		// for ostream_iterator
#include "ToyTokenizer.h"

using namespace std;

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

	string in(input_4);		// 여기를 변경(위의 문자열 중에 하나로)

	// 1. 전위표기법 변경
	string out;
	if(postfix_to_prefix(in.c_str(), out)) {
		cout << "in(postfix) : " << in << endl;
		cout << "out(prefix) : " << out << endl << endl;
	}

	// 2. 중간코드 생성
	std::vector<std::string> out_imc;
	if(make_im_code(in.c_str(), out_imc)) {
		copy(out_imc.begin(), out_imc.end(), ostream_iterator<string>(cout, "\n"));

		cout << endl;
		int ev;
		// 3. 평가
		if(evaluation(out_imc, ev)) {
			cout << "eval : " << ev << endl;
		}
		return 0;
	} else {
		cout << "failed..." << endl;
	}

	return 0;
}
