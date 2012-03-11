#include <stdio.h>
#include <windows.h>
#include "httpModule.h"
#include "common.h"

extern char g_id[MAX_ID];

void processingStdin(char *_id)
{
	strcpy_s(g_id,MAX_ID, _id);	// id ����

	char pwdKey[MAX_PASSWORD];
	int returnValue, resultQuery;
	
	do {
		returnValue = scanf_s("%s", pwdKey, MAX_PASSWORD);
		if(EOF == returnValue) break;

		resultQuery = requestPassword(pwdKey);
		printf("%s\t%d\n", pwdKey, resultQuery);
		if(TRUE == resultQuery || EXCEPT_ERROR == resultQuery || CONNECT_ERROR == resultQuery) break;
		printf("");		// ���� ���
		
	}
	while (1);
}
void stdinEntry(char * id)
{
	SYSTEMTIME stCurrStartTime, stCurrEndTime;		// �ý��� �ð�
	GetLocalTime(&stCurrStartTime);		// ���� ���� �ð��� ���Ѵ�.

	processingStdin(id);	// ó��

	GetLocalTime(&stCurrEndTime);	// ���� ���� �ð��� ���Ѵ�.

	// ���� ���
	printf("target ID : %s\n", id);
	printf("Start : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute,stCurrStartTime.wSecond, stCurrStartTime.wMilliseconds);
	printf("End   : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrEndTime.wMonth,stCurrEndTime.wDay,
		stCurrEndTime.wHour, stCurrEndTime.wMinute,stCurrEndTime.wSecond,stCurrEndTime.wMilliseconds);
	WinExec("MessageBox.exe Terminated", SW_SHOWNORMAL);	// ���α׷� ����� ������ �޽���
}