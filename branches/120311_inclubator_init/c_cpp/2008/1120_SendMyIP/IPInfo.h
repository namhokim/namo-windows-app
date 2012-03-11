#pragma once
#include "unicode.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class IPInfo
{
public:
	IPInfo(void)
		:m_bInit(false)
	{
		Init();
	}

	~IPInfo(void)
	{
		Uninit();
	}
	_STRING getMyIP();
private:
	bool m_bInit;
	SOCKET m_socket;
	bool Init() throw()
	{
		if (m_bInit)
			return true;

		WSADATA wsadata;
		if (WSAStartup(MAKEWORD(2, 2), &wsadata)) return false;
		if((m_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0)) == INVALID_SOCKET)
		{
			m_bInit = false;
		}
		m_bInit = true;
		return true;
	}

	bool Uninit() throw()
	{
		if (m_bInit)
			if (WSACleanup()) return false;
		m_bInit = false;
		return true;
	}
};
