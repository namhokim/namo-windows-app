// CreateProcessWin32Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	LPCTSTR pszApplicationName = _T("%SystemRoot%\\system32\\calc.exe");
	TCHAR szCommandLine[MAX_PATH] = {0};
//	lstrcpy(szCommandLine, _T("C:\\WINDOWS\\system32\\notepad.exe"));
	lstrcpy(szCommandLine, _T(" calc"));

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	BOOL bSuccess = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE,
		0, NULL, NULL, &si, &pi);
	DWORD dw = ::GetLastError();
	if(bSuccess) {
		// ������ �ڵ��� �ʿ���⿡ �ٷ� ����
		CloseHandle(pi.hThread);


		// ���ϵ� ���μ����� ����ñ��� ���
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);

	}

	return 0;
}

