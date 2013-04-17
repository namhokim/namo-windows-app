// portfix_to_prefix.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
	string input_13("(1A 2 MINUS)");

	string in(input_13);		// ���⸦ ����(���� ���ڿ� �߿� �ϳ���)

	// 1. ����ǥ��� ����
	string out;
	int res = postfix_to_prefix(in.c_str(), out);
	cout << "in(postfix) : " << in << endl;
	if(res==NON_ERROR) {
		cout << "out(prefix) : " << out << endl << endl;
	} else {
		cout << "failed : " << ErrorCodeToStringA(res) << endl;
	}

	// 2. �߰��ڵ� ����
	std::vector<std::string> out_imc;
	if(make_im_code(in.c_str(), out_imc)==NON_ERROR) {
		copy(out_imc.begin(), out_imc.end(), ostream_iterator<string>(cout, "\n"));

		cout << endl;
		int ev;
		// 3. ��
		res = evaluation(out_imc, ev);
		if(res==NON_ERROR) {
			cout << "eval : " << ev << endl;
		} else {
			cout << "Undefined" << endl;
		}
		return 0;
	} else {
		cout << "failed : " << ErrorCodeToStringA(res) << endl;
	}

	return 0;
}