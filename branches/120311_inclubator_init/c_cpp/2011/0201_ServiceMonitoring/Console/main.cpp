#include <Windows.h>
#include <stdio.h>

typedef VOID( CALLBACK * PFN_SC_NOTIFY_CALLBACK ) (
    IN PVOID pParameter 
);

int main()
{
	// Open SCM
	SC_HANDLE scm;
	scm = ::OpenSCManager(
			NULL,						// Local computer
			SERVICES_ACTIVE_DATABASE,	// Service control manager database¸í
			SC_MANAGER_ENUMERATE_SERVICE// DesiredAccess
			);
	if(NULL==scm) {
		DWORD dw = ::GetLastError();
		switch(dw) {
			case ERROR_ACCESS_DENIED:
				break;
			case ERROR_DATABASE_DOES_NOT_EXIST:
				break;
			default:
				break;
		}
		return 0;
	}

	SERVICE_NOTIFY sn;
	DWORD dwNSSC = ::NotifyServiceStatusChange(scm,
						SERVICE_NOTIFY_STOPPED | SERVICE_NOTIFY_RUNNING | SERVICE_NOTIFY_PAUSED,
						&sn);
	if(ERROR_SUCCESS==dwNSSC) {
		DWORD dw = SleepEx(10000, TRUE);
		printf("Alerted : %d\n", dw);
	} else {
		printf("NotifyServiceStatusChange failed : %d\n", dwNSSC);
	}

	::CloseServiceHandle(scm);
	return 0;
}
