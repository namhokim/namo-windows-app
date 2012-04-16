#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

void usage(char *pname)
{
	fprintf(stderr, "Usage: %s [options] ip-address\n", pname);
	fprintf(stderr, "\t -h \t\thelp\n");
	fprintf(stderr, "\t -l length \tMAC physical address length to set\n");
	fprintf(stderr, "\t -s src-ip \tsource IP address\n");
	exit(1);
}

int __cdecl main(int argc, char **argv)
{
	DWORD dwRetVal;
	IPAddr DestIp = 0;
	IPAddr SrcIp = 0;       /* default for src ip */
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG PhysAddrLen = 6;  /* default to length of six bytes */

	char *DestIpString = NULL;
	char *SrcIpString = NULL;

	BYTE *bPhysAddr;
	unsigned int i;

	if (argc > 1) {
		for (i = 1; i < (unsigned int) argc; i++) {
			if ((argv[i][0] == '-') || (argv[i][0] == '/')) {
				switch (tolower(argv[i][1])) {
				case 'l':
					PhysAddrLen = (ULONG) atol(argv[++i]);
					break;
				case 's':
					SrcIpString = argv[++i];
					SrcIp = inet_addr(SrcIpString);
					break;
				case 'h':
				default:
					usage(argv[0]);
					break;
				}               /* end switch */
			} else
				DestIpString = argv[i];
		}                       /* end for */
	} else
		usage(argv[0]);

	if (DestIpString == NULL || DestIpString[0] == '\0')
		usage(argv[0]);

	DestIp = inet_addr(DestIpString);

	memset(&MacAddr, 0xff, sizeof (MacAddr));

	//printf("Sending ARP request for IP address: %s\n", DestIpString);

	dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &PhysAddrLen);

	if (dwRetVal == NO_ERROR) {
		bPhysAddr = (BYTE *) & MacAddr;
		if (PhysAddrLen) {
			for (i = 0; i < (int) PhysAddrLen; i++) {
				if (i == (PhysAddrLen - 1))
					fprintf(stdout,"%.2X", (int) bPhysAddr[i]);
				else
					fprintf(stdout,"%.2X-", (int) bPhysAddr[i]);
			}
		} else
			fprintf(stderr,
			"Warning: SendArp completed successfully, but returned length=0\n");

	} else {
		fprintf(stderr, "Error: SendArp failed with error: %d", dwRetVal);
		switch (dwRetVal) {
		case ERROR_GEN_FAILURE:
			fprintf(stderr, " (ERROR_GEN_FAILURE)\n");
			break;
		case ERROR_INVALID_PARAMETER:
			fprintf(stderr, " (ERROR_INVALID_PARAMETER)\n");
			break;
		case ERROR_INVALID_USER_BUFFER:
			fprintf(stderr, " (ERROR_INVALID_USER_BUFFER)\n");
			break;
		case ERROR_BAD_NET_NAME:
			fprintf(stderr, " (ERROR_GEN_FAILURE)\n");
			break;
		case ERROR_BUFFER_OVERFLOW:
			fprintf(stderr, " (ERROR_BUFFER_OVERFLOW)\n");
			break;
		case ERROR_NOT_FOUND:
			fprintf(stderr, " (ERROR_NOT_FOUND)\n");
			break;
		default:
			fprintf(stderr, "\n");
			break;
		}
	}

	return 0;
}
