#include <stdio.h>
#include "../Registry.h"

int main()
{
	HKEY hKey = HKEY_CURRENT_USER;
	LPCTSTR lsSubkey = TEXT("SOFTWARE\\SINDOH\\TEST");

	OpenTest(hKey, lsSubkey);	// 2: ERROR_FILE_NOT_FOUND (키가 없기 때문에 발생)
								// 6: ERROR_INVALID_HANDLE (키를 열지 못했기 때문에 발생)
	CreateTest(hKey, lsSubkey);	// 0: ERROR_SUCCESS (키가 없기 때문에, 생성->성공)
	OpenTest(hKey, lsSubkey);	// 0: ERROR_SUCCESS (키가 존재-> 열기: 성공)
								// 6: ERROR_FILE_NOT_FOUND (Value가 없기 때문에 발생)
	DeleteTest(hKey, lsSubkey);	// 0: ERROR_SUCCESS (키 삭제 -> 성공)

	return 0;
}

