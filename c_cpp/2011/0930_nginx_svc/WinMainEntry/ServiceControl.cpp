#include "ServiceControl.h"
#include "ServiceMain.h"		// extern LPTSTR szSvcName;
#include "OutputDebugPrintf.h"	// for OutputDebugPrintfA
#include <stdlib.h>

BOOL __stdcall StopDependentServices(SC_HANDLE schSCManager, SC_HANDLE schService);

int __stdcall DoStartSvc()
{
    SERVICE_STATUS_PROCESS ssStatus; 
    DWORD dwOldCheckPoint; 
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR buf[MESSAGE_FORMAT_MAX];

    // Get a handle to the SCM database. 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        OutputDebugPrintf(TEXT("OpenSCManager failed (%s)\n"),
			ErrorCodeToString(buf, GetLastError()));

        return EXIT_FAILURE;
    }

    // Get a handle to the service.
    schService = OpenService( 
        schSCManager,         // SCM database 
        szSvcName,            // name of service 
        SERVICE_ALL_ACCESS);  // full access 
 
    if (schService == NULL)
    { 
        OutputDebugPrintf(TEXT("OpenService failed (%s,%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        ::CloseServiceHandle(schSCManager);

        return EXIT_FAILURE;
    }    

    // Check the status in case the service is not stopped. 
    if (!QueryServiceStatusEx( 
            schService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // information level
            (LPBYTE) &ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded ) )              // size needed if buffer is too small
    {
        OutputDebugPrintf(TEXT("QueryServiceStatusEx failed (%s,%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        ::CloseServiceHandle(schService); 
        ::CloseServiceHandle(schSCManager);

        return EXIT_FAILURE; 
    }

    // Check if the service is already running. It would be possible 
    // to stop the service here, but for simplicity this example just returns. 
    if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
    {
        OutputDebugPrintf(TEXT("Cannot start the [%s] service because it is already running\n"),
			szSvcName);
        ::CloseServiceHandle(schService); 
        ::CloseServiceHandle(schSCManager);

        return EXIT_FAILURE; 
    }

    // Save the tick count and initial checkpoint.
    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    // Wait for the service to stop before attempting to start it.
    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 
 
        dwWaitTime = ssStatus.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

        Sleep( dwWaitTime );

        // Check the status until the service is no longer stop pending.
        if (!QueryServiceStatusEx( 
                schService,                     // handle to service 
                SC_STATUS_PROCESS_INFO,         // information level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // size needed if buffer is too small
        {
			OutputDebugPrintf(TEXT("QueryServiceStatusEx failed (%s:%s)\n"),
				szSvcName, ErrorCodeToString(buf, GetLastError()));
            ::CloseServiceHandle(schService); 
            ::CloseServiceHandle(schSCManager);

            return EXIT_FAILURE; 
        }

        if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
        {
            // Continue to wait and check.
            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
            {
                OutputDebugPrintf(TEXT("Timeout waiting for [%s] service to stop\n"),
					szSvcName);
                ::CloseServiceHandle(schService); 
                ::CloseServiceHandle(schSCManager);

                return EXIT_FAILURE; 
            }
        }
    }

    // Attempt to start the service.
    if (!StartService(
            schService,  // handle to service 
            0,           // number of arguments 
            NULL) )      // no arguments 
    {
		OutputDebugPrintf(TEXT("StartService failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        ::CloseServiceHandle(schService); 
        ::CloseServiceHandle(schSCManager);
        return EXIT_FAILURE; 
	} else {
		OutputDebugPrintf(TEXT("[%s] Service start pending...\n"),
			szSvcName); 
	}

    // Check the status until the service is no longer start pending. 
    if (!QueryServiceStatusEx( 
            schService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // info level
            (LPBYTE) &ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded ) )              // if buffer too small
    {
		OutputDebugPrintf(TEXT("QueryServiceStatusEx failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        ::CloseServiceHandle(schService); 
        ::CloseServiceHandle(schSCManager);

        return EXIT_FAILURE; 
    }
 
    // Save the tick count and initial checkpoint.
    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
    { 
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth the wait hint, but no less than 1 second and no 
        // more than 10 seconds. 
 
        dwWaitTime = ssStatus.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

        Sleep( dwWaitTime );

        // Check the status again. 
        if (!QueryServiceStatusEx( 
            schService,             // handle to service 
            SC_STATUS_PROCESS_INFO, // info level
            (LPBYTE) &ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded ) )              // if buffer too small
        {
            OutputDebugPrintf(TEXT("QueryServiceStatusEx failed (%s:%s)\n"),
				szSvcName, ErrorCodeToString(buf, GetLastError()));
            break; 
        }
 
        if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
        {
            // Continue to wait and check.
            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
            {
                // No progress made within the wait hint.
                break;
            }
        }
    } 

    // Determine whether the service is running.
    if (ssStatus.dwCurrentState == SERVICE_RUNNING) 
    {
        OutputDebugPrintf(TEXT("[%s] Service started successfully.\n"),
			szSvcName); 

		::CloseServiceHandle(schService); 
		::CloseServiceHandle(schSCManager);

		return EXIT_SUCCESS;
    }
    else 
    { 
        OutputDebugPrintf(TEXT("[%s] Service not started. \n"), szSvcName);
        OutputDebugPrintf(TEXT("  Current State: %d\n"), ssStatus.dwCurrentState); 
        OutputDebugPrintf(TEXT("  Exit Code: %d\n"), ssStatus.dwWin32ExitCode); 
		OutputDebugPrintf(TEXT("  Check Point: %d\n"), ssStatus.dwCheckPoint); 
		OutputDebugPrintf(TEXT("  Wait Hint: %d\n"), ssStatus.dwWaitHint); 

		::CloseServiceHandle(schService); 
		::CloseServiceHandle(schSCManager);

		return EXIT_FAILURE;
	}
}

int __stdcall DoStopSvc()
{
    SERVICE_STATUS_PROCESS	ssp;
    DWORD					dwStartTime = ::GetTickCount();
    DWORD					dwBytesNeeded;
    DWORD					dwTimeout = 30000; // 30-second time-out
    DWORD					dwWaitTime;
	SC_HANDLE				schSCManager;
	SC_HANDLE				schService;
	TCHAR					buf[MESSAGE_FORMAT_MAX];
	int						retVal = EXIT_FAILURE;

    // Get a handle to the SCM database. 
    schSCManager = ::OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        OutputDebugPrintf(TEXT("OpenSCManager failed (%s)\n"),
			ErrorCodeToString(buf, GetLastError()));
        return EXIT_FAILURE;
    }

    // Get a handle to the service.
    schService = ::OpenService( 
        schSCManager,         // SCM database 
        szSvcName,            // name of service 
        SERVICE_STOP | 
        SERVICE_QUERY_STATUS | 
        SERVICE_ENUMERATE_DEPENDENTS);  
 
    if (schService == NULL)
    { 
        OutputDebugPrintf(TEXT("OpenService failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        CloseServiceHandle(schSCManager);
        return EXIT_FAILURE;
    }    

    // Make sure the service is not already stopped.
    if ( !::QueryServiceStatusEx( 
            schService, 
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp, 
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded ) )
    {
        OutputDebugPrintf(TEXT("QueryServiceStatusEx failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        goto stop_cleanup;
    }

    if ( ssp.dwCurrentState == SERVICE_STOPPED )
    {
        OutputDebugPrintf(TEXT("[%s] Service is already stopped.\n"), szSvcName);
		retVal = EXIT_SUCCESS;
        goto stop_cleanup;
    }

    // If a stop is pending, wait for it.
    while ( ssp.dwCurrentState == SERVICE_STOP_PENDING ) 
    {
        OutputDebugPrintf(TEXT("[%s] Service stop pending...\n"), szSvcName);

        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 
 
        dwWaitTime = ssp.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

        Sleep( dwWaitTime );

        if ( !QueryServiceStatusEx( 
                 schService, 
                 SC_STATUS_PROCESS_INFO,
                 (LPBYTE)&ssp, 
                 sizeof(SERVICE_STATUS_PROCESS),
                 &dwBytesNeeded ) )
        {
            OutputDebugPrintf(TEXT("QueryServiceStatusEx failed (%s)\n"),
				ErrorCodeToString(buf, GetLastError()));
            goto stop_cleanup;
        }

        if ( ssp.dwCurrentState == SERVICE_STOPPED )
        {
            OutputDebugPrintf(TEXT("[%s] Service stopped successfully.\n"), szSvcName);
			retVal = EXIT_SUCCESS;
            goto stop_cleanup;
        }

        if ( GetTickCount() - dwStartTime > dwTimeout )
        {
            OutputDebugPrintf(TEXT("[%s] Service stop timed out.\n"), szSvcName);
            goto stop_cleanup;
        }
    }

    // If the service is running, dependencies must be stopped first.
    StopDependentServices(schSCManager, schService);

    // Send a stop code to the service.
    if ( !ControlService( 
            schService, 
            SERVICE_CONTROL_STOP, 
            (LPSERVICE_STATUS) &ssp ) )
    {
        OutputDebugPrintf(TEXT( "ControlService failed (%s:%s)\n"),
			szSvcName, ErrorCodeToString(buf, GetLastError()));
        goto stop_cleanup;
    }

    // Wait for the service to stop.
    while ( ssp.dwCurrentState != SERVICE_STOPPED ) 
    {
        Sleep( ssp.dwWaitHint );
        if ( !QueryServiceStatusEx( 
                schService, 
                SC_STATUS_PROCESS_INFO,
                (LPBYTE)&ssp, 
                sizeof(SERVICE_STATUS_PROCESS),
                &dwBytesNeeded ) )
        {
            OutputDebugPrintf(TEXT( "QueryServiceStatusEx failed (%s:%s)\n"),
				szSvcName, ErrorCodeToString(buf, GetLastError()));
            goto stop_cleanup;
        }

        if ( ssp.dwCurrentState == SERVICE_STOPPED )
            break;

        if ( GetTickCount() - dwStartTime > dwTimeout )
        {
            OutputDebugPrintf(TEXT( "[%s] Wait timed out\n"), szSvcName);
            goto stop_cleanup;
        }
    }
    OutputDebugPrintf(TEXT("[%s] Service stopped successfully\n"), szSvcName);
	retVal = EXIT_SUCCESS;

stop_cleanup:
    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);

	return retVal;
}

BOOL __stdcall StopDependentServices(SC_HANDLE schSCManager, SC_HANDLE schService)
{
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if ( EnumDependentServices( schService, SERVICE_ACTIVE, 
         lpDependencies, 0, &dwBytesNeeded, &dwCount ) ) 
    {
         // If the Enum call succeeds, then there are no dependent
         // services, so do nothing.
         return TRUE;
    } 
    else 
    {
        if ( GetLastError() != ERROR_MORE_DATA )
            return FALSE; // Unexpected error

        // Allocate a buffer for the dependencies.
        lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc( 
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded );
  
        if ( !lpDependencies )
            return FALSE;

        __try {
            // Enumerate the dependencies.
            if ( !EnumDependentServices( schService, SERVICE_ACTIVE, 
                lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                &dwCount ) )
            return FALSE;

            for ( i = 0; i < dwCount; i++ ) 
            {
                ess = *(lpDependencies + i);
                // Open the service.
                hDepService = OpenService( schSCManager, 
                   ess.lpServiceName, 
                   SERVICE_STOP | SERVICE_QUERY_STATUS );

                if ( !hDepService )
                   return FALSE;

                __try {
                    // Send a stop code.
                    if ( !ControlService( hDepService, 
                            SERVICE_CONTROL_STOP,
                            (LPSERVICE_STATUS) &ssp ) )
                    return FALSE;

                    // Wait for the service to stop.
                    while ( ssp.dwCurrentState != SERVICE_STOPPED ) 
                    {
                        Sleep( ssp.dwWaitHint );
                        if ( !QueryServiceStatusEx( 
                                hDepService, 
                                SC_STATUS_PROCESS_INFO,
                                (LPBYTE)&ssp, 
                                sizeof(SERVICE_STATUS_PROCESS),
                                &dwBytesNeeded ) )
                        return FALSE;

                        if ( ssp.dwCurrentState == SERVICE_STOPPED )
                            break;

                        if ( GetTickCount() - dwStartTime > dwTimeout )
                            return FALSE;
                    }
                } 
                __finally 
                {
                    // Always release the service handle.
                    CloseServiceHandle( hDepService );
                }
            }
        } 
        __finally 
        {
            // Always free the enumeration buffer.
            HeapFree( GetProcessHeap(), 0, lpDependencies );
        }
    } 
    return TRUE;
}
