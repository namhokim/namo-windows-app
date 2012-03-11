#include <Windows.h>
#include <Process.h>	// for _beginthreadex
#include <cstdio>

//#define	THREAD_CRT_VERSION	1

DWORD WINAPI ClientConn(LPVOID arg)
{
	int i = (*(int *)arg);
	printf("\tstart ClientConn() : %d\n", i);
	Sleep(1000);
	printf("\tend ClientConn()\n");
	return 0;
}

void ThreadTest()
{
	HANDLE hThread;
	DWORD dwThreadID;
	int arg = 1;
	hThread =
#ifdef	THREAD_CRT_VERSION
	(HANDLE)_beginthreadex(NULL,0,
		(unsigned int(__stdcall*)(void*))ClientConn,
		(void*)0, 0, (unsigned *)&dwThreadID);
#else
		::CreateThread(NULL, 0,
			ClientConn,
			(LPVOID)&arg, 0, &dwThreadID);
#endif
	Sleep(3000);
	::CloseHandle(hThread);
}

int main()
{
	while(1)	{
		Sleep(1500);
		printf("call ThreadTest()\n");
		ThreadTest();
		printf("after call ThreadTest()\n");
		Sleep(1500);
	}
	return 0;
}