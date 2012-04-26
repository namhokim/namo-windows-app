#ifndef UNICODE
#define UNICODE
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */

int main()
{

	DWORD dwRetVal = 0;
	PMIB_ICMP pIcmpStats;

	pIcmpStats = (MIB_ICMP *) MALLOC(sizeof (MIB_ICMP));
	if (pIcmpStats == NULL) {
		wprintf(L"Error allocating memory\n");
		return 1;
	}

	dwRetVal = GetIcmpStatistics(pIcmpStats);
	if (dwRetVal == NO_ERROR) {
		wprintf(L"Number of incoming ICMP messages: %ld\n",
			pIcmpStats->stats.icmpInStats.dwMsgs);
		wprintf(L"Number of incoming ICMP errors received: %ld\n",
			pIcmpStats->stats.icmpInStats.dwErrors);
		wprintf(L"Number of outgoing ICMP messages: %ld\n",
			pIcmpStats->stats.icmpOutStats.dwMsgs);
		wprintf(L"Number of outgoing ICMP errors sent: %ld\n",
			pIcmpStats->stats.icmpOutStats.dwErrors);
	} else {
		wprintf(L"GetIcmpStatistics failed with error: %ld\n", dwRetVal);
	}

	if (pIcmpStats)
		FREE(pIcmpStats);

	return 0;
}
