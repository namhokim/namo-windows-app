// upper.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <stdio.h>
#include <ctype.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int c = getc(stdin);
	while(EOF!=c) {
		putchar(toupper(c));
		c = getc(stdin);
	}

	return 0;
}
