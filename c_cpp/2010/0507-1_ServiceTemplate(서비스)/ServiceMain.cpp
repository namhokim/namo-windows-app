/******************************************************************************
Module:  ServiceMain.cpp
Notices: Copyright (c) 2010 Namho Kim
Purpose: ���� ���η�ƾ
See : botton of this code
******************************************************************************/

#include "ServiceMain.h"
#include "ServiceImpl.h"	// ������� ���� ������ ���ϴ�
#include <stdlib.h>

#pragma comment(lib, "advapi32.lib")

///////////////////////////////////////////////////////////////////////////////
// ��������
SERVICE_STATUS          gSvcStatus; 
SERVICE_STATUS_HANDLE   gSvcStatusHandle; 
HANDLE                  ghSvcStopEvent = NULL;	// ���� �ñ׳�
HANDLE                  ghSvcPauseEvent = NULL;	// �Ͻ����� �ñ׳�

///////////////////////////////////////////////////////////////////////////////
// ���� �Լ�����
VOID WINAPI SvcCtrlHandler( __in DWORD );
VOID ReportSvcStatus(__in DWORD, __in DWORD, __in DWORD);
VOID SvcInit( __in DWORD, __in LPTSTR * );

///////////////////////////////////////////////////////////////////////////////
//
// Purpose: 
//   ���񽺸� ���� ������(Entry point)
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   ����
//

VOID WINAPI ServiceMain(__in  DWORD dwArgc, __in  LPTSTR *lpszArgv)
{
	// Register the handler function for the service
	gSvcStatusHandle = RegisterServiceCtrlHandler(SVC_NAME, SvcCtrlHandler);

	if( !gSvcStatusHandle ) { 
		SvcReportEvent(TEXT("RegisterServiceCtrlHandler")); 
		return; 
	}

	// These SERVICE_STATUS members remain as set here
	gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;	// ����
	gSvcStatus.dwServiceSpecificExitCode = EXIT_SUCCESS;

	// Report initial status to the SCM
	ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );	// 3�� �ҿ�(��Ʈ)

	// Perform service-specific initialization and work.
	SvcInit( dwArgc, lpszArgv );
}

///////////////////////////////////////////////////////////////////////////////
//
// Purpose: 
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
// 
// Return value:
//   ����
//
VOID WINAPI SvcCtrlHandler( __in DWORD dwCtrl )
{
   // Handle the requested control code. 
   switch(dwCtrl) {  
      case SERVICE_CONTROL_STOP: 
         ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

         // Signal the service to stop.
         SetEvent(ghSvcStopEvent);
         ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
         
         return;
 
      case SERVICE_CONTROL_INTERROGATE:		// ��Ʈ�� ����
         break;

	  //case SERVICE_CONTROL_PAUSE:
		 // ReportSvcStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0);
		 // // Non-signal the service to continue
		 // ResetEvent(ghSvcPauseEvent);
		 // ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
		 // break;

	  //case SERVICE_CONTROL_CONTINUE:
		 // ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 0);
		 // // Signal the service to continue
		 // SetEvent(ghSvcPauseEvent);
		 // ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
		 // break;
 
      default: 
         break;
   } 
}


///////////////////////////////////////////////////////////////////////////////
//
// Purpose: 
//   ���� ������ ���¸� �����ϰ� SCM���� ����
//
// Parameters:
//   dwCurrentState - ���� ����(see SERVICE_STATUS)
//		http://msdn.microsoft.com/en-us/library/ms685996(VS.85).aspx
//   dwWin32ExitCode - �ý��� ���� �ڵ�
//   dwWaitHint - ���� �۾�(pending)�� �����ð�(����. �и���)
// 
// Return value:
//   None
//
VOID ReportSvcStatus(__in DWORD dwCurrentState, __in DWORD dwWin32ExitCode, __in DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;

	// SERVICE_STATUS ����ü ���� ä��
	gSvcStatus.dwCurrentState = dwCurrentState;
	gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
	gSvcStatus.dwWaitHint = dwWaitHint;

	if (dwCurrentState == SERVICE_START_PENDING) {
		gSvcStatus.dwControlsAccepted = 0;
	} else {
		gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	}

	if ( (dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED) ) {
		gSvcStatus.dwCheckPoint = 0;
	} else {
		gSvcStatus.dwCheckPoint = dwCheckPoint++;
	}

	// ������ ���¸� SCM���� ����
	SetServiceStatus( __in gSvcStatusHandle, __in &gSvcStatus );
}


///////////////////////////////////////////////////////////////////////////////
//
// Purpose: 
//   ���� �ʱ�ȭ
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   ����
//
VOID SvcInit( __in DWORD dwArgc, __in LPTSTR *lpszArgv)
{
	// TEST
	HANDLE hFile = CreateFile(L"c:\\service.test", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	char data[1024];
	for(DWORD i=0; i<dwArgc; ++i) {
		StringCchPrintfA(data, 1024, "%u) %S\r\n", i, lpszArgv[i]);

		DWORD dwWritten;
		WriteFile(hFile, data, strlen(data), &dwWritten, 0);
	}
	CloseHandle(hFile);

    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call ReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   ReportSvcStatus with SERVICE_STOPPED.

    // Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.
    ghSvcStopEvent = CreateEvent(
                         NULL,    // �⺻ ���� �Ӽ���
                         TRUE,    // ���� ���� �̺�Ʈ
                         FALSE,   // �ʱ���� : �� �ñ׳�
                         NULL);   // �̸� ���� �̺�Ʈ

    if ( ghSvcStopEvent == NULL) {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

	// �̺�Ʈ ����
	ghSvcPauseEvent = CreateEvent(
                         NULL,    // �⺻ ���� �Ӽ���
                         TRUE,    // ���� ���� �̺�Ʈ
                         TRUE,    // �ʱ���� : �ñ׳�(������ ����)
                         NULL);   // �̸� ���� �̺�Ʈ

	 if ( ghSvcPauseEvent == NULL) {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

    // Report running status when initialization is complete.
    ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );

    // TO_DO: Perform work until service stops.
    while(1) {
        DoService();	// ���� ���� (ServiceImple.cpp�� ����)

        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

	// �̺�Ʈ ��ü�� �ݴ´�
	CloseHandle(ghSvcPauseEvent);
	CloseHandle(ghSvcStopEvent);
}

///////////////////////////////////////////////////////////////////////////////
//
// Purpose: 
//   ���񽺰� ���� �ñ׳� �������� ��ȯ
// 
// Return value:
//   ��/����
//
bool isStopSignaled()
{
	return (WAIT_OBJECT_0 == WaitForSingleObject(ghSvcStopEvent, 0) );
}

///////////////////////////////////////////////////////////////////////////////
//
// Purpose: 
//   ���񽺰� �Ͻ� ���� �ñ׳� �������� ��ȯ
//
// Return value:
//   ��/����
//
bool isPauseSignaled()
{
	return (WAIT_OBJECT_0 == WaitForSingleObject(ghSvcPauseEvent, 0) );
}




/*
Copyright (c) 2010  Namho Kim

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */
