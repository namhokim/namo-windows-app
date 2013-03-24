#include "stdafx.h"
#include "ShReg.h"

// Ű�� ���縦 Ȯ���Ѵ�.(����Ű/üũŰ �и�����)
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey, LPCTSTR checkKey) {
	BOOL returValue=FALSE;
	TCHAR fullSubKeyPath[1024];	// ��ü ��ģ �н�(ũ�� Ȯ�ο��!!)
	HKEY hKey=NULL;				// ������Ʈ�� Ű

	// subKey�� checkKey�� ��ģ��.
	wsprintf(fullSubKeyPath, TEXT("%s\\%s"), subKey, checkKey);

	// RegOpenKeyEx �� �����.
	RegOpenKeyEx(keyDomain, fullSubKeyPath, 0,KEY_ALL_ACCESS, &hKey);

	// hKey�� �ڵ��� ���ƿ��� ���� ��ȯ
	if(NULL == hKey) returValue = FALSE;	// �ڵ��� ����(=>Ű ������)
	else {
		returValue = TRUE;		// �ڵ��� ���ȴ�(=>Ű����)
		RegCloseKey(hKey);		// ������Ʈ�� �ڵ��� �ݾ��ش�.
	}
	
	return returValue;	
}

// Ű�� ���縦 Ȯ���Ѵ�.(����Ű/üũŰ ��ü����)
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey) {
	BOOL returValue=FALSE;
	HKEY hKey=NULL;				// ������Ʈ�� Ű

	// RegOpenKeyEx �� �����.
	RegOpenKeyEx(keyDomain, subKey, 0,KEY_ALL_ACCESS, &hKey);

	// hKey�� �ڵ��� ���ƿ��� ���� ��ȯ
	if(NULL == hKey) returValue = FALSE;	// �ڵ��� ����(=>Ű ������)
	else {
		returValue = TRUE;		// �ڵ��� ���ȴ�(=>Ű����)
		RegCloseKey(hKey);		// ������Ʈ�� �ڵ��� �ݾ��ش�.
	}
	
	return returValue;	
}

// ���ڿ��� �д´�.
BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPTSTR lpRet, DWORD nSize) 
{
	DWORD Size;

	// Ű�� �����.
	RegOpenKeyEx(hKey, lpKey, 0,KEY_ALL_ACCESS, &hKey);
	
	// hKey�� �ڵ��� ���ƿ��� ���� ��ȯ
	if(NULL == hKey) return FALSE;	// �ڵ��� ����(=>Ű ������)
	else {							// �ڵ��� ���ȴ�(=>Ű����)
		Size=nSize;
		if (RegQueryValueEx(hKey, lpValue, 0, NULL,(LPBYTE)lpRet, &Size)
			!=ERROR_SUCCESS) {
			return FALSE;
		}
		RegCloseKey(hKey);		// ������Ʈ�� �ڵ��� �ݾ��ش�.
	}
	return TRUE;
}

// �������� �д´�.
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

// ���ڿ��� �д´�.
BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, 
   LPTSTR lpRet, DWORD nSize) 
{
	HKEY key;
	DWORD dwDisp;
	DWORD Size;

	// Ű�� �����ϴ��� ã�´�.
	if(!IsExistKey(hKey,lpKey)) return FALSE;	// Ű�� ������ ����

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

// �������� ����.
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

// ���ڿ��� ����.
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

