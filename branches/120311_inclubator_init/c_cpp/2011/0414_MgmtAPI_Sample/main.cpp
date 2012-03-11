#include <stdio.h>
#include <MgmtAPI.h>	// MGMT API
#pragma comment(lib, "MgmtAPI.Lib")		// for SNMP Management API {SnmpMgrOpen, SnmpMgrClose}
#pragma comment(lib, "SnmpAPI.Lib")		// for SNMP Utility API

#define	TEST

void run();

int main()
{
#ifdef	TEST
	Sleep(10000);

	unsigned long cnt = 1;
	while(cnt <= 1000000) {
		printf(" #.%d >\n", cnt);
#endif

		run();

#ifdef	TEST
		Sleep(1000);
		++cnt;
	}

	Sleep(INFINITE);
#endif
	return 0;
}

void run()
{
	INT nTimeOut	= 3000;
	INT nRetries	= 0;
	LPSNMP_MGR_SESSION session;
	LPSTR ip		= "10.17.1.22";
	LPSTR community = "public";

	// 초기화
	session = ::SnmpMgrOpen(ip, community, nTimeOut, nRetries);
	if(NULL == session) {				// error
		printf(" ### SnmpMgrOpen() error : %d\n", GetLastError());
		// SNMP_MEM_ALLOC_ERROR (1)
		return;
	}


	//////////////////////////////
	SNMPAPI res;
	SnmpVarBindList varBindings;
	AsnInteger errorStatus, errorIndex;

	varBindings.list	= (SnmpVarBind *)::SnmpUtilMemAlloc( sizeof(SnmpVarBind) );	// alloc if success
	varBindings.len		= 1;
	
	// 자동으로 iso.org.dod.internet.mgmt. (1.3.6.1.2.) 이 붙음, 아니면 .1.3.6.1.2.1.1.0 식으로 쓴다
	::SnmpMgrStrToOid("1.1.0", &varBindings.list[0].name);							// alloc if success
	varBindings.list[0].value.asnType = ASN_NULL;

	// SNMP GET
	res = ::SnmpMgrRequest(session, SNMP_PDU_GET, &varBindings, &errorStatus, &errorIndex);
	if(NULL == res) {
		printf(" ### snmp error : %d / errorStatus : %d / errorIndex : %d\n",
			GetLastError(), errorStatus, errorIndex);
		//SNMP_MGMTAPI_TIMEOUT (40), SNMP_MGMTAPI_SELECT_FDERRORS(41)
	} else {
		// 결과 출력
		if(varBindings.list[0].value.asnType == ASN_OCTETSTRING) {
			BYTE * stream;
			UINT   length;

			stream = varBindings.list[0].value.asnValue.string.stream;
			length = varBindings.list[0].value.asnValue.string.length;

			for(UINT i=0; i<length; ++i) {
				printf("%c", stream[i]);
			}
			printf("%\n");
		}
	}

	SnmpUtilOidFree(&varBindings.list[0].name);		// by SnmpMgrStrToOid
	SnmpUtilAsnAnyFree(&varBindings.list[0].value);	// by SnmpMgrRequest
	SnmpUtilMemFree(varBindings.list);				// by SnmpUtilMemAlloc


	//////////////////////////////

	// 해제
	::SnmpMgrClose(session);
}