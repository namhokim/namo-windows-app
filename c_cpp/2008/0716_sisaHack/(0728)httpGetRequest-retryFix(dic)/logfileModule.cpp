#include <windows.h>
#include <stdio.h>

char savingFilename[MAX_PATH];

BOOL writeLog(char * writeContent)
{
	HANDLE hFile;					// ���� �ڵ�
	DWORD writtenLength;			// ����� ����

	if(0 == strlen(savingFilename)) strcpy_s(savingFilename,MAX_PATH,"logging.log");

	hFile = CreateFile(savingFilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile) {					// ���� �ڵ��� ���� ����(����)
		printf("can't create file handle\n");
		return FALSE;
	}
	else {
		SetFilePointer(hFile, 0, NULL, FILE_END);	// ������ ������ ���������� �̵�
		WriteFile(hFile, writeContent, (DWORD)strlen(writeContent), &writtenLength, NULL);
	}
	CloseHandle(hFile);	// �α����� �ݱ�

	return TRUE;
}