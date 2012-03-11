#include "StdAfx.h"
#include "LibProcess.h"

namespace process {	

	int Execute(CString& exeFile, int arg1) {
		TCHAR szArgs[20] = {0};
		wsprintf(szArgs, _T("%d"), arg1);

		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		// PE �������� �켱üũ(�ؽ�Ʈ �ٿ�ε带 �޾ƹ����� ��쿡�� ���̱� ����)

		BOOL bRes = ::CreateProcess(exeFile, szArgs, NULL, NULL, NULL, 0, NULL, NULL, &si, &pi);
		if(bRes) {
			DWORD dwWait, exitCode;

			// ������ �ڵ��� �ʿ���⿡ �ٷ� ����
			CloseHandle(pi.hThread);

			// ���ϵ� ���μ����� ����ñ��� ���
			while(1) {
				dwWait = WaitForSingleObject(pi.hProcess, MAX_WAIT_TIME_FOR_TEST_PROCESS);
				if(WAIT_TIMEOUT==dwWait) {
					// ���μ����� ��� ����ִ� (NG
					TerminateProcess(pi.hProcess, EXIT_FAILURE);
				} else {
					break;
				}
			}

			// �����ڵ� ����
			::GetExitCodeProcess(pi.hProcess, &exitCode);
			CString msg;
			msg.Format(L"GetExitCodeProcess code : %d\n", exitCode);
			MessageBox(NULL, msg, NULL, MB_OK);
			CloseHandle(pi.hProcess);
			return exitCode;
		} else {
			CString msg;
			msg.Format(L"CreateProcess Error : %d\n", GetLastError());
			MessageBox(NULL, msg, NULL, MB_OK);
		}
		return EXIT_FAILURE;
	} /* int Execute(CString& exeFile, int arg1) { */
} /* namespace process */