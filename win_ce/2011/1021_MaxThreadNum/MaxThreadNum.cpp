// MaxThreadNum.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MaxThreadNum.h"
#include <vector>

using namespace std;

typedef struct _THREAD_INFO {
	DWORD dwThreadID;
	HANDLE hThread;
} THREAD_INFO;

// http://msdn.microsoft.com/en-us/library/ms913287.aspx
DWORD WINAPI ThreadProc(LPVOID lpParameter);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	DWORD dwThID;
	HANDLE hThread;
	vector<THREAD_INFO> v;

	// http://msdn.microsoft.com/en-us/library/ms939170.aspx
	do {
		hThread = CreateThread(NULL, 0, &ThreadProc, NULL, CREATE_SUSPENDED, &dwThID);

		if(NULL!=hThread) {
			THREAD_INFO th;
			th.dwThreadID = dwThID;
			th.hThread = hThread;
			v.push_back(th);
		}
	} while (NULL != hThread);

	TCHAR msg[1024] = {0};
	wsprintf(msg, _T("Number of threads : %u"), v.size());
	MessageBox(NULL, msg, NULL, MB_OK);

	return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	return 0;
}