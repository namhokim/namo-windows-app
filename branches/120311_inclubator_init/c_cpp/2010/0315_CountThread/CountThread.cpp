#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define MAX_THREADS	(1024*10)

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD threadNum = (DWORD)lpParam;

	while(1) {
		_tprintf(_T("thread num: %d \n"), threadNum);
		Sleep(5000);
	}
	return 0;	// 쓰레드 종료
}

DWORD cntOfThread = 0;

int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadID[MAX_THREADS];
	HANDLE hThread[MAX_THREADS];

	// 생성 가능한 최대 개수의 쓰레드 생성
	while(1) {
		hThread[cntOfThread] = CreateThread(
			NULL,						// 기본 보안 속성
			1024*1024*5,				// 스택 크기(기본:1MB=1024x1024Byte)
			ThreadProc,					// 쓰레드 함수
			(LPVOID)cntOfThread,		// 쓰레드 함수 인자
			0,							// 기본 생성 플래그
			&dwThreadID[cntOfThread]	// 쓰레드 ID 반환
		);

		// 쓰레드 생성 확인
		if(hThread[cntOfThread] == NULL) {
			_tprintf(_T("MAXIMUM THREAD NUMBER: %d \n"), cntOfThread);
			break;
		}
		cntOfThread++;
	}

	for(DWORD i=0; i<cntOfThread; i++) {
		::CloseHandle(hThread[i]);
	}
	return 0;	// 프로세스 종료
}
