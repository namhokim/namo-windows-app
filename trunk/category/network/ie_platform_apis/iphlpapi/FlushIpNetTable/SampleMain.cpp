#include <stdio.h>
#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

int main()
{
	DWORD dw;
	// ���� - NO_ERROR
	// ���� - ERROR_ACCESS_DENIED (5L) -  ������ ��Ÿ�� ������ ���� 2008 ���Ŀ����� ������ ������ �ʿ�
	//		- ERROR_INVALID_PARAMETER (87L)
	//		- ERROR_NOT_SUPPORTED (50L)
	dw = FlushIpNetTable(12);
	printf("FlushIpNetTable result: %d\n", dw);

	return 0;
}