// Redirect.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#pragma warning(disable : 4996)

void testOutput()
{
	using namespace std;
	cout << "bluablua\r\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	/* redirect standard output to a file */
	if (freopen("log.txt", "a+", stdout) == NULL)
		fprintf(stderr, "error redirecting stdout\n");

	/* this output will go to a file */
	fprintf(stdout, "This will go into a file.\r\n");
	testOutput();
	fprintf(stdout, "This will go into~.\r\n");

	/* close the standard output stream */
	fclose(stdout);
	return 0;
}

