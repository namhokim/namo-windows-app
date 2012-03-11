
// Link with Iphlpapi.lib
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */

int __cdecl main(int argc, char **argv)
{

	/* Declare and initialize variables */

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	int i = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_INET;	//AF_INET6, AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
	PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
	PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
	IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
	IP_ADAPTER_PREFIX *pPrefix = NULL;

	outBufLen = sizeof (IP_ADAPTER_ADDRESSES);
	pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
	if (pAddresses == NULL) {
		printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
		exit(1);
	}

	// Make an initial call to GetAdaptersAddresses to get the 
	// size needed into the outBufLen variable
	if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen)
		== ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
	}

	if (pAddresses == NULL) {
		printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
		exit(1);
	}
	// Make a second call to GetAdapters Addresses to get the
	// actual data we want

	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;
		int cntNIC = 1;
		while (pCurrAddresses) {

			if (pCurrAddresses->PhysicalAddressLength != 0) {
				printf("%d> Physical address: ", cntNIC++);
				for (i = 0; i < (int) pCurrAddresses->PhysicalAddressLength;
					i++) {
						if (i == (pCurrAddresses->PhysicalAddressLength - 1))
							printf("%.2X\n",
							(int) pCurrAddresses->PhysicalAddress[i]);
						else
							printf("%.2X-",
							(int) pCurrAddresses->PhysicalAddress[i]);
				}
			}

			pCurrAddresses = pCurrAddresses->Next;
		}
	} else {
		printf("Call to GetAdaptersAddresses failed with error: %d\n",
			dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else {

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   // Default language
				(LPTSTR) & lpMsgBuf, 0, NULL)) {
					printf("\tError: %s", lpMsgBuf);
					LocalFree(lpMsgBuf);
					FREE(pAddresses);
					exit(1);
			}
		}
	}
	FREE(pAddresses);
	
	printf("엔터키를 누르세요...");
	getchar();
	return 0;
}
