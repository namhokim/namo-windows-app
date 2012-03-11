#include <string>
#include <iostream>
#include <tchar.h>
#ifndef _RELEASE
#include "HttpRequest.h"
#else
#include "UserInfoValidator.h"
#endif
using namespace std;
void Usage()
{
	cout << "\ncheckUsr [id] [password]\n\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
#ifndef _RELEASE
	HttpRequest req;
	if(req.open(L"10.15.1.102", 9090)) {
		req.putTest();
	}

#else
	if(argc != 3) Usage();
	else {
		UserInfoValidator vt;
		//vt.setDebugMode(true);
		wcout << vt.resolveMsg(vt.isValid(argv[1], argv[2])) << endl;
	}
#endif
	return 0;
}
