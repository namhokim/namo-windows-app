// RunTelnet.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <cstdlib>

int _tmain(int argc, _TCHAR* argv[])
{
	::system("start telnet localhost 9100");
	return 0;
}

