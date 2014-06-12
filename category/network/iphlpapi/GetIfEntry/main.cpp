#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "IPHLPAPI.lib")

#include <iphlpapi.h>

#include <stdio.h>
#include <stdlib.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */

int main()
{

	// Declare and initialize variables.

	// Declare and initialize variables.
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i, j;

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE *pIfTable;
	MIB_IFROW *pIfRow;

	// Allocate memory for our pointers.
	pIfTable = (MIB_IFTABLE *) MALLOC(sizeof (MIB_IFTABLE));
	if (pIfTable == NULL) {
		printf("Error allocating memory needed to call GetIfTable\n");
		exit (1);
	}
	// Before calling GetIfEntry, we call GetIfTable to make
	// sure there are entries to get and retrieve the interface index.

	// Make an initial call to GetIfTable to get the
	// necessary size into dwSize
	dwSize = sizeof (MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
		FREE(pIfTable);
		pIfTable = (MIB_IFTABLE *) MALLOC(dwSize);
		if (pIfTable == NULL) {
			printf("Error allocating memory\n");
			exit (1);
		}
	}
	// Make a second call to GetIfTable to get the actual
	// data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, 0)) == NO_ERROR) {
		if (pIfTable->dwNumEntries > 0) {
			pIfRow = (MIB_IFROW *) MALLOC(sizeof (MIB_IFROW));
			if (pIfRow == NULL) {
				printf("Error allocating memory\n");
				if (pIfTable != NULL) {
					FREE(pIfTable);
					pIfTable = NULL;
				}
				exit (1);
			}

			printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
			for (i = 0; i < pIfTable->dwNumEntries; i++) {
				pIfRow->dwIndex = pIfTable->table[i].dwIndex;
				if ((dwRetVal = GetIfEntry(pIfRow)) == NO_ERROR) {
					if (!(pIfRow->dwType==IF_TYPE_ETHERNET_CSMACD ||
						pIfRow->dwType==IF_TYPE_IEEE80211)) continue;

					printf("\tIndex:\t %ld\n", pIfRow->dwIndex);
					printf("\tInterfaceName[%d]:\t ", i);
					if (pIfRow->wszName != NULL)
						printf("%ws", pIfRow->wszName);
					printf("\n");

					printf("\tDescription[%d]:\t ", i);
					for (j = 0; j < pIfRow->dwDescrLen; j++)
						printf("%c", pIfRow->bDescr[j]);
					printf("\n");

					printf("\tIndex[%d]:\t\t %d\n", i, pIfRow->dwIndex);

					printf("\tType[%d]:\t\t ", i);
					switch (pIfRow->dwType) {
					case IF_TYPE_OTHER:
						printf("Other\n");
						break;
					case IF_TYPE_ETHERNET_CSMACD:
						printf("Ethernet\n");
						break;
					case IF_TYPE_ISO88025_TOKENRING:
						printf("Token Ring\n");
						break;
					case IF_TYPE_PPP:
						printf("PPP\n");
						break;
					case IF_TYPE_SOFTWARE_LOOPBACK:
						printf("Software Lookback\n");
						break;
					case IF_TYPE_ATM:
						printf("ATM\n");
						break;
					case IF_TYPE_IEEE80211:
						printf("IEEE 802.11 Wireless\n");
						break;
					case IF_TYPE_TUNNEL:
						printf("Tunnel type encapsulation\n");
						break;
					case IF_TYPE_IEEE1394:
						printf("IEEE 1394 Firewire\n");
						break;
					default:
						printf("Unknown type %ld\n", pIfRow->dwType);
						break;
					}

					printf("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);

					printf("\tSpeed[%d]:\t\t %ld\n", i, pIfRow->dwSpeed);

					printf("\tPhysical Addr:\t\t ");
					if (pIfRow->dwPhysAddrLen == 0)
						printf("\n");
					//                    for (j = 0; j < (int) pIfRow->dwPhysAddrLen; j++) {
					for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
						if (j == (pIfRow->dwPhysAddrLen - 1))
							printf("%.2X\n", (int) pIfRow->bPhysAddr[j]);
						else
							printf("%.2X-", (int) pIfRow->bPhysAddr[j]);
					}
					printf("\tAdmin Status[%d]:\t %ld\n", i,
						pIfRow->dwAdminStatus);

					printf("\tOper Status[%d]:\t ", i);
					switch (pIfRow->dwOperStatus) {
					case IF_OPER_STATUS_NON_OPERATIONAL:
						printf("Non Operational\n");
						break;
					case IF_OPER_STATUS_UNREACHABLE:
						printf("Unreasonable\n");
						break;
					case IF_OPER_STATUS_DISCONNECTED:
						printf("Disconnected\n");
						break;
					case IF_OPER_STATUS_CONNECTING:
						printf("Connecting\n");
						break;
					case IF_OPER_STATUS_CONNECTED:
						printf("Connected\n");
						break;
					case IF_OPER_STATUS_OPERATIONAL:
						printf("Operational\n");
						break;
					default:
						printf("Unknown status %ld\n", 
							pIfRow->dwAdminStatus);
						break;
					}
					printf("\n");
				}

				else {
					printf("GetIfEntry failed for index %d with error: %ld\n",
						i, dwRetVal);
					// Here you can use FormatMessage to find out why 
					// it failed.

				}
			}
		} else {
			printf("\tGetIfTable failed with error: %ld\n", dwRetVal);
		}

	}

	exit (0);
}