#include "../Registry.h"

// ��Ŀ>�Ŵ��佺Ʈ ����>UAC ���� ����
//	:asInvoker, highestAvailable, requireAdministrator

int main()
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	LPCTSTR lsSubkey = TEXT("SOFTWARE\\SINDOH\\WISDOM_LT");

	OpenTest(hKey, lsSubkey);	// 2: ERROR_FILE_NOT_FOUND (Ű�� ���� ������ �߻�)
								// 6: ERROR_INVALID_HANDLE (Ű�� ���� ���߱� ������ �߻�)
	CreateTest(hKey, lsSubkey);	// 5: ERROR_SUCCESS (Ű�� ���� ������, ����->���Ѻ���)
	DeleteTest(hKey, lsSubkey);	// 2: ERROR_FILE_NOT_FOUND (Ű�� ���� ������ �߻�)
	system("pause");			// ������ ����â�� ���ٰ� �����
	return 0;
}
