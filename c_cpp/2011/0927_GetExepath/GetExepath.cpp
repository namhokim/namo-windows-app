// GetExepath.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

DWORD GetModuleFilePathW( LPWCH lpFilepath, DWORD nSize ) {

	DWORD dwLength = GetModuleFileNameW(NULL, lpFilepath, nSize);
	if(0!=dwLength) {
		LPWCH string = lpFilepath + dwLength;
		while(--string != lpFilepath && *string != L'\\')	;
		if (*string == L'\\') {
			*string = L'\0';			// NULL 치환
			return (string-lpFilepath);	// 길이 반환
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

