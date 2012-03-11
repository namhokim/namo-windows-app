// IsExistFile.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <clocale>

BOOL IsFileExist(LPCTSTR filename);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale ( LC_ALL, "" );

	if(argc==2) {
		_tprintf(_T("%s is %s\n"), argv[1], (IsFileExist(argv[1]) ? _T("exist") : _T("not exist")));
	} else {
		_tprintf(_T("Parameter insufficient - count : %i\n"), argc);
	}
	return 0;
}

#include <Windows.h>
BOOL IsFileExist(LPCTSTR filename)
{
	DWORD dw = GetFileAttributes(filename);
	if(dw == INVALID_FILE_ATTRIBUTES) {
		//dw = ::GetLastError();
		// ERROR_FILE_NOT_FOUND, ERROR_PATH_NOT_FOUND, ERROR_BAD_NETPATH ...
		return FALSE;
	}
	return TRUE;
}