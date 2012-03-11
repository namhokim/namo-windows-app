#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� ����:
#include <tchar.h>

// ���� ��Ʈ�� ��� ����:
#include "ServiceMain.h"
#include "ServiceConfig.h"	// ActCmd{lpCmdLine | install, uninstall}
#include "ServiceControl.h"	// ActCmd{lpCmdLine | start, stop}
#include "ExeInfo.h"		// GetExeDirectory, GetExeFilename

int HandleWithCmdLine(LPTSTR lpCmdLine);
VOID HandleByService();
VOID Usage();

// Entry Point Function
// wWinMain -> HandleWithCmdLine -> ActCmd{lpCmdLine | install, uninstall, autoupdate, start, stop}
//			-> HandleByService
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	if(lstrlen(lpCmdLine)>0) {
		return HandleWithCmdLine(lpCmdLine);
	} else {
		HandleByService();		
		return 0;
	}
}

// Ŀ�ǵ���ο� ���� ó��(�Ϲ� ���ø����̼�)
int HandleWithCmdLine(LPTSTR lpCmdLine)
{
	// install, uninstall, autoupdate, start, stop
	TCHAR szCurrDir[MAX_PATH] = {0,};
	GetExeDirectory(szCurrDir);

	//::MessageBox(NULL, szCurrDir, TEXT("���"), MB_OK);
	if (lstrcmpi( lpCmdLine, TEXT("start")) == 0 )
		return DoStartSvc();
	else if (lstrcmpi( lpCmdLine, TEXT("stop")) == 0 )
		return DoStopSvc();
	else if (lstrcmpi( lpCmdLine, TEXT("install")) == 0 )
		return DoServiceInstall();
	else if (lstrcmpi( lpCmdLine, TEXT("uninstall")) == 0 )
		return DoServiceUninstall();
	else if (lstrcmpi( lpCmdLine, TEXT("autoupdate")) == 0 ) {
		//DoStopSvc();
		return 0;
	} else {
		Usage();
		return 0;
	}
}

// ���� ���ø����̼�
VOID HandleByService()
{
	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{ 
		{ SVCNAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain }, 
		{ NULL, NULL } 
	};


	// This call returns when the service has stopped. 
	// The process should simply terminate when the call returns.
	if (!StartServiceCtrlDispatcher( DispatchTable )) 
	{ 
		SvcReportEvent(TEXT("StartServiceCtrlDispatcher")); 
	}
}

VOID Usage()
{
	TCHAR exeFilename[MAX_PATH] = {0,};
	TCHAR msg[1024];
	GetExeFilename(exeFilename);
	wsprintf(msg, TEXT("%s [install, uninstall, start, stop]"), exeFilename);
	MessageBox(NULL, msg, TEXT("Usage"), MB_OK);
}