#include <string>
#include <iostream>
#include "StringTokenizer.h"

int main()
{
	using namespace std;

	// 상수 문자열
	const string Space(" ");

	// 분리할 문자열
	string orgin = "apple banana";	// original

	// 토큰 인스턴스
	StringTokenizer tok(orgin, Space);
	cout << tok.countTokens() << endl;
	while(tok.hasMoreTokens()) {
		cout << tok.countTokens() << endl;
		cout << tok.nextToken() << endl;
		cout << tok.countTokens() << endl;
	}

	return 0;
}