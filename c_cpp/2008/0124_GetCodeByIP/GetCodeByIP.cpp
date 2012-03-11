// GetModelbyIP.cpp : Defines the entry point for the DLL application.
//
/*++ BUILD Version: 0001    // Increment this if a change has global effects

Copyright (c) 1991-1995  Microsoft Corporation

Module Name:

    snmputil.c

Abstract:

    Sample SNMP Management API usage for Windows NT.

    This file is an example of how to code management applications using
    the SNMP Management API for Windows NT.  It is similar in operation to
    the other commonly available SNMP command line utilities.

    Extensive comments have been included to describe its structure and
    operation.  See also "Microsoft Windows/NT SNMP Programmer's Reference".

Created:

    28-Jun-1991

Revision History:

--*/


static char *vcsid = "@(#) $Logfile:   N:/agent/mgmtapi/vcs/snmputil.c_v  $ $Revision:   1.5  $";


// General notes:
//   Microsoft's SNMP Management API for Windows NT is implemented as a DLL
// that is linked with the developer's code.  These APIs (examples follow in
// this file) allow the developer's code to generate SNMP queries and receive
// SNMP traps.  A simple MIB compiler and related APIs are also available to
// allow conversions between OBJECT IDENTIFIERS and OBJECT DESCRIPTORS.


// Necessary includes.

#include <windows.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <io.h>
#include <time.h>
#include <snmp.h>
#include <mgmtapi.h>


// Constants used in this example.

#define GET     1
#define GETNEXT 2
#define WALK    3
#define TRAP    4

#define TIMEOUT 1000 /* milliseconds */
#define RETRIES 1

#define hrPrinterStatus ".1.3.6.1.2.1.25.3.5.1.1.1"
#define prtAlertLocation ".1.3.6.1.2.1.1.1"


typedef struct OID_DATA
{	
	char	ipaddress[32];
    char	community[32];
    char	oid[255];    //요청 OID
    char	oidname[255];	//응답이름
    char	oidvalue[255];	//응답값
} OID_DATA;

#include "GetCodeByIP.h"
// 함수 프로토타입 정의
// 최상위 레이어
int GetModelbyIP(char *ipAddr, char *ModelName);
//상태값을 반환  in:ip,comm    out:status
int FnSindoricohStatus(char *ip,char *comm,int *status, char *ModelName);
//__declspec(dllexport)
int FnSnmpGETDATA(char *ip,char *comm,char *oid,int method,OID_DATA *poidData);
//SnmpWalk 의 하위항목 갯수반환
//__declspec(dllexport) 
int FnWalkCount(char *ip,char *comm,char *oid);
//__declspec(dllexport)
UINT AnyToStr(AsnObjectSyntax *sAny, char **pString);


#define	ERROR_MODEL				-1
#define	NO_COMPANY				-10
#define	NEED_DATA_LEXMARK		-100
#define	NEED_DATA_SINDORICOH	-200
#define	NEED_DATA_RICOH			-300
#define		LENGTHOFMODELNAME	1024

int main(int argc, char* argv[])
{
	int code=0;
	CHAR ModelName[LENGTHOFMODELNAME]={0};

	if(2 == argc) {
		GetModelbyIP(argv[1], ModelName);
		printf("model: %s\n=>", ModelName);
		printf("code: %d\n", GetModelCodebyIP(argv[1]));
	return TRUE;
	}
}

