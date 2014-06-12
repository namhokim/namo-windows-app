#include <stdio.h>
#define	WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

#include <vector>
using namespace std;

void PrintResult(PMIB_IPNETTABLE pIpNetTable);
void PrintIpRow(PMIB_IPNETROW_LH pIpNetRow);

int main()
{
	MIB_IPNETTABLE table;
	ULONG dwSize = sizeof(table);
	BOOL bSorted = TRUE;

	// TRY FIRST (size: 28)
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa365956(v=vs.85).aspx
	DWORD dw = GetIpNetTable(&table, &dwSize, bSorted);
	if (ERROR_INSUFFICIENT_BUFFER==dw) {
		vector<BYTE> buf(dwSize);
		PMIB_IPNETTABLE pIpNetTable = (PMIB_IPNETTABLE)&buf[0];

		dw = GetIpNetTable(pIpNetTable, &dwSize, bSorted);
		if (NO_ERROR==dw) {
			PrintResult(pIpNetTable);
		}
	}

	return 0;
}

void PrintResult(PMIB_IPNETTABLE pIpNetTable)
{
	printf("Number of entries : %d\n", pIpNetTable->dwNumEntries);
	for (DWORD i = 0; i<pIpNetTable->dwNumEntries; i++)
	{
		MIB_IPNETROW &row = pIpNetTable->table[i];
		PrintIpRow(&row);
	}
}

const char* GetType(MIB_IPNET_TYPE type)
{
	switch(type)
	{
	case MIB_IPNET_TYPE_OTHER:
		return "other";
	case MIB_IPNET_TYPE_INVALID:
		return "invalid";
	case MIB_IPNET_TYPE_DYNAMIC:
		return "dynamic";
	case MIB_IPNET_TYPE_STATIC:
		return "static";
	default:
		return "unknown";
	}
}
void PrintIpRow(PMIB_IPNETROW_LH pIpNetRow)
{
	if (MIB_IPNET_TYPE_DYNAMIC!=pIpNetRow->Type) return;

	// 인터페이스 인덱스
	printf("0x%x\t", pIpNetRow->dwIndex);

	// 인터넷 주소
	IN_ADDR IPAddr;
	IPAddr.S_un.S_addr = (ULONG) pIpNetRow->dwAddr;
	printf("%s\t", inet_ntoa(IPAddr));

	// 물리적 주소
	for (DWORD i = 0; i < pIpNetRow->dwPhysAddrLen; i++) {
		if (i == (pIpNetRow->dwPhysAddrLen - 1)) {
			printf("%.2x\t", (int) pIpNetRow->bPhysAddr[i]);
		} else {
			printf("%.2x-", (int) pIpNetRow->bPhysAddr[i]);
		}
	}

	// 유형
	printf("%s\n", GetType(pIpNetRow->Type));
}