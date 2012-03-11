#include "uuidgen.h"
#include <iphlpapi.h>	// for IP Helper Functions
#include <vector>		// 동적 메모리 할당시 자동 해제를 이용하기 위해

#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "iphlpapi.lib")

#define NO_INTERFACE	-1

///////////////////////////////////////////////////////////////////////////////
// 함수 전방 선언
int GetDefaultRouteIf();
DWORD GetAdapterAddrByIdx(const int idx, unsigned char* mac);

///////////////////////////////////////////////////////////////////////////////
// UUID
DWORD UuidGenWithMac(__out UUID __RPC_FAR * Uuid)
{
	// 현재 MAC 주소에 기반한 UUID를 생성
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
	if(NO_INTERFACE!=ifIdx) // NIC가 존재한다면
	{
		// 디폴드 게이트웨이로 지정된 NIC의 MAC주소를
		// UUID의 제일 뒤의 12자리로 지정
		DWORD dwIfRes = GetAdapterAddrByIdx(ifIdx, &(Uuid->Data4[2]));

		// 최초 UUID 생성이 문제가 없었더라면,
		// GetAdapterAddrByIdx() 호출의 반환값을 리턴
		if(RPC_S_OK!=lStatus) return dwIfRes;
	}
	return lStatus;
}

///////////////////////////////////////////////////////////////////////////////
// Network Interface card

/**
* 요약: Default Gateway로 나가는 네트워크 인터페이스 인덱스를 구한다
* 반환: 인터페이스 인덱스(없을 경우 : NO_INTERFACE 반환)
*/
int GetDefaultRouteIf()
{
	static const DWORD DefaultGateway = 0x00000000L;	// 주소 : 0.0.0.0

	int ifIdx = NO_INTERFACE;

	MIB_IPFORWARDTABLE ipForwardTable[sizeof (MIB_IPFORWARDTABLE)];	// 성능을 위해 정적 배열로 구현
	PMIB_IPFORWARDTABLE pIpForwardTable = ipForwardTable;

	DWORD dwSize = 0;	// ERROR_INSUFFICIENT_BUFFER 유도
	DWORD dwRetVal = 0;

	// 동적 할당 크기 구함(1차 시도)
	if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == ERROR_INSUFFICIENT_BUFFER)
	{
		std::vector<BYTE> alloc(dwSize);
		pIpForwardTable = (PMIB_IPFORWARDTABLE)&alloc[0];

		// 실제 값을 받아옴(2차 시도)
		if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == NO_ERROR)
		{
			for (int i = 0; i < (int) pIpForwardTable->dwNumEntries; i++)
			{
				if(DefaultGateway == pIpForwardTable->table[i].dwForwardDest)	// Destination가 Default gate 일때
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
* 요약: 인터페이스 인덱스에 해당하는 MAC 주소를 구한다
* 반환: 00-00-00-00-00-00 타입의 맥주소(없을 경우 : 빈 문자열)
* 기타: 인자 생략시 NO_INTERFACE로 들어감(함수 선언 참고)
* XP 이후의 OS의 경우는 GetAdaptersAddresses를 권장하나 Windows 2000 버전을 지원하기 위해
	GetAdaptersInfo() 함수를 사용함(OS에 따라 바뀌는 버전을 만들 필요성 있음)
*/
DWORD GetAdapterAddrByIdx(const int idx, unsigned char* mac)
{
	static const int SizeOfMacAddr = 6;	// 섹션의 수 : 00-00-00-00-00-00

	IP_ADAPTER_INFO adapterInfo[sizeof (IP_ADAPTER_INFO)];	// 성능을 위해 정적 배열로 구현
	PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
	ULONG ulOutBufLen = 0;	// ERROR_BUFFER_OVERFLOW 유도
	DWORD dwRetVal = 0;

	// 동적 할당 크기 구함(1차 시도)
	if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == ERROR_BUFFER_OVERFLOW)
	{
		std::vector<BYTE> alloc(ulOutBufLen);
		pAdapterInfo = (PIP_ADAPTER_INFO)&alloc[0];

		// 실제 값을 받아옴(2차 시도)
		if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
		{
			PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
			while (pAdapter)
			{
				if ( ((idx == pAdapter->Index) || (NO_INTERFACE==idx))	// INTERFACE index가 없을 경우는 첫 번째 인덱스로 설정
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