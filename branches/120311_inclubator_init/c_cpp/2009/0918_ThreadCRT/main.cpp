#include <Windows.h>
#include <Process.h>	// for _beginthreadex
#include <cstdio>

#define	THREAD_CRT_VERSION	1
#define	USING_SEMAPHORE	1
//#define	USING_MUTEX	1
HANDLE hSema, hMutex;

#ifdef	THREAD_CRT_VERSION
unsigned int __stdcall ClientConn(void *arg)
#else
DWORD WINAPI ClientConn(LPVOID arg)
#endif
{
	int i = (*(int *)arg);
#ifdef	USING_SEMAPHORE
	::ReleaseSemaphore(hSema, 1, NULL);
#endif
#ifdef USING_MUTEX
	::ReleaseMutex(hMutex);
#endif
	printf("\tstart ClientConn() : %d\n", i);
	Sleep(3000);
	//::ReleaseSemaphore(hSema, 1, NULL);
	printf("\tend ClientConn()\n");
	return 0;
}

void ThreadTest()
{
#ifdef	USING_SEMAPHORE
	hSema = ::CreateSemaphore(NULL, 1, 1, NULL);
	if (NULL==hSema) exit(1);
#endif
#ifdef USING_MUTEX
	hMutex = CreateMutex(NULL, TRUE, NULL);
	if(hMutex==NULL) exit(1);
#endif

	HANDLE hThread;
#ifdef	THREAD_CRT_VERSION
	unsigned int dwThreadID;
#else
	DWORD dwThreadID;
#endif

	for(int arg = 0; arg < 10; ) {
#ifdef	USING_SEMAPHORE
		DWORD dwWaitResult = ::WaitForSingleObject(hSema, INFINITE);
		/*printf("Semaphore1 Wait result : %d\n", dwWaitResult);
		dwWaitResult = ::WaitForSingleObject(hSema, INFINITE);
		printf("Semaphore2 Wait result : %d\n", dwWaitResult);*/
#endif
#ifdef USING_MUTEX
		DWORD dwWaitResult = ::WaitForSingleObject(hMutex, INFINITE);
		//printf("Mutex Wait result : %d\n", dwWaitResult);
#endif

		arg++;

		hThread =
#ifdef	THREAD_CRT_VERSION
			(HANDLE)_beginthreadex(NULL, 0, ClientConn, &arg, 0, &dwThreadID);
#else
			::CreateThread(NULL, 0, ClientConn, (LPVOID)&arg, 0, &dwThreadID);
#endif
		::CloseHandle(hThread);
	}
	Sleep(3000);
	

#ifdef	USING_SEMAPHORE
	::CloseHandle(hSema);
#endif
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