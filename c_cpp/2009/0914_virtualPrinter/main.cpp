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
	// 소켓 라이브러리를 초기화
	if (::WSAStartup(0x202, &wsaData) != 0) {
		printf("WSAStartup was failed...\n%s\n", Error::GetLastErrorA().c_str());
		return -1;
	}

	// 소켓 생성
	int returnValue = 0;
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if( listenSocket == INVALID_SOCKET ) {
		printf("Failed creating Socket...\n%s\n", Error::GetLastErrorA().c_str());
		returnValue = -1;
		goto RELEASE;
	}

	// 소켓에 로컬 주소와 포트 번호를 바인딩
	if( (returnValue = BindSocket(listenSocket, IP_ADDR, StdTCPIP_PRT)) == -1 ) goto RELEASE;

	// 동시 요청수 지정
	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Failed listening...\n%s\n", Error::GetLastErrorA().c_str());
		returnValue = -1;
		goto RELEASE;
	}

	// 연결 대기
	printf("wait for clients...\n");
	while(1) {
		struct sockaddr_in from;
		SOCKET msgSock;
		int fromLen = sizeof(from);

		// 기다린다
		msgSock = ::accept(listenSocket, (struct sockaddr *)&from, &fromLen);
		if (msgSock == INVALID_SOCKET) {
			printf("Failed accept...\n%s\n", Error::GetLastErrorA().c_str());
			returnValue = -1;
			goto RELEASE;
		}
		// 클라이언트 주소 출력
		printf("%s로 연결이 들어옴\n", ::inet_ntoa(from.sin_addr));

		// 요청 처리
		ProcessRequest(msgSock);
	}

RELEASE:
	// 소켓 라이브러리를 해제
	::WSACleanup();
	return returnValue;
}

int BindSocket(SOCKET sd, std::string addrIP, int nPort)
{
	struct sockaddr_in local;

	// 인자로 넘어온 소켓에 현재 시스템의 주소와 포트 번호를 바인딩
	local.sin_family			= AF_INET;
	local.sin_addr.S_un.S_addr	= ::inet_addr(addrIP.c_str());
	local.sin_port				= ::htons(nPort);	// 네트워크 바이트 순서로 변환(htons)

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

		// 처음 4바이트(DWORD) 다음에 오는 문자열 데이터의 크기가 들어 있다.
		if (dwPacketSize == 0) {
			printf("Client closed connection, %d\n", ::WSAGetLastError());
			::closesocket(msgSock);
			break;
		}
		else {	// 에러없이 4바이트를 받았으면
			// 메모리 할당
			lpData = (LPSTR)::malloc(dwPacketSize+1);
			if (lpData) {
				// 데이터 받음
				retval = ::recv(msgSock, lpData, dwPacketSize, 0);
				if (retval == SOCKET_ERROR || retval == 0) {
					printf("Failed to recv...\n%s\n", Error::GetLastErrorA().c_str());
					::closesocket(msgSock);
					break;
				}
				// 문자열 끝을 NULL로 처리
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