// 입력: IP주소
// 반환값: 코드
int GetModelCodebyIP(char *IPaddr) {
	CHAR ModelName[LENGTHOFMODELNAME]={0};
	PCHAR Token;
	PCHAR NextToken;

	// 모델을 가져온다.
	GetModelbyIP(IPaddr, ModelName);

	// 모델 DB
	Token = strtok_s(ModelName, " ", &NextToken);

	if(NULL != Token) {
		// 토큰이 있다면
		if(0 == strcmp("Lexmark", Token)) {
			Token = strtok_s(NULL, " ", &NextToken);
			if(0 == strcmp("T430", Token)) return 323201;
			else if(0 == strcmp("T642", Token)) return 450001;
			else return NEED_DATA_LEXMARK;
		}
		else if(0 == strcmp("SINDORICOH", Token)) {
			Token = strtok_s(NULL, " ", &NextToken);
			if(0 == strcmp("LP", Token)) {
				Token = strtok_s(NULL, " ", &NextToken);
				if(0 == strcmp("3532N/3522N", Token)) return 353201;
				else return NEED_DATA_SINDORICOH;
			}
			else return NEED_DATA_SINDORICOH;
		}
		else if(0 == strcmp("RICOH", Token)) {
			Token = strtok_s(NULL, " ", &NextToken);
			if(0 == strcmp("Aficio", Token)) {
				Token = strtok_s(NULL, " ", &NextToken);
				if(0 == strcmp("CL4000DN", Token)) return 400011;
				else return NEED_DATA_RICOH;
			}
			else return NEED_DATA_RICOH;
		}
		else return NO_COMPANY;
	}
	else return ERROR_MODEL;

}

int GetModelbyIP(char *ipAddr, char *ModelName)
{
	int status = -1;	// 상태를 담아 올곳

	return FnSindoricohStatus(ipAddr,"public",&status, ModelName);
}

// 에러코드: -100 ~ -104 (실패) / 1 (성공)
int FnSindoricohStatus(char *ip,char *comm,int *status, char *ModelName)
{
	int rtn;
	char returnvalue[LENGTHOFMODELNAME];
	OID_DATA oidData;
	int count, cnt, t;
	count = cnt = t = 0;	// 초기화

	//1st..hrPrinterStatus 체크...
	memset(returnvalue,0x00,sizeof(returnvalue));

	rtn = FnSnmpGETDATA(ip,comm,hrPrinterStatus,GET,&oidData);

	if (rtn < 0) {
//		printf("FnSnmpGETDATA error\n");
		*status=81;
		return -100;
	}
	strcpy_s(returnvalue,sizeof(returnvalue),oidData.oidvalue);
	if (strcmp(returnvalue,"4") ==0) {		//프린터 SPOOL JOB
		*status = 70;
		return -101;
	} else if (strcmp(returnvalue,"5") ==0) {	//프린터 SEND ERR
		*status = 71;
		return -102;
	} 
	//2st..prtAlertLocation 체크...
	count = FnWalkCount(ip,comm,prtAlertLocation);
	if (count < 0) {
		printf("FnWalkCount error\n");
		*status=81;
		return -103;
	} else if (count == 0) {
		*status =0;
		return -104;
	}

	memset(&oidData,0x00,sizeof(OID_DATA));
	strcpy_s(oidData.oidname,sizeof(oidData.oidname),prtAlertLocation);
	strcpy_s(oidData.oid,sizeof(oidData.oid),prtAlertLocation);

	for(cnt=0;cnt<count;cnt++)
	{

		t = FnSnmpGETDATA(ip,comm,oidData.oidname,GETNEXT,&oidData);
		memset(returnvalue,0x00,sizeof(returnvalue));
		strcpy_s(returnvalue,sizeof(returnvalue),oidData.oidvalue);
		wsprintf(ModelName, "%s", returnvalue);		// 프린터 모델 출력
	}
	return 1;
}

