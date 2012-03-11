#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

BOOL CALLBACK EnumDesktopProc(
  __in  LPTSTR lpszDesktop,
  __in  LPARAM lParam
);

BOOL MakeProcess(PTSTR szPath, PTSTR desktopName);

int main()
{
	HDESK hDeskDef = ::OpenDesktop(
					TEXT("Default"),		// The name of the desktop to be opened
					0,
					TRUE,					// processes created by this process will inherit the handle
					DESKTOP_SWITCHDESKTOP	// The access to the desktop.
					);
	if(NULL==hDeskDef) {
		printf("Cannot open 'Default Desktop' : %u\n", ::GetLastError());
		return EXIT_FAILURE;
	}

	HDESK hDesk = ::CreateDesktop(
					TEXT("Billing{7D331537-A3FD-430a-94AD-F3386908D196}"),	// 생성할 데스트톱 이름(대소문자 구분)
					NULL,					// Reserved 반드시 NULL
					NULL,					// Reserved 반드시 NULL
					0,						// 0 또는 DF_ALLOWOTHERACCOUNTHOOK
					DESKTOP_CREATEWINDOW | DESKTOP_SWITCHDESKTOP,	// 데스크톱 요청 권한
					NULL
					);

	if(NULL!=hDesk) {
		
		// 프로세스를 특정 데스크톱에 실행시킨다
		BOOL res = MakeProcess(TEXT("rundll32.exe infocardapi.dll,ManageCardSpace"), TEXT("Billing{7D331537-A3FD-430a-94AD-F3386908D196}"));

		if(res) {
			res = ::SwitchDesktop(hDesk);	// 특정 데스크톱이 보이게 하고, 활성화함
			printf("SwitchDesktop : %u\n", res);

			Sleep(5000);						// 5초간 대기

			res = ::SwitchDesktop(hDeskDef);	// 기본 데스크톱으로 전환
		}

		::CloseDesktop(hDesk);
	}

	return EXIT_SUCCESS;
}

BOOL MakeProcess(PTSTR szPath, PTSTR desktopName)
{
	// 프로세스를 생성하기 위해 startupinfo 구조체를 준비
	STARTUPINFO si = { sizeof(si) };
	si.lpDesktop = desktopName;

	PROCESS_INFORMATION pi;
	TCHAR szCommandLine[MAX_PATH];
	wsprintf(szCommandLine, TEXT("%s"), szPath);

	return ::CreateProcess(NULL, szCommandLine, NULL, NULL,
		FALSE, 0, NULL, NULL, &si, &pi);
}