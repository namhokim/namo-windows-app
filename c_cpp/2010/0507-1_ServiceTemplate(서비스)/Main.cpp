#include "ServiceMain.h"  // 서비스 설정은 이 헤더에서 할 수 있다(예. 서비스명)

///////////////////////////////////////////////////////////////////////////////
// Purpose: 
//   Entry point for the process
//
// Parameters:
//   None
// 
// Return value:
//   None
//
// 호출 과정
// _tmain()				// main.cpp
// -> ServiceMain()		// ServiceMain.cpp
// -> SvcInit()			// ServiceMain.cpp
// -> DoService()		// ServiceImple.cpp : 따라서 개발자는 여기를 구현하면 됨
//
void __cdecl _tmain(int argc, TCHAR *argv[]) 
{
#ifdef	PROVIDE_SERVICE_CONFIG
	// If command-line parameter is "install", install the service. 
    // Otherwise, the service is probably being started by the SCM.

    if( lstrcmpi( argv[1], TEXT("install")) == 0 ) {
        SvcInstall();
        return;
    } else if( lstrcmpi( argv[1], TEXT("uninstall")) == 0 ) {
		SvcUninstall();
        return;
	}
#endif	/* PROVIDE_SERVICE_CONFIG */

	// TO_DO: Add any additional services for the process to this table.
    SERVICE_TABLE_ENTRY DispatchTable[] = { 
        { SVC_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain }, 
        { NULL, NULL } 
    }; 
 
    // This call returns when the service has stopped. 
    // The process should simply terminate when the call returns.

    if (!StartServiceCtrlDispatcher( DispatchTable )) { 
        SvcReportEvent(TEXT("StartServiceCtrlDispatcher")); 
    }
}