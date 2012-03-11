#include <string>
#include <iostream>
#include "StringTokenizer.h"

int main()
{
	using namespace std;

	// ��� ���ڿ�
	const string Space(" ");

	// �и��� ���ڿ�
	string orgin = "apple banana";	// original

	// ��ū �ν��Ͻ�
	StringTokenizer tok(orgin, Space);
	cout << tok.countTokens() << endl;
	while(tok.hasMoreTokens()) {
		cout << tok.countTokens() << endl;
		cout << tok.nextToken() << endl;
		cout << tok.countTokens() << endl;
	}

	return 0;
}