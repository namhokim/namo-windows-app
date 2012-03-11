#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

bool IsEventTime()
{
	SYSTEMTIME currentSystemTime;
	const int DatetimeSize = 5;
	char datetime[DatetimeSize];

	memset(datetime,0,DatetimeSize);
	GetLocalTime(&currentSystemTime);
	sprintf_s(datetime, DatetimeSize,"%02d%02d", currentSystemTime.wHour,currentSystemTime.wMinute);

	cout << "current time : " << datetime << endl;

	if(strcmp("1832",datetime)==0) return true;
	else return false;
}

HANDLE g_ExitEvent;
VOID CALLBACK MyTimerProc( 
    HWND hwnd,        // handle to window for timer messages 
    UINT message,     // WM_TIMER message 
    UINT idTimer,     // timer identifier 
    DWORD dwTime)     // current system time 
{ 
	cout << " : called timer procedure\n";
	if(IsEventTime()) SetEvent(g_ExitEvent);
} 

int main(int argc, char* argv[])
{
	UINT_PTR uPtr;

	g_ExitEvent=CreateEvent(NULL, TRUE, FALSE, L"BackToTheFutureStateExit");

	cout << "set Timer\n";
	uPtr = SetTimer(NULL,0,30000,(TIMERPROC) MyTimerProc);
	cout << uPtr << endl;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) && WaitForSingleObject(g_ExitEvent, 0)==WAIT_TIMEOUT)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(WaitForSingleObject(g_ExitEvent, 0)==WAIT_TIMEOUT) break;
	}

	CloseHandle(g_ExitEvent);
	cout << "kill Timer\n";
	KillTimer(NULL, uPtr);

	return 0;
}
