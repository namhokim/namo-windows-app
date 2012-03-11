#include <Windows.h>
#include <Lmcons.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

// 콜백함수
BOOL CALLBACK EnumDesktopProc(
  __in  LPTSTR lpszDesktop,
  __in  LPARAM lParam
);

int main()
{
	// 현재 윈도우 스테이션이 사용중인 데스크톱들을 열거
	BOOL res = ::EnumDesktops(NULL, &EnumDesktopProc, 0);
	if(0!=res) {
		printf("\nReturn of EnumDesktops : %d\n", res);
	} else {	// Error
		printf("\nErron in EnumDesktops : %u\n", ::GetLastError());
	}

	// 현재 윈도우 스테이션의 핸들을 이용하여 데스크톱들을 열거(보통 "WinSta0")
	res = ::EnumDesktops(::GetProcessWindowStation(), &EnumDesktopProc, 0);
	if(0!=res) {
		printf("\nReturn of EnumDesktops : %d\n", res);
	} else {	// Error
		printf("\nErron in EnumDesktops : %u\n", ::GetLastError());
	}

	// "WinSta0"의 데스크톱들을 열거
	HWINSTA hWinSta =
		::OpenWindowStation(TEXT("WinSta0"),	// The name of the window station to be opened
							FALSE,				// If this value is TRUE,
												//processes created by this process will inherit the handle.
												// Otherwise, the processes do not inherit this handle.
							WINSTA_ALL_ACCESS			// The access to the window station
							);
	if(NULL!=hWinSta) {
		// .NET: http://stackoverflow.com/questions/251248/how-can-i-get-the-sid-of-the-current-windows-account
		// http://stackoverflow.com/questions/2017705/get-the-logged-in-windows-user-name-associated-with-a-desktop
		// WMI : http://www.codeproject.com/KB/cs/processownersid.aspx
		DWORD nLenNeeded = 0;
		GetUserObjectInformation(hWinSta, UOI_USER_SID, NULL, 0, &nLenNeeded);
		if(nLenNeeded > 0) {
			DWORD nLen = nLenNeeded;
			LPBYTE pBuf = (LPBYTE)malloc(nLenNeeded);
			BOOL bRes;

			nLenNeeded = 0;
			bRes = GetUserObjectInformation(hWinSta, UOI_USER_SID, pBuf, nLen, &nLenNeeded);
			PISID pSID = (PISID)pBuf;

			TCHAR name[UNLEN + 1] = {0};
			TCHAR domain[UNLEN + 1] = {0};
			DWORD dwName, dwDomain;
			SID_NAME_USE sidType;

			dwName = dwDomain = UNLEN + 1;
			bRes = LookupAccountSid(NULL, pSID, name, &dwName, domain, &dwDomain, &sidType);
			if(!bRes) {
				dwName = GetLastError();	// ERROR_NONE_MAPPED
			}
			free(pBuf);
		}

		res = ::EnumDesktops(hWinSta, &EnumDesktopProc, 0);
		if(0!=res) {
			printf("\nReturn of EnumDesktops : %d\n", res);
		} else {	// Error
			printf("\nErron in EnumDesktops : %u\n", ::GetLastError());
		}
		::CloseWindowStation(hWinSta);
	} else {	// when a meet error
		printf("\nErron in OpenWindowStation : %u\n", ::GetLastError());
	}


	system("pause");

	return EXIT_SUCCESS;
}

BOOL CALLBACK EnumDesktopProc(
  __in  LPTSTR lpszDesktop,
  __in  LPARAM lParam
)
{
	_tprintf(TEXT("%s\n"), lpszDesktop);
	return TRUE;
}