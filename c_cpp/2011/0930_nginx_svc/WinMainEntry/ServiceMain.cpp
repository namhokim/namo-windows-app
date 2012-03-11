#include "ServiceMain.h"
#include "OutputDebugPrintf.h"	// for OutputDebugPrintf
#include "ExeInfo.h"
#include <strsafe.h>			// for StringCchPrintf

SERVICE_STATUS          gSvcStatus; 
SERVICE_STATUS_HANDLE   gSvcStatusHandle; 
HANDLE                  ghSvcStopEvent = NULL;
LPTSTR					szSvcName = SVCNAME;

///////////////////////////////////////////////////////////////////////////////
VOID WINAPI SvcCtrlHandler( DWORD dwCtrl );
VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint);


///////////////////////////////////////////////////////////////////////////////
VOID WINAPI ServiceMain(
  __in  DWORD dwArgc,
  __in  LPTSTR *lpszArgv
)
{
	// Register the handler function for the service
	gSvcStatusHandle = RegisterServiceCtrlHandler( 
        szSvcName, 
        SvcCtrlHandler);

    if( !gSvcStatusHandle )
    { 
        SvcReportEvent(TEXT("RegisterServiceCtrlHandler")); 
        return; 
    } 

    // These SERVICE_STATUS members remain as set here
    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    gSvcStatus.dwServiceSpecificExitCode = 0;    

    // Report initial status to the SCM
    ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );

    // Perform service-specific initialization and work.
    SvcInit( dwArgc, lpszArgv );
}


VOID SvcReportEvent(LPTSTR szFunction)
{ 
	HANDLE hEventSource;
	LPCTSTR lpszStrings[2];
	TCHAR Buffer[80];

    hEventSource = ::RegisterEventSource(NULL, szSvcName);

    if( NULL != hEventSource )
    {
        ::StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

        lpszStrings[0] = szSvcName;
        lpszStrings[1] = Buffer;

        ::ReportEvent(hEventSource,        // event log handle
                    EVENTLOG_ERROR_TYPE, // event type
                    0,                   // event category
                    SVC_ERROR,           // event identifier
                    NULL,                // no security identifier
                    2,                   // size of lpszStrings array
                    0,                   // no binary data
                    lpszStrings,         // array of strings
                    NULL);               // no binary data

        ::DeregisterEventSource(hEventSource);
    }
}


//
// Purpose: 
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
// 
// Return value:
//   None
//
VOID WINAPI SvcCtrlHandler( DWORD dwCtrl )
{
   // Handle the requested control code. 
   switch(dwCtrl) 
   {  
      case SERVICE_CONTROL_STOP: 
         ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

         // Signal the service to stop.
         ::SetEvent(ghSvcStopEvent);
         ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
         
         return;
      case SERVICE_CONTROL_INTERROGATE: 
         break; 
 
      default: 
         break;
   } 
   
}

//
// Purpose: 
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation, 
//     in milliseconds
// 
// Return value:
//   None
//
VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.
    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

	if (dwCurrentState == SERVICE_START_PENDING) {
        gSvcStatus.dwControlsAccepted = 0;
	} else {
		gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	}

    if ( (dwCurrentState == SERVICE_RUNNING) ||
		(dwCurrentState == SERVICE_STOPPED) ) {
        gSvcStatus.dwCheckPoint = 0;
	} else {
		gSvcStatus.dwCheckPoint = dwCheckPoint++;
	}

    // Report the status of the service to the SCM.
	::SetServiceStatus( gSvcStatusHandle, &gSvcStatus );
}

VOID ReadIni(LPTSTR szStartCmd, LPTSTR szEndCmd, DWORD dwLength);

VOID SvcInit( DWORD dwArgc, LPTSTR *lpszArgv)
{
    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call ReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   ReportSvcStatus with SERVICE_STOPPED.

    // Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.
    ghSvcStopEvent = ::CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( ghSvcStopEvent == NULL)
    {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

    // Report running status when initialization is complete.
    ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );

	// Get Current exe directory
	TCHAR szCurrDir[MAX_PATH] = {0};
	GetExeDirectory(szCurrDir);

	// Get From INI
	TCHAR szStartCmd[MAX_PATH+MAX_PATH] = {0};
	TCHAR szEndCmd[MAX_PATH+MAX_PATH] = {0};
	ReadIni(szStartCmd, szEndCmd, MAX_PATH+MAX_PATH);

	// Run Start Command Line
	BOOL bSuccess;
	PROCESS_INFORMATION pi;
	if(lstrlen(szStartCmd)>0) {
		STARTUPINFO si = { sizeof(si) };
		bSuccess = CreateProcess(NULL, szStartCmd, NULL, NULL, FALSE, 
			0, NULL, szCurrDir, &si, &pi);
		if(bSuccess) {
			// 스레드 핸들이 필요없기에 바로 삭제
			CloseHandle(pi.hThread);
		}
	}

	// TO_DO: Perform work until service stops.
	while(1)
	{
		DWORD dwWait = 5000;	// INFINITE

		// Check whether to stop the service.
		if(WAIT_TIMEOUT!=WaitForSingleObject(pi.hProcess, 0)) break;

		// Check whether process out
		if(bSuccess && WAIT_TIMEOUT!=WaitForSingleObject(ghSvcStopEvent, dwWait)) break;
    }

	// 핸들 닫기
	if(bSuccess) CloseHandle(pi.hProcess);

	// Run End Command Line
	if(lstrlen(szEndCmd)>0) {
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		BOOL bSuccess = CreateProcess(NULL, szEndCmd, NULL, NULL, FALSE, 
			0, NULL, szCurrDir, &si, &pi);
		if(bSuccess) {
			// 스레드 핸들이 필요없기에 바로 삭제
			CloseHandle(pi.hThread);

			// 대기
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
		}
	}

	// Notify to SCN for Terminate
	ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
	return;
}

BOOL GetIniFilename(LPTSTR filename, DWORD dwLength)
{
    DWORD dw = GetModuleFileName(NULL,filename,dwLength);

    if(dw!=0) {    // SUCCESS
        filename[dw-3] = TEXT('i');
        filename[dw-2] = TEXT('n');
        filename[dw-1] = TEXT('i');

        return TRUE;

    } else {
        dw = GetLastError();
        OutputDebugPrintf(TEXT("Failed to Get INI Filename from module (%u)"), dw);
        return FALSE;
    }
}

VOID ReadIni(LPTSTR szStartCmd, LPTSTR szEndCmd, DWORD dwLength)
{
	// INI 파일 이름 생성
	TCHAR iniFile[MAX_PATH] = {0};
	if(GetIniFilename(iniFile, MAX_PATH)) {
		GetPrivateProfileString(TEXT("NGINX_SERVICE"), TEXT("STARTCMD"), NULL, szStartCmd, dwLength, iniFile);
		OutputDebugPrintf(TEXT("STARTCMD: %s"), szStartCmd);
		GetPrivateProfileString(TEXT("NGINX_SERVICE"), TEXT("ENDCMD"), NULL, szEndCmd, dwLength, iniFile);
		OutputDebugPrintf(TEXT("ENDCMD: %s"), szEndCmd);
	}
}