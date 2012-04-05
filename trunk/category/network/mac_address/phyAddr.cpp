#include "phyAddr.h"

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Iphlpapi.h>	// Requirements: Windows 2000 Professional
#include <StrSafe.h>

#pragma comment(lib, "Iphlpapi.lib")

#define NO_INTERFACE (-1)

namespace Network
{
	const size_t buf_size = 5;

	// 함수 전방 선언
	DWORD GetBestInterfaceIdx();
	BOOL SetMacAddressByIndex(const DWORD dwIf, std::vector<BYTE>& out);

	// 외부 노출된 함수 구현
	bool GetBestInterfaceAddress(std::vector<BYTE>& out)
	{
		DWORD dwIf = Network::GetBestInterfaceIdx();
		if(NO_INTERFACE==dwIf) {
			return false;
		}

		if(Network::SetMacAddressByIndex(dwIf, out)) {
			return true;
		} else {
			return false;
		}
	}


	// 전방 선언된 함수 구현
	DWORD GetBestInterfaceIdx()
	{
		IPAddr in;
		DWORD dwIf;
		ZeroMemory(&in, sizeof(IPAddr));	// IPv4 : 0.0.0.0

		// GetBestInterface function
		// http://msdn.microsoft.com/en-us/library/windows/desktop/aa365920(v=vs.85).aspx
		DWORD dw = ::GetBestInterface(in, &dwIf);

		return ( (NO_ERROR==dw) ? dwIf : NO_INTERFACE);
	}

	BOOL SetMacAddressByIndex(const DWORD dwIf, std::vector<BYTE>& out)
	{
		// GetAdaptersInfo function
		// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/aa365917(v=vs.85).aspx

		// 동적 할당 크기 구함(1차 시도)
		ULONG ulOutBufLen = 0;	// ERROR_BUFFER_OVERFLOW 유도
		if (::GetAdaptersInfo(NULL, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			PIP_ADAPTER_INFO pAdapterInfo;

			std::vector<BYTE> alloc(ulOutBufLen);
			pAdapterInfo = (PIP_ADAPTER_INFO)&alloc[0];

			// 실제 값을 받아옴(2차 시도)
			if (::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_SUCCESS) {
				PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
				while (pAdapter) {
					if(dwIf == (pAdapter->Index)) {	// interface index matched
						for (UINT i=0; i<pAdapter->AddressLength; ++i) {
							out.push_back(pAdapter->Address[i]);
						}
						return TRUE;
					}
					pAdapter = pAdapter->Next;
				} // while (pAdapter)
			}
		}
		return FALSE;
	}

} // end of namespace Network
