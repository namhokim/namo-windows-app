#include <stdio.h>
#include <Winsock2.h>
#include <TCHAR.H>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WORD wVersionRequested;
    WSADATA wsaData;
	int err;

	struct sockaddr_in add;
	TCHAR szAddressString[128] = {0};
	DWORD dwAddressStringLength = 0;

	// init
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

	// set
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = inet_addr("127.0.0.1");
	add.sin_port = htons(80);
	ZeroMemory(&add.sin_zero,8);
	dwAddressStringLength = sizeof(szAddressString) / sizeof(TCHAR);

	// use
	INT ret = WSAAddressToString((LPSOCKADDR)&add, sizeof(struct sockaddr), NULL, szAddressString, &dwAddressStringLength);
	if(0!=ret) {
		err = ::WSAGetLastError();
		printf("Error : %d\n", err);
	} else {
		_tprintf(TEXT("%s\n"), szAddressString);
	}

	// clean
	WSACleanup();

	return 0;
}