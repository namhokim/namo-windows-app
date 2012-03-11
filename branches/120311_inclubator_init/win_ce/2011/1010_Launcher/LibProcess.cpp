#include "StdAfx.h"
#include "LibProcess.h"

namespace process {	

	int Execute(CString& exeFile, int arg1) {
		TCHAR szArgs[20] = {0};
		wsprintf(szArgs, _T("%d"), arg1);

		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		// PE 파일인지 우선체크(텍스트 다운로드를 받아버리는 경우에는 꼬이기 때문)

		BOOL bRes = ::CreateProcess(exeFile, szArgs, NULL, NULL, NULL, 0, NULL, NULL, &si, &pi);
		if(bRes) {
			DWORD dwWait, exitCode;

			// 스레드 핸들이 필요없기에 바로 삭제
			CloseHandle(pi.hThread);

			// 차일드 프로세스가 종료시까지 대기
			while(1) {
				dwWait = WaitForSingleObject(pi.hProcess, MAX_WAIT_TIME_FOR_TEST_PROCESS);
				if(WAIT_TIMEOUT==dwWait) {
					// 프로세스가 계속 살아있다 (NG
					TerminateProcess(pi.hProcess, EXIT_FAILURE);
				} else {
					break;
				}
			}

			// 종료코드 구함
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