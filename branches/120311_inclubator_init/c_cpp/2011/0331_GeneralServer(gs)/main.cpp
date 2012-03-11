/*
 * Copyright (c) 2010 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

const DWORD BufSize = 128*1024;	// 128KB

int SendData(const char* printer, const char* file);
int StartMyService(const char* port);

// Winsock 2 DLL 종료 시키는 래퍼 클래스
class CEnsureWsClean
{
public:
	CEnsureWsClean():m_isInit(FALSE) {
		if(WSAStartup(MAKEWORD(2,2), &m_wsa) == 0) {
			m_isInit = TRUE;
		}
	}

	~CEnsureWsClean() {
		Cleanup();
	}

private:
	WSADATA m_wsa;
	BOOL m_isInit;
	void Cleanup() {
		if(m_isInit) WSACleanup();
	}
};

int main(int argc, char *argv[])
{
	CEnsureWsClean ws;

	if(argc!=2) {
		printf("gs.exe [port]\n");
		return 1;
	}
	StartMyService(argv[1]);
	//SendData(argv[1], argv[2]);

	return 0;
}

// HANDLE 자동 닫는 래퍼 클래스
class CEnsureFileHandle
{
public:
	CEnsureFileHandle():m_hFile(INVALID_HANDLE_VALUE) {
	}

	CEnsureFileHandle(HANDLE hFile):m_hFile(hFile) {
	}

	~CEnsureFileHandle() {
		Cleanup();
	}

	HANDLE operator=(HANDLE hFile) { 
		Cleanup(); 
		m_hFile = (HANDLE) hFile;
		return(*this);  
	}

	operator HANDLE() {
		return (HANDLE)m_hFile;
	}

private:
	HANDLE m_hFile;
	void Cleanup() {
		if(INVALID_HANDLE_VALUE!=m_hFile) ::CloseHandle(m_hFile);
	}
};

int SendData(const char* printer, const char* file)
{
	CEnsureFileHandle hFile = ::CreateFileA(file,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if(INVALID_HANDLE_VALUE!=hFile) {
		// socket
		SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
		if(s==INVALID_SOCKET) return FALSE;

		// connect
		SOCKADDR_IN sa;
		sa.sin_family		= AF_INET;
		sa.sin_port			= htons(9100);
		sa.sin_addr.s_addr	= ::inet_addr(printer);
		int rVal = ::connect(s, (SOCKADDR *)&sa, sizeof(sa));
		if(SOCKET_ERROR == rVal) return FALSE;

		BYTE buf[BufSize];
		DWORD dwRead = 0;
		::ReadFile(hFile, buf, BufSize, &dwRead, NULL);
		while(dwRead>0) {
			rVal = ::send(s, (const char *)buf, dwRead, 0);
			if(SOCKET_ERROR == rVal) return FALSE;

			::ReadFile(hFile, buf, BufSize, &dwRead, NULL);
		}

		rVal = ::closesocket(s);
		if(0 != rVal) return FALSE;

		return TRUE;
	} else {
		return FALSE;
	}	
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display2(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int StartMyService(const char* port)
{
	int rVal;

	// socket
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
	if(s==INVALID_SOCKET) return FALSE;

	// bind
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family		= AF_INET;
	sa.sin_port			= htons(atoi(port));
	sa.sin_addr.s_addr	= htonl(INADDR_ANY);
	rVal = bind(s, (SOCKADDR *)&sa, sizeof(sa));
	if(rVal==SOCKET_ERROR) {
		err_display("#bind()");
		return FALSE;
	}

	// listen
	rVal = listen(s, SOMAXCONN);
	if(rVal==SOCKET_ERROR) {
		err_display("#listen()");
		return FALSE;
	}

	SOCKET c;
	SOCKADDR_IN ca;
	int addrlen = sizeof(ca);
	BYTE buf[BufSize];

	// accept
	c = accept(s, (SOCKADDR *)&ca, &addrlen);
	if(c == INVALID_SOCKET) {
		err_display("#accept()");
		::closesocket(s);
		return FALSE;
	}

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	while(1) {

		// recv
		rVal = recv(c, reinterpret_cast<char *>(buf), BufSize, 0);
		if(rVal == SOCKET_ERROR) {
			err_display("#recv()");
			break;
		} else if(rVal == 0) {
			break;
		}

		// Write
		BOOL bRes = FALSE;
		DWORD NumberOfBytesWrite = 0;
		bRes = WriteFile(hStdOut, buf, rVal, &NumberOfBytesWrite, NULL);
		if(!bRes) {
			err_display2("#WriteFile()");
			break;
		} else if(NumberOfBytesWrite == 0) {
			break;
		}
	}

	::closesocket(c);
	::closesocket(s);

	return 0;
}