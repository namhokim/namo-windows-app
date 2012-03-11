#include <windows.h>
#include <string>
#include <algorithm>
#include <vector>

#include <iostream>
#include <tchar.h>

using namespace std;

void Tokenize(const string& str,
			  vector<string>& tokens,
			  const string& delimiters = " ")
{
	// �� ù ���ڰ� �������� ��� ����
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// �����ڰ� �ƴ� ù ���ڸ� ã�´�
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// token�� ã������ vector�� �߰��Ѵ�
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// �����ڸ� �پ�Ѵ´�.  "not_of"�� �����϶�
		lastPos = str.find_first_not_of(delimiters, pos);
		// ���� �����ڰ� �ƴ� ���ڸ� ã�´�
		pos = str.find_first_of(delimiters, lastPos);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*const string authorName("\"�ַ���׽�Ʈ\"<solution@sindo.com>;");
	cout << authorName;*/
	vector<string> tokens;

    string str("\"�賲ȣ\"<namhokim@sindoh.com>;\"�賲ȣ(nate)\"<namo2da@nate.com>;");

	string sep = ";";
    Tokenize(str, tokens,sep);

	size_t vectorSize = tokens.size();
	string displayName;
	string email;

	for (size_t i=0; i<vectorSize; i++) {
		cout << tokens[i] << endl;
		/*if(i%2 == 0) displayName = tokens[i];
		else {
			email = tokens[i];
			cout << "\"" << displayName  << "\"<" << email << ">\n";
		}*/
	}
    //copy(tokens.begin(), tokens.end(), ostream_iterator<string>(cout, "\n"));

	return 0;
}
