#include "StdAfx.h"
#include "SnmpExtension.h"
#include <Snmp.h>

// http://msdn.microsoft.com/en-us/library/windows/desktop/aa378018(v=vs.85).aspx

//#pragma comment(lib, "Snmpapi.lib")	// for _SnmpUtilVarBindFree@4, _SnmpUtilOidNCmp@12, _SnmpUtilOidCpy@8 
//#pragma comment(lib, "inetmib1.lib")

typedef DWORD (WINAPI * pSSGU) ();	// SnmpSvcGetUptime

typedef BOOL(WINAPI * pSEI) (
	IN DWORD dwTimeZeroReference,
	OUT HANDLE * hPollForTrapEvent,
	OUT AsnObjectIdentifier * supportedView);

typedef BOOL(WINAPI * pSEQ) (
	IN BYTE requestType,
	IN OUT RFC1157VarBindList * variableBindings,
	OUT AsnInteger * errorStatus,
	OUT AsnInteger * errorIndex);

typedef void (WINAPI * pSEC) ();	// SnmpExtensionClose

namespace Snmp
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa378288(v=vs.85).aspx
	pSSGU SvcGetUptime = NULL;
	pSEI ExtensionInit = NULL;
	pSEQ ExtensionQuery = NULL;
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa378017(v=vs.85).aspx
	pSEC ExtensionClose = NULL;

	BOOL InitLibrary();
}	// namespace snmp

void foo()
{
	DWORD dwTick;
	HANDLE PollForTrapEvent;
	AsnObjectIdentifier SupportedView;
	
	// Check
	if (Snmp::InitLibrary()==FALSE) return;

	// Main
	dwTick = Snmp::SvcGetUptime();
	Snmp::ExtensionInit(dwTick, &PollForTrapEvent, &SupportedView);

	Snmp::ExtensionClose();

	return;
}

namespace Snmp
{
	BOOL InitLibrary()
	{
		// Prevent re-initialize
		if (Snmp::SvcGetUptime!=NULL) return TRUE;

		// DLL load
		HMODULE m_hInst = GetModuleHandle(TEXT("inetmib1.dll"));
		if (m_hInst < (HINSTANCE) HINSTANCE_ERROR)
		{
			m_hInst = NULL;
			return FALSE;
		}

		// Initialize the Dll functions pointers
		SvcGetUptime = (pSSGU)GetProcAddress(m_hInst, "SnmpSvcGetUptime");
		ExtensionInit = (pSEI)GetProcAddress(m_hInst, "SnmpExtensionInit");
		ExtensionQuery = (pSEQ)GetProcAddress(m_hInst, "SnmpExtensionQuery");
		ExtensionClose = (pSEC)GetProcAddress(m_hInst, "SnmpExtensionClose");

		return TRUE;
	}
}	// namespace snmp