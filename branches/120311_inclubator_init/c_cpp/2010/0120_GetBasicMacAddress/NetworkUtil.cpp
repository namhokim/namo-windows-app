#include "NetworkUtil.h"

#define WIN32_LEAN_AND_MEAN	// exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets
#include <Windows.h>
#include <iphlpapi.h>	// for IP Helper Functions
#include <vector>		// ���� �޸� �Ҵ�� �ڵ� ������ �̿��ϱ� ����
#pragma comment(lib, "iphlpapi.lib")

// Function Prototype(for reference of internal functions)
int GetDefaultRouteIf();
std::string GetAdapterAddrByIdx(const int idx = NO_INTERFACE, const char* deliminator = "-");

////////////////////////////////////////////////////////////////////////////////////////////////////
// Exposed Functions
/**
 * ���: Default Gateway�� ������ ��Ʈ��ũ �������̽� MAC �ּҸ� ���Ѵ�
 * ��ȯ: 00-00-00-00-00-00 Ÿ���� ���ּ�(���� ��� : �� ���ڿ�)
 */
std::string GetBasicMacAddress(const char* deliminator)
{
	int ifIdx = GetDefaultRouteIf();
	return GetAdapterAddrByIdx(ifIdx, deliminator);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Intenal Functions

/**
 * ���: Default Gateway�� ������ ��Ʈ��ũ �������̽� �ε����� ���Ѵ�
 * ��ȯ: �������̽� �ε���(���� ��� : NO_INTERFACE ��ȯ)
 */
int GetDefaultRouteIf()
{
	int ifIdx = NO_INTERFACE;

	MIB_IPFORWARDTABLE ipForwardTable[sizeof (MIB_IPFORWARDTABLE)];	// ������ ���� ���� �迭�� ����
	PMIB_IPFORWARDTABLE pIpForwardTable = ipForwardTable;

	DWORD dwSize = 0;	// ERROR_INSUFFICIENT_BUFFER ����
	DWORD dwRetVal = 0;

	// ���� �Ҵ� ũ�� ����(1�� �õ�)
	if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == ERROR_INSUFFICIENT_BUFFER) {
		std::vector<BYTE> alloc(dwSize);
		pIpForwardTable = (PMIB_IPFORWARDTABLE)&alloc[0];

		// ���� ���� �޾ƿ�(2�� �õ�)
		if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == NO_ERROR) {

			for (int i = 0; i < (int) pIpForwardTable->dwNumEntries; i++) {
				if(0x00000000 == pIpForwardTable->table[i].dwForwardDest) {	// Destination�� 0.0.0.0 �ּ�(Default gate) �϶�
					ifIdx = pIpForwardTable->table[i].dwForwardIfIndex;
					break;
				}
			}
		}
	}

	return ifIdx;
}

/**
 * ���: �������̽� �ε����� �ش��ϴ� MAC �ּҸ� ���Ѵ�
 * ��ȯ: 00-00-00-00-00-00 Ÿ���� ���ּ�(���� ��� : �� ���ڿ�)
 * ��Ÿ: ���� ������ NO_INTERFACE�� ��(�Լ� ���� ����)
 */
std::string GetAdapterAddrByIdx(const int idx, const char* deliminator)
{
	char mac[sizeof("00-00-00-00-00-00+")];
	::memset(mac, 0, sizeof(mac));

	IP_ADAPTER_INFO adapterInfo[sizeof (IP_ADAPTER_INFO)];	// ������ ���� ���� �迭�� ����
	PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
	ULONG ulOutBufLen = 0;	// ERROR_BUFFER_OVERFLOW ����
	DWORD dwRetVal = 0;

	// ���� �Ҵ� ũ�� ����(1�� �õ�)
	if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == ERROR_BUFFER_OVERFLOW) {
		std::vector<BYTE> alloc(ulOutBufLen);
		pAdapterInfo = (PIP_ADAPTER_INFO)&alloc[0];

		// ���� ���� �޾ƿ�(2�� �õ�)
		if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
			PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
			while (pAdapter) {
				if(((idx == pAdapter->Index) || (NO_INTERFACE==idx))	// INTERFACE index�� ���� ���� ù ��° �ε����� ����
					&& (6 == pAdapter->AddressLength) && (MIB_IF_TYPE_ETHERNET == pAdapter->Type)) {
					sprintf_s(mac, sizeof("00-00-00-00-00-00"), "%.2X%s%.2X%s%.2X%s%.2X%s%.2X%s%.2X", 
						(int) pAdapter->Address[0], deliminator,
						(int) pAdapter->Address[1], deliminator,
						(int) pAdapter->Address[2], deliminator,
						(int) pAdapter->Address[3], deliminator,
						(int) pAdapter->Address[4], deliminator,
						(int) pAdapter->Address[5]);
					break;
				}
				pAdapter = pAdapter->Next;
			}
		}
	}

	return mac;
}



