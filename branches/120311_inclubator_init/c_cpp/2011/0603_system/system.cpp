// system.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if(argc > 1) {
		printf("Execute : %s\n", argv[1]);
		printf("return code : %d\n", ::system(argv[1]));
	} else {
		printf("> system [.exe]\n");
	}
	return 0;
}

