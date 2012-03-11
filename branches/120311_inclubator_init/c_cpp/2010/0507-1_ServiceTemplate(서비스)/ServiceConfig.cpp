/******************************************************************************
Module:  ServiceConfig.cpp
Notices: Copyright (c) 2010 Namho Kim
Purpose: 서비스 관리(Configuration)에 관련된 함수의 구현
			- 설치, 제거, 이벤트 로그 기록
See : botton of this code
******************************************************************************/

#include "ServiceMain.h"

#ifdef	PROVIDE_SERVICE_CONFIG

///////////////////////////////////////////////////////////////////////////////
// Ref. http://msdn.microsoft.com/en-us/library/ms683500(VS.85).aspx
//
// Purpose: 
//   Installs a service in the SCM database
//
// Parameters:
//   None
// 
// Return value:
//   None
//
VOID SvcInstall()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR szPath[MAX_PATH];

	if( !GetModuleFileName( NULL, szPath, MAX_PATH ) ) 
	{
		printf("Cannot install service (%d)\n", GetLastError());
		return;
	}

	// Get a handle to the SCM database. 
	schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) 
	{
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return;
	}

	// Create the service
	schService = CreateService( 
		schSCManager,              // SCM database 
		SVC_NAME,                  // name of service (서비스 이름)
		SVC_DISPLAY_NAME,          // service name to display (표시 이름)
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SVC_START_TYPE,				// start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		szPath,                    // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,                      // no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 

	if (schService == NULL) 
	{
		printf("CreateService failed (%d)\n", GetLastError()); 
		CloseServiceHandle(schSCManager);
		return;
	}
	else {
		printf("Service installed successfully\n");
		if(SVC_START_AFTER_INSTALL && ::StartService(schService, 0, NULL)) {
			printf("Service started\n");
		} else {
			printf("Service cannot started (%u)\n", ::GetLastError());
		}
	}

#ifdef	SVC_DESCRIPTION		// 서비스 설명문이 정의되어 있을 경우
	SERVICE_DESCRIPTION lpDesc;
	lpDesc.lpDescription = SVC_DESCRIPTION;
	ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &lpDesc);
#endif

	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
}

///////////////////////////////////////////////////////////////////////////////
// Ref. http://msdn.microsoft.com/en-us/library/ms682571(VS.85).aspx
//
// Purpose: 
//   Uninstalls a service in the SCM database
//   만약 서비스가 실행중이라면, 중지된 이후에 실제 삭제된다
//
// Parameters:
//   None
// 
// Return value:
//   None
//
VOID SvcUninstall()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;

	// Get a handle to the SCM database. 
	schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) {
		printf("OpenSCManager failed (%d)\n", GetLastError());
		return;
	}

	// Get a handle to the service.
	schService = OpenService( 
		schSCManager,	// SCM database 
		SVC_NAME,		// name of service (서비스 이름)
		SERVICE_STOP | DELETE	// desired access
		);

	if (NULL == schService) {
		printf("OpenService failed (%d)\n", GetLastError()); 
		CloseServiceHandle(schSCManager);
		return;
	}

	// Stop the service.
	if(SVC_STOP_BEFORE_UNINSTALL) {
		SERVICE_STATUS ss;
		::ControlService(schService, SERVICE_CONTROL_STOP, &ss);// 중지신호
		if(ss.dwCurrentState != SERVICE_STOPPED) {		// 중지여부 체크
			Sleep(ss.dwWaitHint);						// 기다린 후
			QueryServiceStatus(schService, &ss);		// 다시 체크
		}
		if(ss.dwCurrentState == SERVICE_STOPPED) {		// 서비스 멈추었나?
			printf("Service was stoped\n"); 
		} else {
			printf("Service will be just deleted marked\n"); 
		}
	}

	// Delete the service.
    if (! DeleteService(schService) ) {
        printf("DeleteService failed (%d)\n", GetLastError()); 
	} else {
		printf("Service deleted successfully\n"); 
	}


	CloseServiceHandle(schService); 
	CloseServiceHandle(schSCManager);
}

#endif	/* PROVIDE_SERVICE_CONFIG */

///////////////////////////////////////////////////////////////////////////////
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
// 
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(__in LPTSTR szFunction) 
{ 
	DWORD	dwLastError = GetLastError();
    HANDLE	hEventSource;
    LPCTSTR	lpszStrings[2];
    TCHAR	Buffer[80];

    hEventSource = RegisterEventSource(NULL, SVC_NAME);

    if( NULL != hEventSource )
    {
        StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, dwLastError);

        lpszStrings[0] = SVC_NAME;
        lpszStrings[1] = Buffer;

        ReportEvent(hEventSource,        // event log handle
                    EVENTLOG_ERROR_TYPE, // event type
                    0,                   // event category
                    SVC_ERROR,           // event identifier
                    NULL,                // no security identifier
                    2,                   // size of lpszStrings array
                    0,                   // no binary data
                    lpszStrings,         // array of strings
                    NULL);               // no binary data

        DeregisterEventSource(hEventSource);
    }
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
