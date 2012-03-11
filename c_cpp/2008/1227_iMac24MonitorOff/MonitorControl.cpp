#include "MonitorControl.h"
//extern bool isValidMove;
extern HWND hWndMain;

MonitorControl::MonitorControl(void)
:IntervalTime(20)
{
	memset(&deviceMode, 0, sizeof(DEVMODE));
}

MonitorControl::~MonitorControl(void)
{
}

bool MonitorControl::setMode(DWORD width, DWORD height, DWORD dwFlag)
{
	DEVMODE mode;
	// mode.dmBitsPerPel = 16; // 16비트 칼라로 변경
	mode.dmPelsWidth = width;
	mode.dmPelsHeight = height;
	mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	return (ChangeDisplaySettings(&mode, dwFlag) == DISP_CHANGE_SUCCESSFUL);
}

DWORD WINAPI tfMonitorCtrl(LPVOID lpParam)
{
	HANDLE hEvent = (HANDLE)lpParam;

//	Sleep(1000);
//	isValidMove = true;
	MonitorControl mc;
	while(WaitForSingleObject(hEvent, 0) ==WAIT_TIMEOUT) {
		mc.setMode(800,600,CDS_FULLSCREEN);
		SetForegroundWindow(hWndMain);
//		ShowWindow(hWndMain, SW_SHOWMAXIMIZED);
		Sleep(mc.IntervalTime);
		mc.setMode(1920,1200,CDS_FULLSCREEN);
		SetForegroundWindow(hWndMain);
		Sleep(mc.IntervalTime);
//		ShowWindow(hWndMain, SW_SHOWMAXIMIZED);
	}

	ExitProcess(0);	// 부모 메인 윈도우 종료
	return 0;
}
