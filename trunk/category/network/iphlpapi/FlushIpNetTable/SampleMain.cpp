#include <stdio.h>
#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

int main()
{
	DWORD dw;
	// 성공 - NO_ERROR
	// 실패 - ERROR_ACCESS_DENIED (5L) -  윈도우 비스타와 윈도우 서버 2008 이후에서는 관리자 권한이 필요
	//		- ERROR_INVALID_PARAMETER (87L)
	//		- ERROR_NOT_SUPPORTED (50L)
	dw = FlushIpNetTable(12);
	printf("FlushIpNetTable result: %d\n", dw);

	return 0;
}