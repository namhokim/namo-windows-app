#include "StdAfx.h"
#include "SnmpExtension.h"
#include <Snmp.h>

#pragma comment(lib, "Snmpapi.lib")

// http://msdn.microsoft.com/en-us/library/windows/desktop/aa378018(v=vs.85).aspx

//#pragma comment(lib, "Snmpapi.lib")	// for _SnmpUtilVarBindFree@4, _SnmpUtilOidNCmp@12, _SnmpUtilOidCpy@8 
//#pragma comment(lib, "inetmib1.lib")

// SnmpSvcGetUptime
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa378288(v=vs.85).aspx
typedef DWORD (WINAPI * pSSGU) (void);	

// SnmpExtensionInit
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa378018(v=vs.85).aspx
typedef BOOL(WINAPI * pSEI) (
	_In_  DWORD dwUptimeReference,
	_Out_ HANDLE * phSubagentTrapEvent,
	_Out_ AsnObjectIdentifier * pFirstSupportedRegion);

// SnmpExtensionQuery
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa378021(v=vs.85).aspx
typedef BOOL(WINAPI * pSEQ) (
	_In_     BYTE bPduType,
	_Inout_  SnmpVarBindList *pVarBindList,
	_Out_    AsnInteger32 *pErrorStatus,
	_Out_    AsnInteger32 *pErrorIndex);

namespace Snmp
{
	pSSGU SvcGetUptime = NULL;
	pSEI ExtensionInit = NULL;
	pSEQ ExtensionQuery = NULL;

	BOOL InitLibrary();
}	// namespace snmp

void foo()
{
	BOOL bRes;

	DWORD dwTick;
	HANDLE SubagentTrapEvent;
	AsnObjectIdentifier FirstSupportedRegion;

	// Initialize the variable list
	SnmpVarBindList VarBindList;
	AsnInteger32 ErrorStatus;
	AsnInteger32 ErrorIndex;

	const int LenVarBind = 2;
	SnmpVarBind VarBind[LenVarBind];

	VarBindList.list = VarBind;
	VarBindList.len = 1;

	AsnObjectIdentifier MIB_NULL = {0, 0};
	VarBind[0].name = MIB_NULL;
	VarBind[1].name = MIB_NULL;

	UINT OID_sysDescr[] = {1, 3, 6, 1, 2, 1, 1, 1, 0};
	AsnObjectIdentifier MIB_ifEntryType = 
	{sizeof(OID_sysDescr) / sizeof(UINT), OID_sysDescr};

	SnmpUtilOidCpy(&VarBind[0].name, &MIB_ifEntryType);
	
	// Check
	if (Snmp::InitLibrary()==FALSE) return;

	// Main
	dwTick = Snmp::SvcGetUptime();
	bRes = Snmp::ExtensionInit(dwTick, &SubagentTrapEvent, &FirstSupportedRegion);
	if (bRes==FALSE) return;

	Snmp::ExtensionQuery(SNMP_PDU_GET, &VarBindList, &ErrorStatus, &ErrorIndex);

	printf("Result: %d\n", ErrorStatus);

	return;
}

namespace Snmp
{
	LPCTSTR szSnmpapi_dll = TEXT("Snmpapi.dll");
	LPCTSTR szInetmib1_dll = TEXT("inetmib1.dll");
	// Windows 2000 - C:\WINNT\system32\inetmib1.dll
	// Windows 7 - C:\Windows\SysWOW64\inetmib1.dll
	// SnmpExtensionInit
	// SnmpExtensionInitEx
	// SnmpExtensionQuery
	// SnmpExtensionTrap

	BOOL ResetInitLibrary()
	{
		pSSGU SvcGetUptime = NULL;
		pSEI ExtensionInit = NULL;
		pSEQ ExtensionQuery = NULL;

		return FALSE;
	}

	BOOL InitSnmpapi()
	{
		// Prevent re-initialize
		if (Snmp::SvcGetUptime!=NULL) return TRUE;

		// DLL load
		HMODULE hDll = GetModuleHandle(szSnmpapi_dll);
		if (hDll == NULL)
		{
			hDll = LoadLibrary(szSnmpapi_dll);
			if (hDll == NULL)
			{
				return FALSE;
			}
		}

		// Initialize the Dll functions pointers
		SvcGetUptime = (pSSGU)GetProcAddress(hDll, "SnmpSvcGetUptime");
		if (ExtensionInit==NULL) return ResetInitLibrary();

		return TRUE;
	}

	BOOL InitInetmib1()
	{
		// Prevent re-initialize
		if (ExtensionInit!=NULL) return TRUE;

		// DLL load
		HMODULE hDll = GetModuleHandle(szInetmib1_dll);
		if (hDll == NULL)
		{
			hDll = LoadLibrary(szInetmib1_dll);
			if (hDll == NULL)
			{
				return FALSE;
			}
		}

		// Initialize the Dll functions pointers
		ExtensionInit = (pSEI)GetProcAddress(hDll, "SnmpExtensionInit");
		if (ExtensionInit==NULL) return ResetInitLibrary();
		ExtensionQuery = (pSEQ)GetProcAddress(hDll, "SnmpExtensionQuery");
		if (ExtensionQuery==NULL) return ResetInitLibrary();

		return TRUE;
	}

	// this -> InitInetmib1() | InitSnmpapi()
	BOOL InitLibrary()
	{
		return (InitInetmib1() && InitSnmpapi());
	}

	// Sample Extension-Agent DLL - http://msdn.microsoft.com/en-us/library/aa913887.aspx
}	// namespace snmp