#include <string>
#include <iostream>

using namespace std;

string ToUpper(const string &str);

int main ()
{
	string line;

	while(cin >> line) {
		cout << ToUpper(line) << endl;
	}
	return 0;
}

string ToUpper(const string &str)
{
	string::size_type pos, size = str.size();
	string ret;
	ret.reserve(size);

	for(pos=0; pos<size; ++pos) {
		char c = str[pos];
		if( (::isalpha(c)!=0) && (::isupper(c)!=0)) {
			ret.push_back('_');
			ret.push_back(c);
		} else {
			ret.push_back(::toupper(c));
		}
	}
	return ret;
}