//__declspec(dllexport)
// 에러코드 : -10 ~ -15 / count (0)
int FnSnmpGETDATA(char *ip,char *comm,char *oid,int method,OID_DATA *poidData)
{
	INT                operation;
	LPSTR              agent;
	LPSTR              community;
	RFC1157VarBindList variableBindings;
	LPSNMP_MGR_SESSION session;

	INT        timeout = TIMEOUT;
	INT        retries = RETRIES;

	BYTE       requestType;
	AsnInteger errorStatus, errorIndex;

	AsnObjectIdentifier reqObject, root;

	char    *chkPtr = NULL;

	int count = 0;

	if  (method == 1) operation = GET;
	else if (method == 2) operation = GETNEXT;
	else if (method == 3) operation = WALK;
	else if (method == 4) operation = TRAP;

	//memset(&oidData,0x00,sizeof(OID_DATA));
	strcpy_s(poidData->ipaddress,sizeof(poidData->ipaddress),ip);
	strcpy_s(poidData->community,sizeof(poidData->community),comm);
	strcpy_s(poidData->oid,sizeof(poidData->oid),oid);

	agent = (LPSTR)SNMP_malloc((UINT)(strlen(poidData->ipaddress) + 1));
	strcpy_s(agent, strlen(poidData->ipaddress) + 1, poidData->ipaddress);

	community = (LPSTR)SNMP_malloc((UINT)(strlen(poidData->community) + 1));
	strcpy_s(community, strlen(poidData->community) + 1,poidData->community);

	// Get oid's...
	variableBindings.list = NULL;
	variableBindings.len = 0;

	// Convert the string representation to an internal representation.
	if (!SnmpMgrStrToOid(poidData->oid, &reqObject)) {
		//WriteLog("Error: Invalid oid, %s, specified.\n", oidData.oid);
		//memcpy(poidData, &oidData, sizeof(OID_DATA));
		return -10;
	} else {
		// Since sucessfull, add to the variable bindings list.
		variableBindings.len++;
		if ((variableBindings.list = (RFC1157VarBind *)SNMP_realloc(variableBindings.list, sizeof(RFC1157VarBind) *variableBindings.len)) == NULL) {
			//WriteLog("Error: Error allocating oid, %s.\n",oidData.oid);	
			//memcpy(poidData, &oidData, sizeof(OID_DATA));
			return -11;
		}
		variableBindings.list[variableBindings.len - 1].name = reqObject; // NOTE!  structure copy
		variableBindings.list[variableBindings.len - 1].value.asnType =  ASN_NULL;
	}

	// Establish a SNMP session to communicate with the remote agent.  The
	// community, communications timeout, and communications retry count
	// for the session are also required.
	if ((session = SnmpMgrOpen(agent, community, timeout, retries)) == NULL)
	{
//		printf("error on SnmpMgrOpen %d\n", GetLastError());

		return -12;
	}

	if (operation == GET) requestType = ASN_RFC1157_GETREQUEST;
	else requestType = ASN_RFC1157_GETNEXTREQUEST;

	// Request that the API carry out the desired operation.
	if (!SnmpMgrRequest(session, requestType, &variableBindings,
		&errorStatus, &errorIndex))
	{
		// The API is indicating an error.
//		printf("error on SnmpMgrRequest %d\n", GetLastError());
		return -13;
	}
	else
	{
		// The API succeeded, errors may be indicated from the remote
		// agent.
		if (errorStatus > 0)
		{
//			printf("Error: errorStatus=%d, errorIndex=%d\n",
//				errorStatus, errorIndex);
			return -14;
		}
		else
		{
			// Display the resulting variable bindings.
			UINT i;
			char *string = NULL;
			for(i=0; i < variableBindings.len; i++)
			{
				SnmpMgrOidToStr(&variableBindings.list[i].name, &string);
				strcpy_s(poidData->oidname, sizeof(poidData->oidname),string);

				if (string) SNMP_free(string);
				//SnmpUtilPrintAsnAny(&variableBindings.list[i].value);
				AnyToStr(&variableBindings.list[i].value,&string);	//실제 값만 반환
				strcpy_s(poidData->oidvalue,sizeof(poidData->oidvalue),string);
			} // end for()
		}
	}

	// Free the variable bindings that have been allocated.
	SnmpUtilVarBindListFree(&variableBindings);

	// Free the variable bindings that have been allocated.
	SnmpUtilVarBindListFree(&variableBindings);
	SnmpUtilOidFree(&root);

	// Close SNMP session with the remote agent.
	if (!SnmpMgrClose(session))
	{
//		printf("error on SnmpMgrClose %d\n", GetLastError());

		return -15;
	}
	return count;
} // end main()


