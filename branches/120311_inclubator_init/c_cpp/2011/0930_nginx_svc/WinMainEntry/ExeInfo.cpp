#include "ExeInfo.h"

// C의 런타임 헤더 파일:
#include <tchar.h>

DWORD GetExeDirectory(LPTSTR lpFilepath)
{
	DWORD dwLength = GetModuleFileName(NULL, lpFilepath, MAX_PATH);
	if(0!=dwLength) {
		LPTCH string = lpFilepath + dwLength;
		while(--string != lpFilepath && *string != _T('\\'))    ;
		if (*string == _T('\\')) {
			*string = _T('\0');            // NULL 치환
			return (string-lpFilepath);    // 길이 반환
		}
	}
	return 0;
}

DWORD GetExeFilename(LPTSTR szFilename)
{
	TCHAR szPath[MAX_PATH] = {0,};
	PTCHAR pch, pDest;
	DWORD dwPos, dwRes;
	
	dwPos = 0;

	// Get Exe file path
	dwRes = ::GetModuleFileName(NULL, szPath, MAX_PATH);
	if(dwRes>=MAX_PATH) return dwRes;

	// Find the last of backslash
	pch = szPath;
	while( *pch ) {
		if(_T('\\') == (*pch)) {
			dwPos = pch - szPath;
		};
		++pch;
	}

	// Copy
	pch = szPath + dwPos + 1;
	pDest = szFilename;
	while( ( (*pDest)=(*pch) ) ) {
		if(_T('\\') == (*pch)) {
			dwPos = pch - szPath;
		}
		++pDest;
		++pch;
	}

	return dwRes;
}