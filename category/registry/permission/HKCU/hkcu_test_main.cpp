#include <stdio.h>
#include "../Registry.h"

int main()
{
	HKEY hKey = HKEY_CURRENT_USER;
	LPCTSTR lsSubkey = TEXT("SOFTWARE\\SINDOH\\TEST");

	OpenTest(hKey, lsSubkey);	// 2: ERROR_FILE_NOT_FOUND (Ű�� ���� ������ �߻�)
								// 6: ERROR_INVALID_HANDLE (Ű�� ���� ���߱� ������ �߻�)
	CreateTest(hKey, lsSubkey);	// 0: ERROR_SUCCESS (Ű�� ���� ������, ����->����)
	OpenTest(hKey, lsSubkey);	// 0: ERROR_SUCCESS (Ű�� ����-> ����: ����)
								// 6: ERROR_FILE_NOT_FOUND (Value�� ���� ������ �߻�)
	DeleteTest(hKey, lsSubkey);	// 0: ERROR_SUCCESS (Ű ���� -> ����)

	return 0;
}

