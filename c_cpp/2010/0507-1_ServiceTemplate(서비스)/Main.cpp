#include "ServiceMain.h"  // ���� ������ �� ������� �� �� �ִ�(��. ���񽺸�)

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
// ȣ�� ����
// _tmain()				// main.cpp
// -> ServiceMain()		// ServiceMain.cpp
// -> SvcInit()			// ServiceMain.cpp
// -> DoService()		// ServiceImple.cpp : ���� �����ڴ� ���⸦ �����ϸ� ��
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