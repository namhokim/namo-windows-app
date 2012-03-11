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
	// 맨 첫 글자가 구분자인 경우 무시
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// 구분자가 아닌 첫 글자를 찾는다
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// token을 찾았으니 vector에 추가한다
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// 구분자를 뛰어넘는다.  "not_of"에 주의하라
		lastPos = str.find_first_not_of(delimiters, pos);
		// 다음 구분자가 아닌 글자를 찾는다
		pos = str.find_first_of(delimiters, lastPos);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*const string authorName("\"솔루션테스트\"<solution@sindo.com>;");
	cout << authorName;*/
	vector<string> tokens;

    string str("\"김남호\"<namhokim@sindoh.com>;\"김남호(nate)\"<namo2da@nate.com>;");

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
