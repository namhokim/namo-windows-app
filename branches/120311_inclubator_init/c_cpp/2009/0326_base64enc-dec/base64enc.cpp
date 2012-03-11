// base64enc.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <iostream>
using namespace std;

#define ENCODING
#ifdef ENCODING
void Usage()
{
	cout << "Usage> base64enc(.exe) string\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc !=2) Usage();
	else {
		const int bufferSz = 1024;
		char out[bufferSz];
		int destLen = bufferSz;
		BOOL res = Base64Encode((BYTE *) argv[1], (int)strlen(argv[1]),
			out, &destLen, ATL_BASE64_FLAG_NOPAD);
		out[destLen] = '\0';
		cout << out << endl;
	}
	return 0;
}
#else

void Usage()
{
	cout << "Usage> base64dec(.exe) string\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc !=2) Usage();
	else {
		const int bufferSz = 1024;
		char out[bufferSz];
		int destLen = bufferSz;
		BOOL res = Base64Decode((LPCSTR)argv[1], (int)strlen(argv[1]), (BYTE *)out, &destLen);
		out[destLen] = '\0';
		cout << out << endl;
	}
	return 0;
}
#endif