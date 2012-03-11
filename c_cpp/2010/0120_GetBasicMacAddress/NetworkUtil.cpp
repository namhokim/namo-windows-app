#include "NetworkUtil.h"

#define WIN32_LEAN_AND_MEAN	// exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets
#include <Windows.h>
#include <iphlpapi.h>	// for IP Helper Functions
#include <vector>		// 동적 메모리 할당시 자동 해제를 이용하기 위해
#pragma comment(lib, "iphlpapi.lib")

// Function Prototype(for reference of internal functions)
int GetDefaultRouteIf();
std::string GetAdapterAddrByIdx(const int idx = NO_INTERFACE, const char* deliminator = "-");

////////////////////////////////////////////////////////////////////////////////////////////////////
// Exposed Functions
/**
 * 요약: Default Gateway로 나가는 네트워크 인터페이스 MAC 주소를 구한다
 * 반환: 00-00-00-00-00-00 타입의 맥주소(없을 경우 : 빈 문자열)
 */
std::string GetBasicMacAddress(const char* deliminator)
{
	int ifIdx = GetDefaultRouteIf();
	return GetAdapterAddrByIdx(ifIdx, deliminator);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Intenal Functions

/**
 * 요약: Default Gateway로 나가는 네트워크 인터페이스 인덱스를 구한다
 * 반환: 인터페이스 인덱스(없을 경우 : NO_INTERFACE 반환)
 */
int GetDefaultRouteIf()
{
	int ifIdx = NO_INTERFACE;

	MIB_IPFORWARDTABLE ipForwardTable[sizeof (MIB_IPFORWARDTABLE)];	// 성능을 위해 정적 배열로 구현
	PMIB_IPFORWARDTABLE pIpForwardTable = ipForwardTable;

	DWORD dwSize = 0;	// ERROR_INSUFFICIENT_BUFFER 유도
	DWORD dwRetVal = 0;

	// 동적 할당 크기 구함(1차 시도)
	if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == ERROR_INSUFFICIENT_BUFFER) {
		std::vector<BYTE> alloc(dwSize);
		pIpForwardTable = (PMIB_IPFORWARDTABLE)&alloc[0];

		// 실제 값을 받아옴(2차 시도)
		if ((dwRetVal = ::GetIpForwardTable(pIpForwardTable, &dwSize, 0)) == NO_ERROR) {

			for (int i = 0; i < (int) pIpForwardTable->dwNumEntries; i++) {
				if(0x00000000 == pIpForwardTable->table[i].dwForwardDest) {	// Destination가 0.0.0.0 주소(Default gate) 일때
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
 */
std::string GetAdapterAddrByIdx(const int idx, const char* deliminator)
{
	char mac[sizeof("00-00-00-00-00-00+")];
	::memset(mac, 0, sizeof(mac));

	IP_ADAPTER_INFO adapterInfo[sizeof (IP_ADAPTER_INFO)];	// 성능을 위해 정적 배열로 구현
	PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
	ULONG ulOutBufLen = 0;	// ERROR_BUFFER_OVERFLOW 유도
	DWORD dwRetVal = 0;

	// 동적 할당 크기 구함(1차 시도)
	if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == ERROR_BUFFER_OVERFLOW) {
		std::vector<BYTE> alloc(ulOutBufLen);
		pAdapterInfo = (PIP_ADAPTER_INFO)&alloc[0];

		// 실제 값을 받아옴(2차 시도)
		if ((dwRetVal = ::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
			PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
			while (pAdapter) {
				if(((idx == pAdapter->Index) || (NO_INTERFACE==idx))	// INTERFACE index가 없을 경우는 첫 번째 인덱스로 설정
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



