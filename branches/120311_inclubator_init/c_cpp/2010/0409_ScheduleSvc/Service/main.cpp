#include <Windows.h>
#include <stdlib.h>
#include "Utils.h"
#include <iostream>

const short int Hour = 14;
const short int Min = 17;
const short int Sec = 0;

int main(int argc, char *argv[])
{
	LARGE_INTEGER liUTC;
	Time::GetLocalDueTime(&liUTC, Hour, Min, Sec, 0);

	enum _KernelObjects {
		KO_EVENT, KO_TIMER, KO_SIZE
	};

	// 이벤트 생성
	HANDLE hKerObj[KO_SIZE];
	hKerObj[KO_EVENT] = CreateEvent(NULL, FALSE, FALSE, TEXT("TESTTIMER"));
	// 자동 리셋 타이머를 생성
	hKerObj[KO_TIMER] =  ::CreateWaitableTimer(NULL, FALSE, NULL);

	// 대기 타이머 설정
	SetWaitableTimer(hKerObj[KO_TIMER], &liUTC, /*6*60*60**/1000, NULL, NULL, FALSE);

	int cnt = 0;
	while (1) {
		DWORD dw = ::WaitForMultipleObjects(KO_SIZE, hKerObj, FALSE, INFINITE);
		if(WAIT_OBJECT_0 == dw || WAIT_FAILED == dw) {
			std::cout << "Event Signaled : " << dw << std::endl;
			break;
		}
		std::cout << "Timer Signaled : " << dw << std::endl;
		if((++cnt)>5) {
			CloseHandle(hKerObj[KO_TIMER]);
		}
	}
	std::cout << "Terminated" << std::endl;

	CloseHandle(hKerObj[KO_EVENT]);
	CloseHandle(hKerObj[KO_TIMER]);

	return EXIT_SUCCESS;
}
