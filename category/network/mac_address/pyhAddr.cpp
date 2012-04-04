#include "pyhAddr.h"

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Iphlpapi.h>	// Requirements: Windows 2000 Professional
#include <StrSafe.h>

#pragma comment(lib, "Iphlpapi.lib")

#define NO_INTERFACE (-1)

namespace Network
{
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

	// TESTED
	// Microsoft Windows 2000 [Version 5.00.2195]
	// Microsoft Windows XP [Version 5.1.2600] ; Professional K SP3 (32bit)
	// Microsoft Windows [Version 5.2.3790] ; Windows Server 2003 R2 Standard Edition SP2 (32bit)
	// Microsoft Windows [Version 6.1.7601] ; Windows Server 2008 R2 Standard SP1 (64bit)
	// Microsoft Windows [Version 6.1.7601] ; Windows 7 Professional K SP1 (64bit)
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

		/*if(isSuccess) {
			char buf[5];
			std::vector<BYTE>::size_type pos;
			for (pos=0; pos<mac.size(); ++pos) {
				BYTE b = mac[pos];
				if (pos == (mac.size() - 1)) {
					sprintf(buf, "%.2X", (int) b );
					out.append(buf);
				}
				else {
					sprintf(buf, "%.2X-", (int) b );
					out.append(buf);
				}
			}
			return true;
		} else {
			return false;
		}*/
	}

	bool GetBasicMacAddress(std::string& out)
	{
		std::vector<BYTE> mac;
		if(GetBestInterfaceAddress(mac)) {
			char buf[5];
			std::vector<BYTE>::size_type pos, size=mac.size();
			for (pos=0; pos<size; ++pos) {
				BYTE b = mac[pos];
				if (pos == (size - 1)) {
					StringCchPrintfA(buf, 5, "%.2X", (int) b );
					out.append(buf);
				}
				else {
					StringCchPrintfA(buf, 5, "%.2X-", (int) b );
					out.append(buf);
				}
			}
			return true;
		} else {
			return false;
		}
	}

	bool GetBasicMacAddress(std::wstring& out)
	{
		std::vector<BYTE> mac;
		if(GetBestInterfaceAddress(mac)) {
			wchar_t buf[5];
			std::vector<BYTE>::size_type pos, size=mac.size();
			for (pos=0; pos<size; ++pos) {
				BYTE b = mac[pos];
				if (pos == (size - 1)) {
					StringCchPrintfW(buf, 5, L"%.2X", (int) b );
					out.append(buf);
				}
				else {
					StringCchPrintfW(buf, 5, L"%.2X-", (int) b );
					out.append(buf);
				}
			}
			return true;
		} else {
			return false;
		}
	}

} // end of namespace Network
