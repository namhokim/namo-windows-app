// CreateProcessWin32Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
		// 스레드 핸들이 필요없기에 바로 삭제
		CloseHandle(pi.hThread);


		// 차일드 프로세스가 종료시까지 대기
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);

	}

	return 0;
}

