// Ren.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "windows.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc > 1) {
		BOOL r = ::MoveFile(argv[1], argv[2]);
		if(r == FALSE) printf("Error code : %d\n", ::GetLastError());
		else printf("Normal code : %d\n", r);
	}
	return 0;
}

