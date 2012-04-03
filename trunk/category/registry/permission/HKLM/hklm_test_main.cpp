#include "../Registry.h"

// 링커>매니페스트 도구>UAC 실행 수준
//	:asInvoker, highestAvailable, requireAdministrator

int main()
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	LPCTSTR lsSubkey = TEXT("SOFTWARE\\SINDOH\\WISDOM_LT");

	OpenTest(hKey, lsSubkey);	// 2: ERROR_FILE_NOT_FOUND (키가 없기 때문에 발생)
								// 6: ERROR_INVALID_HANDLE (키를 열지 못했기 때문에 발생)
	CreateTest(hKey, lsSubkey);	// 5: ERROR_SUCCESS (키가 없기 때문에, 생성->권한부족)
	DeleteTest(hKey, lsSubkey);	// 2: ERROR_FILE_NOT_FOUND (키가 없기 때문에 발생)
	system("pause");			// 관리자 권한창이 떴다가 사라짐
	return 0;
}
