#include <windows.h>
#include <locale>

int main()
{
	HKEY hKey = 0;
	LONG res = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
	 L"SYSTEM\\ControlSet001\\Services\\RemoteRegistry", 0, KEY_READ, &hKey);
	if(ERROR_SUCCESS == res) {
		WCHAR data[1024];
		DWORD dwData;
		::RegQueryValueEx(hKey, L"Description", NULL, NULL, (LPBYTE)data, &dwData);
		::setlocale(LC_ALL, "korean");
		::wprintf(L"Data : %s\n", data);
		::RegCloseKey(hKey);
	}
	::getchar();
	return 0;
}
