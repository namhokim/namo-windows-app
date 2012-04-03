#include "Registry.h"
#include <stdio.h>

void OpenTest(HKEY hKey, LPCTSTR lsSubkey)
{
	LONG lRet;
	HKEY hkResult;
	LPCTSTR lpValueName = TEXT("value");
	DWORD type, cbData = 0;

	// RegOpenKeyEx function
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724897(v=vs.85).aspx
	lRet = RegOpenKeyEx(hKey, lsSubkey, 0, KEY_QUERY_VALUE | KEY_WOW64_32KEY, &hkResult);
	printf("OpenTest - RegOpenKeyEx : %u\n", lRet);
	lRet = RegQueryValueEx(hkResult, lpValueName, NULL, &type, NULL, &cbData);
	printf("OpenTest- RegQueryValueEx : %u\n", lRet);
	if(ERROR_SUCCESS==lRet) RegCloseKey(hkResult);
}

void CreateTest(HKEY hKey, LPCTSTR lsSubkey)
{
	LONG lRet;
	HKEY hkResult;
	DWORD dwDisposition;

	// RegCreateKeyEx function
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724844(v=vs.85).aspx
	lRet = RegCreateKeyEx(hKey, lsSubkey, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_READ | KEY_WOW64_32KEY , NULL, &hkResult, &dwDisposition);
	// 87(KEY_WOW64_32KEY | KEY_WOW64_64KEY): 매개 변수가 틀립니다. 
	printf("CreateTest : %u\n", lRet);
	if(ERROR_SUCCESS==lRet) RegCloseKey(hkResult);

}

void DeleteTest(HKEY hKey, LPCTSTR lsSubkey)
{
	LONG lRet;

	// RegDeleteKeyEx function
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724847(v=vs.85).aspx
	lRet = RegDeleteKeyEx(hKey, lsSubkey, KEY_WOW64_32KEY, 0);
	printf("DeteteTest : %u\n", lRet);
}