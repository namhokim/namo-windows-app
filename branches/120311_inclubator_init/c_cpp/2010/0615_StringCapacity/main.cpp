#include <string>
#include <iostream>

using namespace std;

int main() {
	string str;
	for(string::size_type pos=0; pos < 100000/*string::npos*/; ++pos)
	{
		//str.append("c");
		str.reserve(pos);
		cout << pos << "\t" << str.capacity() << endl;
	}

	return 0;
}