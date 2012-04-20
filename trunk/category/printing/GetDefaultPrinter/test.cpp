#include <stdio.h>
#include <Windows.h>
#include "test.h"

void RunTestAll()
{
	Test_GetDefaultPrinter_Param_AllNull();
	Test_GetDefaultPrinter_Param_Buffer1024_Null();
	Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT();
	Test_GetDefaultPrinter_Param_Null_LPDWORD_INIT_WITH_0();
	Test_GetDefaultPrinter_Param_Null_LPDWORD_INIT_WITH_1024();
	Test_GetDefaultPrinter_Param_Buffer1024_INIT_WITH_1024();
	Test_GetDefaultPrinter_Param_Buffer2_INIT_WITH_1024();
}

void Test_GetDefaultPrinter_Param_AllNull()
{
	printf("Test Test_GetDefaultPrinter_Param_AllNull>\n");

	BOOL bReturn;
	DWORD dwGLE;
	bReturn = GetDefaultPrinter(NULL, NULL);
	dwGLE = GetLastError();
	printf(" Result: %d\n", bReturn);
	printf(" GetLastError: %u\n", dwGLE);

	printf("\n");
}

void Test_GetDefaultPrinter_Param_Buffer1024_Null()
{
	printf("Test Test_GetDefaultPrinter_Param_Buffer1024_Null>\n");

	BOOL bReturn;
	TCHAR szBuffer[1024] = {0};
	DWORD dwGLE;
	bReturn = GetDefaultPrinter(NULL, NULL);
	dwGLE = GetLastError();
	printf(" Result: %d\n", bReturn);
	printf(" GetLastError: %u\n", dwGLE);

	printf("\n");
}

void Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT()
{
	printf("Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT>\n");

	BOOL bReturn;
	DWORD dwGLE, cchBuffer;
	// printf(" Uninitialized value (cchBuffer): %u\n", cchBuffer);	// warning C4700: 초기화되지 않은 'cchBuffer' 지역 변수를 사용했습니다.
	bReturn = GetDefaultPrinter(NULL, &cchBuffer);
	dwGLE = GetLastError();
	printf(" Result: %d\n", bReturn);
	printf(" GetLastError: %u\n", dwGLE);
	if(!bReturn) {
		printf(" Need size: %u\n", cchBuffer);
	}

	printf("\n");
}

void Test_GetDefaultPrinter_Param_Null_LPDWORD_INIT_WITH_0()
{
	printf("Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT_WITH_0>\n");

	BOOL bReturn;
	DWORD dwGLE, cchBuffer;
	cchBuffer = 0;
	bReturn = GetDefaultPrinter(NULL, &cchBuffer);
	dwGLE = GetLastError();
	printf(" Result: %d\n", bReturn);
	printf(" GetLastError: %u\n", dwGLE);
	if(!bReturn) {
		printf(" Need size: %u\n", cchBuffer);
	}

	printf("\n");
}

void Test_GetDefaultPrinter_Param_Null_LPDWORD_INIT_WITH_1024()
{
	printf("Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT_WITH_1024>\n");

	BOOL bReturn;
	DWORD dwGLE, cchBuffer;
	cchBuffer = 1024;
	bReturn = GetDefaultPrinter(NULL, &cchBuffer);
	dwGLE = GetLastError();
	printf(" Result: %d\n", bReturn);
	printf(" GetLastError: %u\n", dwGLE);
	if(!bReturn) {
		printf(" Need size: %u\n", cchBuffer);
	}

	printf("\n");
}

void Test_GetDefaultPrinter_Param_Buffer1024_INIT_WITH_1024()
{
	printf("Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT_WITH_1024>\n");

	BOOL bReturn;
	DWORD dwGLE, cchBuffer;
	TCHAR szBuffer[1024] = {0};
	cchBuffer = 1024;
	bReturn = GetDefaultPrinter(szBuffer, &cchBuffer);
	dwGLE = GetLastError();
	printf(" Result: %d\n", bReturn);
	printf(" GetLastError: %u\n", dwGLE);
	if(!bReturn) {
		printf(" Need size: %u\n", cchBuffer);
	} else {
		wprintf(L" Default Printer Name: %s", szBuffer);
	}

	printf("\n");
}

void Test_GetDefaultPrinter_Param_Buffer2_INIT_WITH_1024()
{
	printf("Test_GetDefaultPrinter_Param_Null_LPDWORD_NO_INIT_WITH_1024>\n");

	BOOL bReturn;
	DWORD dwGLE, cchBuffer;
	TCHAR szBuffer[2] = {0};
	cchBuffer = 1024;
	bReturn = GetDefaultPrinter(szBuffer, &cchBuffer);	// Run-Time Check Failure #2 - Stack around the variable 'cchBuffer' was corrupted.
	dwGLE = GetLastError();
	printf(" Result: %d\n", bReturn);
	printf(" GetLastError: %u\n", dwGLE);
	if(!bReturn) {
		printf(" Need size: %u\n", cchBuffer);
	} else {
		wprintf(L" Default Printer Name: %s", szBuffer);
	}

	printf("\n");
}

/*
 ERROR_FILE_NOT_FOUND - 2L
 ERROR_INSUFFICIENT_BUFFER - 122L
 ERROR_INVALID_PARAMETER - 87L
 */