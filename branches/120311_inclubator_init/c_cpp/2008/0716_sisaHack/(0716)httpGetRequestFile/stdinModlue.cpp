#include <stdio.h>
#include <windows.h>
#include "httpModule.h"
#include "common.h"

extern char g_id[MAX_ID];

void processingStdin(char *_id)
{
	strcpy_s(g_id,MAX_ID, _id);	// id 설정

	char pwdKey[MAX_PASSWORD];
	int returnValue, resultQuery;
	
	do {
		returnValue = scanf_s("%s", pwdKey, MAX_PASSWORD);
		if(EOF == returnValue) break;

		resultQuery = requestPassword(pwdKey);
		printf("%s\t%d\n", pwdKey, resultQuery);
		if(TRUE == resultQuery || EXCEPT_ERROR == resultQuery || CONNECT_ERROR == resultQuery) break;
		printf("");		// 버퍼 비움
		
	}
	while (1);
}
void stdinEntry(char * id)
{
	SYSTEMTIME stCurrStartTime, stCurrEndTime;		// 시스템 시간
	GetLocalTime(&stCurrStartTime);		// 현재 로컬 시간을 구한다.

	processingStdin(id);	// 처리

	GetLocalTime(&stCurrEndTime);	// 현재 로컬 시간을 구한다.

	// 정보 출력
	printf("target ID : %s\n", id);
	printf("Start : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute,stCurrStartTime.wSecond, stCurrStartTime.wMilliseconds);
	printf("End   : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrEndTime.wMonth,stCurrEndTime.wDay,
		stCurrEndTime.wHour, stCurrEndTime.wMinute,stCurrEndTime.wSecond,stCurrEndTime.wMilliseconds);
	WinExec("MessageBox.exe Terminated", SW_SHOWNORMAL);	// 프로그램 종료시 윈도우 메시지
}