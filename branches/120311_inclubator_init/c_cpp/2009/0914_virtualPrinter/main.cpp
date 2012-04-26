#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <string>

#include ".\3rd\GetLastError.h"

int BindSocket(SOCKET sd, std::string addrIP, int nPort);
void ProcessRequest(SOCKET msgSock);

int __cdecl main()
{
	const int StdTCPIP_PRT = 9100;
	const std::string IP_ADDR = "10.15.30.106";

	WSADATA wsaData;
	// ���� ���̺귯���� �ʱ�ȭ
	if (::WSAStartup(0x202, &wsaData) != 0) {
		printf("WSAStartup was failed...\n%s\n", Error::GetLastErrorA().c_str());
		return -1;
	}

	// ���� ����
	int returnValue = 0;
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if( listenSocket == INVALID_SOCKET ) {
		printf("Failed creating Socket...\n%s\n", Error::GetLastErrorA().c_str());
		returnValue = -1;
		goto RELEASE;
	}

	// ���Ͽ� ���� �ּҿ� ��Ʈ ��ȣ�� ���ε�
	if( (returnValue = BindSocket(listenSocket, IP_ADDR, StdTCPIP_PRT)) == -1 ) goto RELEASE;

	// ���� ��û�� ����
	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Failed listening...\n%s\n", Error::GetLastErrorA().c_str());
		returnValue = -1;
		goto RELEASE;
	}

	// ���� ���
	printf("wait for clients...\n");
	while(1) {
		struct sockaddr_in from;
		SOCKET msgSock;
		int fromLen = sizeof(from);

		// ��ٸ���
		msgSock = ::accept(listenSocket, (struct sockaddr *)&from, &fromLen);
		if (msgSock == INVALID_SOCKET) {
			printf("Failed accept...\n%s\n", Error::GetLastErrorA().c_str());
			returnValue = -1;
			goto RELEASE;
		}
		// Ŭ���̾�Ʈ �ּ� ���
		printf("%s�� ������ ����\n", ::inet_ntoa(from.sin_addr));

		// ��û ó��
		ProcessRequest(msgSock);
	}

RELEASE:
	// ���� ���̺귯���� ����
	::WSACleanup();
	return returnValue;
}

int BindSocket(SOCKET sd, std::string addrIP, int nPort)
{
	struct sockaddr_in local;

	// ���ڷ� �Ѿ�� ���Ͽ� ���� �ý����� �ּҿ� ��Ʈ ��ȣ�� ���ε�
	local.sin_family			= AF_INET;
	local.sin_addr.S_un.S_addr	= ::inet_addr(addrIP.c_str());
	local.sin_port				= ::htons(nPort);	// ��Ʈ��ũ ����Ʈ ������ ��ȯ(htons)

	if (::bind(sd, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR) {
		printf("Failed binding Socket...\n%s\n", Error::GetLastErrorA().c_str());
		return -1;
	}
	else {
		return 0;
	}
}

DWORD GetPacketSize(SOCKET msgSock)
{
	DWORD dwRet;
	int retval = ::recv(msgSock, (LPSTR)&dwRet, sizeof(DWORD), 0);
	
	return ( (retval == SOCKET_ERROR || retval == 0) ? 0 : dwRet );
}

#include <fstream>
void ProcessRequest(SOCKET msgSock)
{
	DWORD dwPacketSize;
	LPSTR lpData = NULL;
	int retval;

	std::ofstream file("recv.prn");
	if(!file) {
		printf("Failed to make a file...\n");
		::closesocket(msgSock);
		return;
	}

	while(1) {
		dwPacketSize = ::GetPacketSize(msgSock);

		// ó�� 4����Ʈ(DWORD) ������ ���� ���ڿ� �������� ũ�Ⱑ ��� �ִ�.
		if (dwPacketSize == 0) {
			printf("Client closed connection, %d\n", ::WSAGetLastError());
			::closesocket(msgSock);
			break;
		}
		else {	// �������� 4����Ʈ�� �޾�����
			// �޸� �Ҵ�
			lpData = (LPSTR)::malloc(dwPacketSize+1);
			if (lpData) {
				// ������ ����
				retval = ::recv(msgSock, lpData, dwPacketSize, 0);
				if (retval == SOCKET_ERROR || retval == 0) {
					printf("Failed to recv...\n%s\n", Error::GetLastErrorA().c_str());
					::closesocket(msgSock);
					break;
				}
				// ���ڿ� ���� NULL�� ó��
				*(lpData+dwPacketSize) = NULL;
				file << lpData;
			}
			else {
				printf("Memory allocation Failed... %d bytes\n", dwPacketSize);
				::closesocket(msgSock);
				break;
			}
		}
	}
	file.close();
}