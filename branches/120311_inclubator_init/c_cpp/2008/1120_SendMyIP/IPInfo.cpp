#include "IPInfo.h"
#include <ws2tcpip.h>	// for WSAIoctl :: SIO_GET_INTERFACE_LIST
#include "StringUtil.h"

const int MaxNICNumber = 10;
_STRING IPInfo::getMyIP()
{
	if(m_bInit == false) return _T("Socket Error:");
	
	int wsError;
	INTERFACE_INFO localAddr[MaxNICNumber];
	DWORD bytesReturned;
	wsError = WSAIoctl(m_socket, SIO_GET_INTERFACE_LIST, NULL, 0, &localAddr,
		sizeof(localAddr), &bytesReturned, NULL, NULL);

	int numLocalAddr;
	numLocalAddr = (bytesReturned/sizeof(INTERFACE_INFO));
	SOCKADDR_IN* pAddrInet;
	IN_ADDR IP, SubnetMask;			// ip와, 서브넷마스크 정의
	std::string IPInfoStringA("");
	for (int i=0; i<numLocalAddr; i++) {
		if (!(localAddr[i].iiFlags & IFF_LOOPBACK)) {

			pAddrInet = (SOCKADDR_IN*)&localAddr[i].iiAddress;	// ip
			IP = pAddrInet->sin_addr;
			IPInfoStringA.append("IP Address: ");
			IPInfoStringA.append(inet_ntoa(IP));
			IPInfoStringA.append("\n");
			pAddrInet = (SOCKADDR_IN*)&localAddr[i].iiNetmask;	// subnet mask
			SubnetMask = pAddrInet->sin_addr;
			IPInfoStringA.append("Subnet Mask: ");
			IPInfoStringA.append(inet_ntoa(SubnetMask));
			IPInfoStringA.append("\n\n");
		}
	}
	_STRING IPInfoStringT;
#ifdef _UNICODE
		IPInfoStringT = mbs_to_wcs(IPInfoStringA);
#else
		IPInfoStringT = IPInfoStringA;
#endif
	//MessageBoxA(NULL, IPInfoString.c_str(), "알림", MB_OK);

	return IPInfoStringT;
}