//__declspec(dllexport)
// 에러코드: -1000 ~ -1005 / count
int FnWalkCount(char *ip,char *comm,char *oid)
{
	INT                operation;
	LPSTR              agent;
	LPSTR              community;
	RFC1157VarBindList variableBindings;
	LPSNMP_MGR_SESSION session;

	INT        timeout = TIMEOUT;
	INT        retries = RETRIES;

	BYTE       requestType;
	AsnInteger errorStatus, errorIndex;

	AsnObjectIdentifier reqObject, root, tempOid;
	OID_DATA oidData;

	char    *chkPtr = NULL;
	int count = 0;
	operation = WALK;

	memset(&oidData,0x00,sizeof(OID_DATA));

	strcpy_s(oidData.ipaddress,sizeof(oidData.ipaddress), ip);
	strcpy_s(oidData.community,sizeof(oidData.community), comm);
	strcpy_s(oidData.oid,sizeof(oidData.oid), oid);

	agent = (LPSTR)SNMP_malloc((UINT)(strlen(oidData.ipaddress) + 1));
	strcpy_s(agent, strlen(oidData.ipaddress) + 1, oidData.ipaddress);

	community = (LPSTR)SNMP_malloc((UINT)(strlen(oidData.community) + 1));
	strcpy_s(community, strlen(oidData.community) + 1,oidData.community);

	// Get oid's...
	variableBindings.list = NULL;
	variableBindings.len = 0;


	// Convert the string representation to an internal representation.
	if (!SnmpMgrStrToOid(oidData.oid, &reqObject)) {
		//WriteLog("Error: Invalid oid, %s, specified.\n", oidData.oid);

		//memcpy(poidData, &oidData, sizeof(OID_DATA));
		return -1000;
	} else {
		// Since sucessfull, add to the variable bindings list.
		variableBindings.len++;
		if ((variableBindings.list = (RFC1157VarBind *)SNMP_realloc(variableBindings.list, sizeof(RFC1157VarBind) *variableBindings.len)) == NULL) {
			//WriteLog("Error: Error allocating oid, %s.\n",oidData.oid);	

			//memcpy(poidData, &oidData, sizeof(OID_DATA));
			return -1001;
		}
		variableBindings.list[variableBindings.len - 1].name = reqObject; // NOTE!  structure copy
		variableBindings.list[variableBindings.len - 1].value.asnType =  ASN_NULL;
	}

	// Establish a SNMP session to communicate with the remote agent.  The
	// community, communications timeout, and communications retry count
	// for the session are also required.

	if (NULL == (session = SnmpMgrOpen(agent, community, timeout, retries)))
	{
//		printf("error on SnmpMgrOpen %d\n", GetLastError());

		return -1002;
	}

	// Walk is a common term used to indicate that all MIB variables
	// under a given OID are to be traversed and displayed.  This is
	// a more complex operation requiring tests and looping in addition
	// to the steps for get/getnext above.

	SnmpUtilOidCpy(&root, &variableBindings.list[0].name);
	requestType = ASN_RFC1157_GETNEXTREQUEST;
	while(1)
	{
		if (!SnmpMgrRequest(session, requestType, &variableBindings,
			&errorStatus, &errorIndex))
		{
			// The API is indicating an error.
//			printf("error on SnmpMgrRequest %d\n", GetLastError());
			return -1003;
			break;
		}
		else
		{
			// The API succeeded, errors may be indicated from the remote
			// agent.

			// Test for end of subtree or end of MIB.
			if (errorStatus == SNMP_ERRORSTATUS_NOSUCHNAME ||
				SnmpUtilOidNCmp(&variableBindings.list[0].name,
				&root, root.idLength))
			{
				break;
			}

			// Test for general error conditions or sucesss.
			if (errorStatus > 0)
			{
//				printf("Error: errorStatus=%d, errorIndex=%d \n",
//					errorStatus, errorIndex);
				return -1004;
			}
			else
			{
				// Display resulting variable binding for this iteration.
				char *string = NULL;

				SnmpMgrOidToStr(&variableBindings.list[0].name, &string);
				if (string) SNMP_free(string);
				//SnmpUtilPrintAsnAny(&variableBindings.list[0].value);
				count++;		//Walk 하위갯수증가
			}
		} // end if()

		// Prepare for the next iteration.  Make sure returned oid is
		// preserved and the returned value is freed.
		SnmpUtilOidCpy(&tempOid, &variableBindings.list[0].name);
		SnmpUtilVarBindFree(&variableBindings.list[0]);
		SnmpUtilOidCpy(&variableBindings.list[0].name, &tempOid);
		variableBindings.list[0].value.asnType = ASN_NULL;
		SnmpUtilOidFree(&tempOid);
	} // end while()

	// Free the variable bindings that have been allocated.
	SnmpUtilVarBindListFree(&variableBindings);
	SnmpUtilOidFree(&root);

	// Close SNMP session with the remote agent.
	if (!SnmpMgrClose(session))
	{
//		printf("error on SnmpMgrClose %d\n", GetLastError());
		return -1005;
	}
	return count;
} // end main()


