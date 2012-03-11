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
					TEXT("Billing{7D331537-A3FD-430a-94AD-F3386908D196}"),	// ������ ����Ʈ�� �̸�(��ҹ��� ����)
					NULL,					// Reserved �ݵ�� NULL
					NULL,					// Reserved �ݵ�� NULL
					0,						// 0 �Ǵ� DF_ALLOWOTHERACCOUNTHOOK
					DESKTOP_CREATEWINDOW | DESKTOP_SWITCHDESKTOP,	// ����ũ�� ��û ����
					NULL
					);

	if(NULL!=hDesk) {
		
		// ���μ����� Ư�� ����ũ�鿡 �����Ų��
		BOOL res = MakeProcess(TEXT("rundll32.exe infocardapi.dll,ManageCardSpace"), TEXT("Billing{7D331537-A3FD-430a-94AD-F3386908D196}"));

		if(res) {
			res = ::SwitchDesktop(hDesk);	// Ư�� ����ũ���� ���̰� �ϰ�, Ȱ��ȭ��
			printf("SwitchDesktop : %u\n", res);

			Sleep(5000);						// 5�ʰ� ���

			res = ::SwitchDesktop(hDeskDef);	// �⺻ ����ũ������ ��ȯ
		}

		::CloseDesktop(hDesk);
	}

	return EXIT_SUCCESS;
}

BOOL MakeProcess(PTSTR szPath, PTSTR desktopName)
{
	// ���μ����� �����ϱ� ���� startupinfo ����ü�� �غ�
	STARTUPINFO si = { sizeof(si) };
	si.lpDesktop = desktopName;

	PROCESS_INFORMATION pi;
	TCHAR szCommandLine[MAX_PATH];
	wsprintf(szCommandLine, TEXT("%s"), szPath);

	return ::CreateProcess(NULL, szCommandLine, NULL, NULL,
		FALSE, 0, NULL, NULL, &si, &pi);
}