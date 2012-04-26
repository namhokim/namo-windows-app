#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

void main()
{
	OVERLAPPED overlap;
	DWORD ret;

	HANDLE hand = NULL;
	overlap.hEvent = WSACreateEvent();

	ret = NotifyAddrChange(&hand, &overlap);

	if (ret != NO_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("NotifyAddrChange error...%d\n", WSAGetLastError());            
			return;
		}
	}

	if ( WaitForSingleObject(overlap.hEvent, INFINITE) == WAIT_OBJECT_0 )
		printf("IP Address table changed..\n");
}