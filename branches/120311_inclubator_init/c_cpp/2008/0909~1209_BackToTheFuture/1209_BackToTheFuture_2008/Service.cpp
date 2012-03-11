#include <windows.h>
#include <iostream>
using namespace std;

#include "Events.h"
extern const int defaultDuration = 1;

SERVICE_STATUS_HANDLE hServiceStatus;
DWORD g_NowState;
HANDLE g_ExitEvent;
bool g_IsPause;

void BackToTheFutureMain(DWORD argc, LPTSTR *argv);
void BackToTheFutureHandler(DWORD opCode);

void SetStatus(DWORD dwState, DWORD dwAccept=SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE)
{
	SERVICE_STATUS ss;
	ss.dwServiceType=SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState=dwState;
	ss.dwControlsAccepted=dwAccept;
	ss.dwWin32ExitCode=0;
	ss.dwServiceSpecificExitCode=0;
	ss.dwCheckPoint=0;
	ss.dwWaitHint=0;

	// 현재 상태를 보관해 둔다.
	g_NowState=dwState;
	SetServiceStatus(hServiceStatus,&ss);
}

void ErrorDisplay(DWORD lastErrorCode)
{
	switch(lastErrorCode) {
	case ERROR_INVALID_NAME:
		cerr << "The specified service name is invalid.\n";
		break;
	case ERROR_SERVICE_DOES_NOT_EXIST:
		cerr << "The specified service does not exist.\n";
		break;
	}
}

bool Initialize()
{
	SetStatus(SERVICE_START_PENDING);
	g_IsPause = false;
	g_ExitEvent=CreateEvent(NULL, TRUE, FALSE, L"BackToTheFutureStateExit");
	if(NULL == g_ExitEvent) {
		SetStatus(SERVICE_STOPPED);
		return false;
	}
	else {
		SetStatus(SERVICE_RUNNING);
		return true;
	}
}

void Finalize()
{
	SetStatus(SERVICE_STOPPED);
}

void RepeatWork()
{
	//SYSTEMTIME st;

	if(!Initialize()) return;
	while(TRUE) {
		// 반복작업: 현재시간이 예약시간인지 확인 -> 맞으면 BackToTheFuture
		if (g_IsPause != true && IsEventTime()) DoAction();
		if (WaitForSingleObject(g_ExitEvent, defaultDuration*1000) == WAIT_OBJECT_0) break;
	}
	Finalize();
}

void BackToTheFutureMain(DWORD argc, LPTSTR *argv)
{
	hServiceStatus=RegisterServiceCtrlHandlerEx(L"BackToTheFuture",(LPHANDLER_FUNCTION_EX)BackToTheFutureHandler,NULL);
	if (hServiceStatus==0) {		// If the function fails, the return value is zero.
		ErrorDisplay(GetLastError());
		return;
	}
	RepeatWork();
}

void BackToTheFutureHandler(DWORD opCode)
{
	// 현재 상태와 같은 제어 코드일 경우는 처리할 필요 없다.
	if (opCode == g_NowState)
		return;

	switch (opCode) {
	case SERVICE_CONTROL_PAUSE:
		SetStatus(SERVICE_PAUSE_PENDING,0);
		g_IsPause=true;
		SetStatus(SERVICE_PAUSED);
		break;
	case SERVICE_CONTROL_CONTINUE:
		SetStatus(SERVICE_CONTINUE_PENDING,0);
		g_IsPause=false;
		SetStatus(SERVICE_RUNNING);
		break;
	case SERVICE_CONTROL_STOP:
		SetStatus(SERVICE_STOP_PENDING,0);
		SetEvent(g_ExitEvent);
		break;
	case SERVICE_CONTROL_INTERROGATE:
	default:
		SetStatus(g_NowState);
		break;
	}
}