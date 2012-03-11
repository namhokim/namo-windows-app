/*
 * Copyright (c) 2010 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")

const DWORD BufSize = 128*1024;	// 128KB

int SendData(const char* printer, const char* file);

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

	/*for(int i=0; i<100; i++) {
		printf("전송 %d\n", i);
		SendData();
	}*/
	if(argc!=3) {
		printf("gp.exe [print IP] [file]\n");
		return 1;
	}
	SendData(argv[1], argv[2]);

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
