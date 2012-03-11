//#define NTDDI_VERSION	NTDDI_WINXP
#include <winsock2.h>
#include <iphlpapi.h>
#include <Icmpapi.h>
#include <stdio.h>


#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

typedef HANDLE (WINAPI *P_ICMP_CREATE_FILE)(void);
typedef BOOL (WINAPI *P_ICMP_CLOSE_HANDLE)(__in HANDLE);
typedef DWORD (P_ICMP_SEND_ECHO

BOOL IsWindowsXP_or_Later();

int main()
{
	// Get OS Version
	BOOL bIsWindowsXPorLater = IsWindowsXP_or_Later();
	HMODULE hDll = NULL;
	
	if(bIsWindowsXPorLater) {						// for Windows XP or later
		printf("LoadLibrary(TEXT(\"Iphlpapi.dll\")");
		hDll = ::LoadLibrary(TEXT("Iphlpapi.dll"));

	} else {
		printf("LoadLibrary(TEXT(\"Icmp.dll\")");
		hDll = ::LoadLibrary(TEXT("Icmp.dll"));		// for Windows 2000 Professional/Server
	}

	if(hDll != NULL) {
		P_ICMP_CREATE_FILE	pIcmpCreateFile = NULL;
		P_ICMP_CLOSE_HANDLE pIcmpCloseHandle = NULL;
		P_ICMP_SEND_ECHO pIcmpSendEcho = NULL;
		HANDLE hIcmp = INVALID_HANDLE_VALUE;

		pIcmpCreateFile		= (P_ICMP_CREATE_FILE)::GetProcAddress(hDll, "IcmpCreateFile");
		pIcmpCloseHandle	= (P_ICMP_CLOSE_HANDLE)::GetProcAddress(hDll, "IcmpCloseHandle");
		pIcmpSendEcho	= (P_ICMP_CLOSE_HANDLE)::GetProcAddress(hDll, "IcmpSendEcho");
		
		if(NULL == pIcmpCreateFile || NULL != pIcmpCloseHandle) {
			::FreeLibrary(hDll);	// versus LoadLibrary
			return 1;
		}

		// Create
		hIcmp = pIcmpCreateFile();		// equivalent to ::IcmpCreateFile();

		// Destroy
		pIcmpCloseHandle(hIcmp);

		::FreeLibrary(hDll);	// versus LoadLibrary
	}

	return 0;
}

BOOL IsWindowsXP_or_Later()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if(0!=GetVersionEx(&osvi)) {
		bIsWindowsXPorLater = 
		( (osvi.dwMajorVersion > 5) ||
		( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1) ));
		return bIsWindowsXPorLater;
	} else {
		return FALSE;
	}
	
}