#include <regex>
#include <iostream>

using namespace std;

bool isValidEmail(string email);
bool isExitString(string email);

int main()
{
	const string InputMsg("email > ");
	
	string email;	// ("namo-_-hi@sindoh11.net");
	cout << InputMsg;
	while(getline(cin, email)) {
		if ( isExitString(email) ) break;
		cout << " => " << (isValidEmail(email) ? "OK" : "NO") << endl;
		cout << InputMsg;
	}
	
	return 0;
}

// "[0-9A-Za-Z]*[-_]*[0-9A-Za-z]*@[0-9A-Za-Z]*[-_]*[0-9A-Za-z]*.[a-zA-Z]*"
bool isValidEmail(string email)
{
//	const std::tr1::regex pattern("[0-9A-Za-z]*[-_]*[0-9A-Za-z]*@[0-9A-Za-z]*.[a-zA-Z]*");
//	const std::tr1::regex pattern("[0-9A-Za-z!#$%&'*+-/=?^_`{|}~]*@[0-9A-Za-z]*.[a-zA-Z]*");
	const std::tr1::regex pattern("[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?");
	return std::tr1::regex_match(email, pattern);
}

bool isExitString(string email)
{
	bool retFlag = false;
	string::size_type NoMatch =  string::npos;
	if(email.find("rhdiddl") != NoMatch) retFlag = true;
	else if (email.find("rhdiddltkd") != NoMatch) retFlag = true;
	else if (email.find("°í¾çÀÌ") != NoMatch) retFlag = true;
	else if (email.find("°í¾çÀÌ»ó") != NoMatch) retFlag = true;
	else if (email.find("±èÀ±Àü") != NoMatch) retFlag = true;
	else if (email.find("±è¿µ¹Ì") != NoMatch) retFlag = true;
	
	return retFlag;
}