// Ren.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

