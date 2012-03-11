#include "phyAddr.h"
#include <winsock2.h>
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

namespace Network
{
	std::tstring GetAdapterAddr(const PIP_ADAPTER_INFO pAdapter);

	PhyAddrVec GetMyPhyAddrs(Adapters type)
	{
		PhyAddrVec r;

		PIP_ADAPTER_INFO pAdapterInfo;
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
		if (pAdapterInfo == NULL) return r;	// Memmory alloc error

		// Make an initial call to GetAdaptersInfo to get
		// the necessary size into the ulOutBufLen variable
		ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
		if (::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			free(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
			if (pAdapterInfo == NULL) return r;	// Memmory alloc error
		}

		DWORD dwRetVal = 0;
		if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
			PIP_ADAPTER_INFO pAdapter = NULL;
			pAdapter = pAdapterInfo;
			while (pAdapter) {
				if(pAdapter->Type == type) {
					r.push_back(GetAdapterAddr(pAdapter));
				}
				pAdapter = pAdapter->Next;
			}
		}

		if (pAdapterInfo) free(pAdapterInfo);

		return r;
	}

	std::tstring GetAdapterAddr(const PIP_ADAPTER_INFO pAdapter)
	{
		std::tstring r;
		TCHAR buf[5];
		for (UINT i = 0; i < (pAdapter->AddressLength); i++) {
			if (i == (pAdapter->AddressLength - 1)) {
				wsprintf(buf, TEXT("%.2X"), (int) pAdapter->Address[i]);
				r.append(buf);
			}
			else {
				wsprintf(buf, TEXT("%.2X-"), (int) pAdapter->Address[i]);
				r.append(buf);
			}
		}
		return r;
	}

} // end of namespace Network