#include "RegDevmode.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <string>

using namespace std;

wchar_t*	szPrefixW = L"SOFTWARE\\SINDOH\\";
wchar_t*	szPostfixW = L"\\devmode";
wchar_t*	szDevmodeW = L"bDevMode";

char*		szPrefixA = "SOFTWARE\\SINDOH\\";
char*		szPostfixA = "\\devmode";
char*		szDevmodeA = "bDevMode";

bool GetDevmodeW(wchar_t* app, LPLtEnvVarsW data)
{
	if(app==NULL) {
		::SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}

	HKEY hKey;
	wstring subkey(szPrefixW);
	subkey.append(app);
	subkey.append(szPostfixW);

	if(ERROR_SUCCESS==RegOpenKeyExW(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ, &hKey)) {
		if(data!=NULL) {
			DWORD dwVal, dwType, cbData;
			cbData = sizeof(dwVal);
			RegQueryValueExW(hKey, L"autobackup", 0, &dwType, (LPBYTE)&data->autobackup, &cbData);
			RegQueryValueExW(hKey, L"autoupdate", 0, &dwType, (LPBYTE)&data->autoupdate, &cbData);
			cbData = MAX_URL_LENGTH;
			RegQueryValueExW(hKey, L"backup_base_url", 0, &dwType, (LPBYTE)&data->backup_base_url, &cbData);
			cbData = MAX_URL_LENGTH;
			RegQueryValueExW(hKey, L"rcpt", 0, &dwType, (LPBYTE)&data->rcpt, &cbData);
			cbData = MAX_URL_LENGTH;
			RegQueryValueExW(hKey, L"reqrelease_jsp", 0, &dwType, (LPBYTE)&data->reqrelease_jsp, &cbData);
		}
		RegCloseKey(hKey);
		return true;
	} else {
		return false;
	}
}

bool GetDevmodeA(char* app, LPLtEnvVarsA data)
{
	if(app==NULL) {
		::SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}

	HKEY hKey;
	string subkey(szPrefixA);
	subkey.append(app);
	subkey.append(szPostfixA);

	if(ERROR_SUCCESS==RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey.c_str(), 0, KEY_READ, &hKey)) {
		if(data!=NULL) {
			DWORD dwVal, dwType, cbData;
			cbData = sizeof(dwVal);
			RegQueryValueExA(hKey, "autobackup", 0, &dwType, (LPBYTE)&data->autobackup, &cbData);
			RegQueryValueExA(hKey, "autoupdate", 0, &dwType, (LPBYTE)&data->autoupdate, &cbData);
			cbData = MAX_URL_LENGTH;
			RegQueryValueExA(hKey, "backup_base_url", 0, &dwType, (LPBYTE)&data->backup_base_url, &cbData);
			cbData = MAX_URL_LENGTH;
			RegQueryValueExA(hKey, "rcpt", 0, &dwType, (LPBYTE)&data->rcpt, &cbData);
			cbData = MAX_URL_LENGTH;
			RegQueryValueExA(hKey, "reqrelease_jsp", 0, &dwType, (LPBYTE)&data->reqrelease_jsp, &cbData);
		}
		RegCloseKey(hKey);
		return true;
	} else {
		return false;
	}
}

bool SetDevmodeW(wchar_t* app, const LPLtEnvVarsW data)
{
	if(app==NULL || data==NULL) {
		::SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}

	DWORD dwDisposition;
	HKEY hKey;
	wstring subkey(szPrefixW);
	subkey.append(app);
	subkey.append(szPostfixW);

	if(ERROR_SUCCESS==RegCreateKeyExW(HKEY_LOCAL_MACHINE, subkey.c_str(),
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition)) {
		RegCloseKey(hKey);
		return true;
	} else {
		return false;
	}

	return false;
}

bool SetDevmodeA(char* app, const LPLtEnvVarsA data)
{
	return false;
}