#include <MgmtAPI.h>	// MGMT API
#include <cstdio>
#include <string>
#pragma comment(lib, "MgmtAPI.Lib")		// for SNMP Management API {SnmpMgrOpen, SnmpMgrClose}
#pragma comment(lib, "SnmpAPI.Lib")		// for SNMP Utility API

int CmpOid(std::string oid1, std::string oid2);

int main()
{
	DWORD d = 1;
	while(1) {
		printf("%d> %d\n", d, CmpOid("1.3.6.1.4.1.367.3.2.1.7.3.2.1.0", "1.3.6.1.4.1.367.32.2.1.7.3.2.2.0"));
		if(d%10000 == 0) Sleep(5000);
		if(d%100000 == 0) break;
		d++;
	}
	getchar();
	return 0;
}

int CmpOid(std::string oid1, std::string oid2)
{
	if(oid1.empty() || oid2.empty()) return 0;	// exception

	if(oid1.at(0)!='.') oid1 = "." + oid1;
	if(oid2.at(0)!='.') oid2 = "." + oid2;

	AsnObjectIdentifier obj1, obj2;
	memset(&obj1, 0, sizeof(AsnObjectIdentifier));
	memset(&obj2, 0, sizeof(AsnObjectIdentifier));

	BOOL res1 = ::SnmpMgrStrToOid(const_cast<LPSTR>(oid1.c_str()), &obj1);
	BOOL res2 = ::SnmpMgrStrToOid(const_cast<LPSTR>(oid2.c_str()), &obj2);
	if(res1==0 || res2==0) return 0;	// exception

	INT diff =  ::SnmpUtilOidCmp(&obj1, &obj2);

	/*INT diff = 0;
	for(UINT i=0; (i<obj1.idLength) && (i<obj2.idLength); i++) {
		diff = obj1.ids[i] - obj2.ids[i];
		if(diff != 0) break;
	}*/
	if(res1) ::SnmpUtilOidFree(&obj1);
	if(res2) ::SnmpUtilOidFree(&obj2);
	return diff;
}
