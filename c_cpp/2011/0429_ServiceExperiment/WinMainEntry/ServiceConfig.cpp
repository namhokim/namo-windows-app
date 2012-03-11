#include "ServiceConfig.h"
#include "ServiceControl.h"	// ActCmd{lpCmdLine | start, stop}
#include "ServiceMain.h"		// extern LPTSTR szSvcName;
#include "OutputDebugPrintf.h"	// for OutputDebugPrintfA
#include <stdlib.h>

int __stdcall DoServiceInstall(void)
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR szPath[MAX_PATH];
	TCHAR buf[MESSAGE_FORMAT_MAX];

	if( !GetModuleFileName( NULL, szPath, MAX_PATH ) ) {
		OutputDebugPrintf(TEXT("Cannot install service (%s)\n"),
			ErrorCodeToString(buf, GetLastError()));

		return EXIT_FAILURE;
	}

	// Get a handle to the SCM database. 

	schSCManager = OpenSCManager( 
		NULL,                    // local computer
		NULL,                    // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);  // full access rights 

	if (NULL == schSCManager) {
		OutputDebugPrintf(TEXT("OpenSCManager failed (%s)\n"),
			ErrorCodeToString(buf, GetLastError()));

		return EXIT_FAILURE;
	}

	// Create the service

	schService = CreateService( 
		schSCManager,              // SCM database 
		SVCNAME,                   // name of service 
		SVCNAME,                   // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_DEMAND_START,      // start type 
		SERVICE_ERROR_NORMAL,      // error control type 
		szPath,                    // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,                      // no dependencies 
		NULL,                      // LocalSystem account 
		NULL);                     // no password 

	if (schService == NULL) {
		OutputDebugPrintf(TEXT("CreateService failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
		::CloseServiceHandle(schSCManager);

		return EXIT_FAILURE;
	} else {
		OutputDebugPrintf(TEXT("[%s] Service installed successfully\n"),
			szSvcName); 
	}

	::CloseServiceHandle(schService); 
	::CloseServiceHandle(schSCManager);

	return EXIT_SUCCESS;
}

int __stdcall DoServiceUninstall(void)
{
	SC_HANDLE schSCManager;
    SC_HANDLE schService;
    SERVICE_STATUS ssStatus;
	TCHAR buf[MESSAGE_FORMAT_MAX];

    // Get a handle to the SCM database. 
 
    schSCManager = ::OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) {
        OutputDebugPrintf(TEXT("OpenSCManager failed (%s)\n"),
			ErrorCodeToString(buf, GetLastError()));

        return EXIT_FAILURE;
    }

    // Get a handle to the service.
    schService = ::OpenService( 
        schSCManager,						// SCM database 
        szSvcName,							// name of service 
        SERVICE_QUERY_STATUS | DELETE);		// need delete access 
 
    if (schService == NULL)
    { 
		OutputDebugPrintf(TEXT("OpenService failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        ::CloseServiceHandle(schSCManager);

        return EXIT_FAILURE;
    }

	// Get Current Status
	if (::QueryServiceStatus(schService, &ssStatus) == 0) {
		OutputDebugPrintf(TEXT("QueryServiceStatus failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
		::CloseServiceHandle(schService);
        ::CloseServiceHandle(schSCManager);

		return EXIT_FAILURE;
	} else {
		if(ssStatus.dwCurrentState==SERVICE_RUNNING) {
			// if Running then Stop
			DoStopSvc();
		}
	}

    // Delete the service.
    if (! DeleteService(schService) ) 
    {
        OutputDebugPrintf(TEXT("DeleteService failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
		::CloseServiceHandle(schService);
		::CloseServiceHandle(schSCManager);
		return EXIT_FAILURE;
    } else {
		OutputDebugPrintf(TEXT("[%s] Service deleted successfully\n"),
			szSvcName); 
	}
 
    ::CloseServiceHandle(schService);
    ::CloseServiceHandle(schSCManager);

	return EXIT_SUCCESS;
}