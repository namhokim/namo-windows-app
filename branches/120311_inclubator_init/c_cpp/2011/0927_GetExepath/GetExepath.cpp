// GetExepath.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

DWORD GetModuleFilePathW( LPWCH lpFilepath, DWORD nSize ) {

	DWORD dwLength = GetModuleFileNameW(NULL, lpFilepath, nSize);
	if(0!=dwLength) {
		LPWCH string = lpFilepath + dwLength;
		while(--string != lpFilepath && *string != L'\\')	;
		if (*string == L'\\') {
			*string = L'\0';			// NULL ġȯ
			return (string-lpFilepath);	// ���� ��ȯ
		}
	}

	return 0;
}







int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR path[MAX_PATH] = {0};
	DWORD dw = GetModuleFilePathW(path, MAX_PATH);
	return 0;
}

