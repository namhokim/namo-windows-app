#include <string>
#include <iostream>
#include <tchar.h>
#include "FTPRequest.h"

using namespace std;
void Usage()
{
	cout << "\ncheckUsr [id] [password]\n\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	wcout.imbue(locale("korean"));
	FTPRequest ftp;
	if(ftp.open(L"10.15.1.102", L"administrator", L"sindo")) {
		//ftp.put(L"StringUtil.h");
		ftp.get(L"abc.h");
		//_COUT << ftp.dir() << endl;
	}
	return 0;
}