// SNMP GET,GETNEXT 의 결과 중 실제 데이터만 반환
UINT AnyToStr(AsnObjectSyntax *sAny, char **pString)
{
	DWORD dwValue = 0;
	UINT uLen = 0;
	BYTE *puData = 0;

	switch ( sAny->asnType ) {
	  case ASN_INTEGER:    
		  *pString = (char *) SnmpUtilMemAlloc( 33 );
		  if (*pString ) {
			  wsprintf(*pString,"%d",sAny->asnValue.number);
		  }
		  break;
	  case ASN_RFC1155_COUNTER:
		  dwValue = sAny->asnValue.counter;
		  goto Uint;
	  case ASN_RFC1155_GAUGE:
		  dwValue = sAny->asnValue.gauge;
		  goto Uint;
	  case ASN_RFC1155_TIMETICKS:
		  dwValue = sAny->asnValue.ticks;
Uint:
		  *pString = (char *) SnmpUtilMemAlloc( 33 );
		  if ( *pString ) {
			  wsprintf(*pString, "%d", dwValue);
		  }
		  break;
	  case ASN_OCTETSTRING:   /* Same as ASN_RFC1213_DISPSTRING */
		  uLen = sAny->asnValue.string.length;
		  puData = sAny->asnValue.string.stream;
		  goto String;
	  case ASN_SEQUENCE:      /* Same as ASN_SEQUENCEOF */
		  uLen = sAny->asnValue.sequence.length;
		  puData = sAny->asnValue.sequence.stream;
		  goto String;
	  case ASN_RFC1155_IPADDRESS:
		  uLen = sAny->asnValue.address.length;
		  puData = sAny->asnValue.address.stream;
		  goto String;
	  case ASN_RFC1155_OPAQUE:
		  uLen = sAny->asnValue.arbitrary.length;
		  puData = sAny->asnValue.arbitrary.stream;
String:
		  *pString = (char *) SnmpUtilMemAlloc( uLen + 1 );
		  if ( *pString ) {
			  memcpy( *pString, (const char *)puData, uLen );
		  }
		  return uLen;
		  //break;
	  case ASN_OBJECTIDENTIFIER: {
		  *pString = (char *) SnmpUtilMemAlloc( sAny->asnValue.object.idLength * 5 );
		  if ( *pString ) {
			  UINT i;
			  char szBuf[17];
			  for ( i = 0; i < sAny->asnValue.object.idLength; i++ ) {
				  wsprintf(szBuf,"%d",sAny->asnValue.object.ids[i]);
				  lstrcat(*pString, szBuf);
				  lstrcat(*pString, "." );
			  }
		  }
		  break;
								 }
	  default:             /* Unrecognised data type */
		  return( FALSE );
	}
	return( *pString ? TRUE : FALSE );
}
