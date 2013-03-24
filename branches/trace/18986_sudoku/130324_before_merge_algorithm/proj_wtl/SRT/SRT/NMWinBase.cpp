#include "StdAfx.h"
#include "NMWinBase.h"

DWORD GetModuleFilePathA(
    __out_ecount_part(nSize, return + 1) LPCH lpFilepath,
    __in     DWORD nSize
	)
{
	DWORD dwLength;
	dwLength = GetModuleFileNameA(NULL, lpFilepath, nSize);
	if(0!=dwLength) {	// If the function fails, the return value is 0 (zero)
		while((--dwLength)>=0) {
			if(lpFilepath[dwLength] == '\\') {	// 찾는다
				lpFilepath[dwLength] = '\0';	// 바꾼다
				return dwLength;
			}
		}
	}

	return 0;
}

DWORD GetModuleFilePathW(
    __out_ecount_part(nSize, return + 1) LPWCH lpFilepath,
    __in     DWORD nSize
	)
{
	DWORD dwLength;
	dwLength = GetModuleFileNameW(NULL, lpFilepath, nSize);
	if(0!=dwLength) {	// If the function fails, the return value is 0 (zero)
		while((--dwLength)>=0) {
			if(lpFilepath[dwLength] == L'\\') {	// 찾는다
				lpFilepath[dwLength] = L'\0';	// 바꾼다
				return dwLength;
			}
		}
	}

	return 0;
}