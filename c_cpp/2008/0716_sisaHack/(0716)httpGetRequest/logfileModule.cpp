#include <windows.h>
#include <stdio.h>

char savingFilename[MAX_PATH];

BOOL writeLog(char * writeContent)
{
	HANDLE hFile;					// 파일 핸들
	DWORD writtenLength;			// 저장된 길이

	if(0 == strlen(savingFilename)) strcpy_s(savingFilename,MAX_PATH,"logging.log");

	hFile = CreateFile(savingFilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile) {					// 파일 핸들을 열지 못함(열음)
		printf("can't create file handle\n");
		return FALSE;
	}
	else {
		SetFilePointer(hFile, 0, NULL, FILE_END);	// 파일의 끝으로 파일포인터 이동
		WriteFile(hFile, writeContent, (DWORD)strlen(writeContent), &writtenLength, NULL);
	}
	CloseHandle(hFile);	// 로깅파일 닫기

	return TRUE;
}