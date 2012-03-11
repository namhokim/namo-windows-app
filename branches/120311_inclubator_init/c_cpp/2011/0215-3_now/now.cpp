// sleep.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	printf("%4d-%02d-%02d %02d:%02d:%02d.%03d\n", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	return 0;
}

