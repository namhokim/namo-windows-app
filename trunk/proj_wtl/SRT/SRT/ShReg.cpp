#include "stdafx.h"
#include "ShReg.h"

// 키의 존재를 확인한다.(서브키/체크키 분리버전)
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey, LPCTSTR checkKey) {
	BOOL returValue=FALSE;
	TCHAR fullSubKeyPath[1024];	// 전체 합친 패스(크기 확인요망!!)
	HKEY hKey=NULL;				// 레지스트리 키

	// subKey와 checkKey를 합친다.
	wsprintf(fullSubKeyPath, TEXT("%s\\%s"), subKey, checkKey);

	// RegOpenKeyEx 로 열어본다.
	RegOpenKeyEx(keyDomain, fullSubKeyPath, 0,KEY_ALL_ACCESS, &hKey);

	// hKey로 핸들이 돌아오는 지로 반환
	if(NULL == hKey) returValue = FALSE;	// 핸들이 없다(=>키 미존재)
	else {
		returValue = TRUE;		// 핸들이 열렸다(=>키존재)
		RegCloseKey(hKey);		// 레지스트리 핸들을 닫아준다.
	}
	
	return returValue;	
}

// 키의 존재를 확인한다.(서브키/체크키 합체버전)
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey) {
	BOOL returValue=FALSE;
	HKEY hKey=NULL;				// 레지스트리 키

	// RegOpenKeyEx 로 열어본다.
	RegOpenKeyEx(keyDomain, subKey, 0,KEY_ALL_ACCESS, &hKey);

	// hKey로 핸들이 돌아오는 지로 반환
	if(NULL == hKey) returValue = FALSE;	// 핸들이 없다(=>키 미존재)
	else {
		returValue = TRUE;		// 핸들이 열렸다(=>키존재)
		RegCloseKey(hKey);		// 레지스트리 핸들을 닫아준다.
	}
	
	return returValue;	
}

// 문자열을 읽는다.
BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPTSTR lpRet, DWORD nSize) 
{
	DWORD Size;

	// 키를 열어본다.
	RegOpenKeyEx(hKey, lpKey, 0,KEY_ALL_ACCESS, &hKey);
	
	// hKey로 핸들이 돌아오는 지로 반환
	if(NULL == hKey) return FALSE;	// 핸들이 없다(=>키 미존재)
	else {							// 핸들이 열렸다(=>키존재)
		Size=nSize;
		if (RegQueryValueEx(hKey, lpValue, 0, NULL,(LPBYTE)lpRet, &Size)
			!=ERROR_SUCCESS) {
			return FALSE;
		}
		RegCloseKey(hKey);		// 레지스트리 핸들을 닫아준다.
	}
	return TRUE;
}

// 정수값을 읽는다.
UINT SHRegReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault)
{
	HKEY key;
	DWORD dwDisp;
	UINT Result;
	DWORD Size;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 		return 0;
	Size=sizeof(LONG);
	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)&Result, &Size)
		!=ERROR_SUCCESS)		Result=nDefault;
	RegCloseKey(key);
	return Result;
}

// 문자열을 읽는다.
BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, 
   LPTSTR lpRet, DWORD nSize) 
{
	HKEY key;
	DWORD dwDisp;
	DWORD Size;

	// 키가 존재하는지 찾는다.
	if(!IsExistKey(hKey,lpKey)) return FALSE;	// 키가 없으면 종료

	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return FALSE;
	Size=nSize;
	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)lpRet, &Size)
		!=ERROR_SUCCESS) {
		_tcscpy_s(lpRet,512,lpDefault);	// strlen(lpRet)-1
		return FALSE;
	}
	RegCloseKey(key);
	return TRUE;
}

// 정수값을 쓴다.
BOOL SHRegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) {
		return FALSE;
	}
	if (RegSetValueEx(key, lpValue,0,REG_DWORD,(LPBYTE)&nData,sizeof(UINT))
		!=ERROR_SUCCESS) 
		return FALSE;
	RegCloseKey(key);
	return TRUE;
}

// 문자열을 쓴다.
BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return FALSE;
	if (RegSetValueEx(key, lpValue,0,REG_SZ,(LPBYTE)lpData,lstrlen(lpData)+1)
		!=ERROR_SUCCESS) 
		return FALSE;
	RegCloseKey(key);
	return TRUE;
}

