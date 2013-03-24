// SRT.cpp : main source file for SRT.exe
//

#include "stdafx.h"
#include "MainFrm.h"	// 메인 폼

#define WINDOW_SIZE_X	548
#define WINDOW_SIZE_Y	358

CAppModule _Module;
// CMainFrame의 윈도우 클래스(for 중복실행방지)
LPCTSTR App_GUID = TEXT("NAMO:AD599917-B130-42a6-9977-BA5414530C66");

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	// TODO : 듀얼모니터의 경우도 고려(차후)
	RECT rect = {0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y};
	if(wndMain.CreateEx(NULL, rect) == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	// 동일한 프로그램이 실행되는지 확인
	HWND hPreWnd = ::FindWindow(App_GUID, NULL);
	if(NULL!=hPreWnd) {			// 같은 윈도우 클래스가 존재한다면
		if(IsIconic(hPreWnd)) {	// 최소화 되어 있다면
			// Active 시키고
			ShowWindow(hPreWnd, SW_RESTORE);
			//SetTray(hPreWnd, NULL, NIM_DELETE);
		}
		
		SetForegroundWindow(hPreWnd);
		return 0;				// 현재 프로그램은 종료
	}

	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

// If you want to use "toolbar, statusbar, trackbar, tooltips" you can use the following call
// make initialize common contols.
//  AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
