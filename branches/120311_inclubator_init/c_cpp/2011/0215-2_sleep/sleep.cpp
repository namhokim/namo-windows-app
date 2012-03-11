// sleep.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc<2) {
		printf("Need to time argument\n");
	} else {
		DWORD dw;
		dw = _ttol(argv[1]);
		if(0==dw) {
			printf("Need to number argument\n");
		} else {
			::Sleep(dw);
		}
	}
	return 0;
}

