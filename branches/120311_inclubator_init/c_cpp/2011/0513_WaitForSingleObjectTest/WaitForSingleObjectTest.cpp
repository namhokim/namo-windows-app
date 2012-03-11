// WaitForSingleObjectTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hEvent;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	DWORD dwCurr, dwMax, dwTimeout, dwWFSO;

	dwCurr = 0;
	dwMax = (INFINITE-1);
	dwTimeout = 1;

	while(1) {
		printf("WaitForSingleObject (%u/%u)\n", dwCurr, dwMax);
		dwWFSO = WaitForSingleObject(hEvent, dwTimeout);
		printf(" res : %d\n\n", dwWFSO);
		if(WAIT_OBJECT_0==dwWFSO) break;

		dwCurr++;
		if(dwCurr>dwMax) SetEvent(hEvent);
	}
	return 0;
}

