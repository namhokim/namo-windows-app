// lower.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <ctype.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int c = getc(stdin);
	while(EOF!=c) {
		putchar(tolower(c));
		c = getc(stdin);
	}

	return 0;
}

