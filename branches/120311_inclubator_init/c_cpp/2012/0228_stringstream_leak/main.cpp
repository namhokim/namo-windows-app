#include <conio.h>		// for _getch
#include <string>
#include <sstream>
using namespace std;
#include <WinSnmp.h>
#pragma comment(lib, "Wsnmp32.lib")

// constants
const unsigned long UNIT = 10000;
//LPCSTR AgentIP = "127.0.0.1";
LPCSTR AgentIP = "10.17.1.22";
UINT SnmpPort = 161;
smiBYTE Public[] = {'p', 'u', 'b', 'l', 'i', 'c'};
LPCSTR Oid = "1.3.6.1.2.1.1.1.0";
smiINT32 ReqID = 1;

// function prototypes
void Test();
SNMPAPI_STATUS CALLBACK wCallBackFunc(
	HSNMP_SESSION hSession, HWND hWnd, UINT wMsg,
	WPARAM wParam, LPARAM lParam, LPVOID lpClientData);

int main()
{
	smiUINT32 major, minor, lev, tr, retr;
	SnmpStartup(&major, &minor, &lev, &tr, &retr);	// not SNMPAPI_FAILURE

	printf("Wait for test...\n");
	_getch();
	unsigned long long dw = 0;
	while (true) {
		printf("%llu>\n", ++dw);
		Test();
		if(dw % UNIT == 0) {
			if(dw == UNIT) {
				printf("Please record curent VM, Handles...\n");
				printf("After press enter...\n");
				_getch();
			} else {
				Sleep(10000);
			}
		}
	}

	SnmpCleanup();
	return 0;
}

void Test()
{
	// Event
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	// not NULL
	ResetEvent(hEvent);

	HSNMP_SESSION hSession = SnmpCreateSession(0, 0, wCallBackFunc, hEvent);	// not SNMPAPI_FAILURE
	HSNMP_ENTITY hEntity = SnmpStrToEntity(hSession, AgentIP);
	SnmpSetPort(hEntity, SnmpPort);
	
	// Community Context
	smiOCTETS dCtx;
	dCtx.len = sizeof(Public);	// 6
	dCtx.ptr = Public;
	HSNMP_CONTEXT hContext = SnmpStrToContext(hSession, &dCtx);

	// Make VBL
	smiOID dName;
	smiVALUE dValue;
	SnmpStrToOid(Oid, &dName);
	dValue.syntax			= SNMP_SYNTAX_NULL;
	dValue.value.uNumber	= 0;
	HSNMP_VBL hVBL = SnmpCreateVbl(hSession, &dName, &dValue);

	// Make PDU
	HSNMP_PDU hPDU = SnmpCreatePdu(hSession, SNMP_PDU_GET, ReqID, 0, 0, hVBL);

	// Request GET & Wait
	SNMPAPI_STATUS status = SnmpSendMsg(hSession, 0, hEntity, hContext, hPDU);
	::WaitForSingleObject(hEvent, INFINITE);

	// Clean up
	SnmpFreePdu(hPDU);			// SnmpCreatePdu
	SnmpFreeDescriptor(SNMP_SYNTAX_OID, (smiLPOPAQUE)&dName);	// SnmpStrToOid
	SnmpFreeContext(hContext);	// SnmpStrToContext
	SnmpFreeEntity(hEntity);	// SnmpStrToEntity
	SnmpClose(hSession);		// SnmpCreateSession
	CloseHandle(hEvent);		// CreateEvent
}

#define EXECUTE_STRING_STREAM
#define EXECUTE_STRING_STREAM_STRING

void stringstream_test()
{
#ifdef EXECUTE_STRING_STREAM_STRING
	std::string what;
	for(int i=0; i< 10000; ++i) what.append("string");
#else
	std::stringstream ss;
	for(int i=0; i< 10000; ++i) ss << "string";
#endif
}

SNMPAPI_STATUS CALLBACK wCallBackFunc(
	HSNMP_SESSION hSession, HWND hWnd, UINT wMsg,
	WPARAM wParam, LPARAM lParam, LPVOID lpClientData)
{
	HANDLE hEvent = lpClientData;
	switch(wParam) {
		case SNMP_ERROR_NOERROR:	// 0
			break;
		case SNMPAPI_TL_TIMEOUT:	// 108
			break;
		// ...
	}
#ifdef EXECUTE_STRING_STREAM
	stringstream_test();
#endif
	SetEvent(hEvent);
	return 0;
}
