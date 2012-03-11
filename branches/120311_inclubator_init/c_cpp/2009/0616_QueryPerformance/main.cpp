//#include "Performance.h"
#include <Windows.h>
#include <cstdio>

int main()
{
	LARGE_INTEGER t1, t2, fr, diff;
	if( ::QueryPerformanceFrequency(&fr) != 0) {	// 하드웨어가 고해상도 카운터를 지원할 때
		bool isSuccess = true;
		if( ::QueryPerformanceCounter(&t1) == 0 ) isSuccess = false;
		
		// 어떤 작업...
		Sleep(1000);
		
		if( ::QueryPerformanceCounter(&t2) == 0 ) isSuccess = false;
		if ( isSuccess ) {
			diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
			double ㎲ = (double)diff.QuadPart / ((double)fr.QuadPart);
			fprintf(stdout, "수행시간 : %f ㎳\n", ㎲*1000);
		}
		else {
			fprintf(stderr, "수행시간 측정에 에러(%d)\n", ::GetLastError());
		}
	}
	else {
		fprintf(stderr, "하드웨어가 고해상도 카운터를 지원하지 않습니다.\n");
	}
	return 0;
}
