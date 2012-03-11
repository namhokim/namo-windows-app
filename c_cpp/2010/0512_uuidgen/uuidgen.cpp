#include "uuidgen.h"
#include <iphlpapi.h>	// for IP Helper Functions
#include <vector>		// ���� �޸� �Ҵ�� �ڵ� ������ �̿��ϱ� ����

#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "iphlpapi.lib")

#define NO_INTERFACE	-1

///////////////////////////////////////////////////////////////////////////////
// �Լ� ���� ����
int GetDefaultRouteIf();
DWORD GetAdapterAddrByIdx(const int idx, unsigned char* mac);

///////////////////////////////////////////////////////////////////////////////
// UUID
DWORD UuidGenWithMac(__out UUID __RPC_FAR * Uuid)
{
	// ���� MAC �ּҿ� ����� UUID�� ����
	RPC_STATUS lStatus = UuidCreateSequential(Uuid);
	switch(lStatus)
	{
	case RPC_S_OK:	// -> ERROR_SUCCESS -> 0L
		break;
	case RPC_S_UUID_LOCAL_ONLY:	// -> 1824L
		break;
	case RPC_S_UUID_NO_ADDRESS:	// -> 1739L
		break;
	}
	//Uuid->Data4
	int ifIdx = GetDefaultRouteIf();
	if(NO_INTERFACE!=ifIdx) // NIC�� �����Ѵٸ�
	{
		// ������ ����Ʈ���̷� ������ NIC�� MAC�ּҸ�
		// UUID�� ���� ���� 12�ڸ��� ����
		DWORD dwIfRes = GetAdapterAddrByIdx(ifIdx, &(Uuid->Data4[2]));

		// ���� UUID ������ ������ ���������,
		// GetAdapterAddrByIdx() ȣ���� ��ȯ���� ����
		if(RPC_S_OK!=lStatus) return dwIfRes;
	}
	return lStatus;
}

///////////////////////////////////////////////////////////////////////////////
// Network Interface card

/**
* ���: Default Gateway�� ������ ��Ʈ��ũ �������̽� �ε����� ���Ѵ�
* ��ȯ: �������̽� �ε���(���� ��� : NO_INTERFACE ��ȯ)
*/
int GetDefaultRouteIf()
{
	static const DWORD DefaultGateway = 0x00000000L;	// �ּ� : 0.0.0.0

	int ifIdx = NO_INTERFACE;

	MIB_IPFORWARDTABLE ipForwardTable[sizeof (MIB_IPFORWARDTABLE)];	// ������ ���� ���� �迭�� ����
	PMIB_IPFORWARDTABLE pIpForwardTable = ipForwardTable;

	DWORD dwSize = 0;	// ERROR_INSUFFICIENT_BUFFER ����
	DWORD dwRetVal = 0;

	// ���� �Ҵ� ũ�� ����(1�� �õ�)
	if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == ERROR_INSUFFICIENT_BUFFER)
	{
		std::vector<BYTE> alloc(dwSize);
		pIpForwardTable = (PMIB_IPFORWARDTABLE)&alloc[0];

		// ���� ���� �޾ƿ�(2�� �õ�)
		if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == NO_ERROR)
		{
			for (int i = 0; i < (int) pIpForwardTable->dwNumEntries; i++)
			{
				if(DefaultGateway == pIpForwardTable->table[i].dwForwardDest)	// Destination�� Default gate �϶�
				{
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
* XP ������ OS�� ���� GetAdaptersAddresses�� �����ϳ� Windows 2000 ������ �����ϱ� ����
	GetAdaptersInfo() �Լ��� �����(OS�� ���� �ٲ�� ������ ���� �ʿ伺 ����)
*/
DWORD GetAdapterAddrByIdx(const int idx, unsigned char* mac)
{
	static const int SizeOfMacAddr = 6;	// ������ �� : 00-00-00-00-00-00

	IP_ADAPTER_INFO adapterInfo[sizeof (IP_ADAPTER_INFO)];	// ������ ���� ���� �迭�� ����
	PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
	ULONG ulOutBufLen = 0;	// ERROR_BUFFER_OVERFLOW ����
	DWORD dwRetVal = 0;

	// ���� �Ҵ� ũ�� ����(1�� �õ�)
	if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == ERROR_BUFFER_OVERFLOW)
	{
		std::vector<BYTE> alloc(ulOutBufLen);
		pAdapterInfo = (PIP_ADAPTER_INFO)&alloc[0];

		// ���� ���� �޾ƿ�(2�� �õ�)
		if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
		{
			PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
			while (pAdapter)
			{
				if ( ((idx == pAdapter->Index) || (NO_INTERFACE==idx))	// INTERFACE index�� ���� ���� ù ��° �ε����� ����
					&& (SizeOfMacAddr == pAdapter->AddressLength)
					&& (MIB_IF_TYPE_ETHERNET == pAdapter->Type)
					)
				{
					for(int i=0; i<SizeOfMacAddr; ++i) {
						mac[i] = pAdapter->Address[i];
					}
					break;
				}
				pAdapter = pAdapter->Next;
			}
		}
	}

	return dwRetVal;
}