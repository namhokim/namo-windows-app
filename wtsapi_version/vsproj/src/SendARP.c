#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"	// for SQLite3

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

int FindAndPrint(const char*mac);
int IfExistDBThenCreate();

void usage(char *pname)
{
	fprintf(stderr, "Usage: %s [options] ip-address\n", pname);
	fprintf(stderr, "\t -h \t\thelp\n");
	fprintf(stderr, "\t -l length \tMAC physical address length to set\n");
	fprintf(stderr, "\t -s src-ip \tsource IP address\n");

	IfExistDBThenCreate();

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

	dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &PhysAddrLen);

	if (dwRetVal == NO_ERROR) {
		bPhysAddr = (BYTE *) & MacAddr;
		if (PhysAddrLen) {
			char mac[128] = {0};
			for (i = 0; i < (int) PhysAddrLen; i++) {
				char m[4] = {0};
				if (i == (PhysAddrLen - 1)) {
					sprintf_s(m, 4, "%.2X", (int) bPhysAddr[i]);
					strcat_s(mac, 128, m);
				}
				else {
					sprintf_s(m, 4, "%.2X-", (int) bPhysAddr[i]);
					strcat_s(mac, 128, m);
				}
			}
			FindAndPrint(mac);
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

const char* TABLE_NAME = "mac_map.db";
const char* MSG_NO_TABLE = "no such table: map";
const char* CREATE_TABLE = "CREATE TABLE IF NOT EXISTS map(mac TEXT PRIMARY KEY ASC, name TEXT)";
const char* SELECT_NAME = "SELECT name FROM map WHERE mac = ?";

int FindAndPrint(const char*mac)
{
	sqlite3 *pDb;
	sqlite3_stmt *pStmt;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(TABLE_NAME, &pDb);
	if( rc ) {	// not SQLITE_OK
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDb));
		sqlite3_close(pDb);
		return rc;
	}

	rc = sqlite3_prepare_v2(pDb, SELECT_NAME, (int)strlen(SELECT_NAME), &pStmt, NULL);
	if( rc ) {
		const char* errmsg = sqlite3_errmsg(pDb);
		if ( 0==strcmp(MSG_NO_TABLE, errmsg) ) {	// if not created table
			rc = sqlite3_exec(pDb, CREATE_TABLE , NULL, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
		} else {
			fprintf(stderr, "Can't prepare database: %s\n", sqlite3_errmsg(pDb));
		}
		sqlite3_close(pDb);
		fprintf(stdout, "%s", mac);	// no data (cause created right now)
		return rc;
	}

	rc = sqlite3_bind_text(pStmt, 1, mac, (int)strlen(mac), NULL);
	if( rc ) {	// if not SQLITE_OK
		fprintf(stderr, "Can't bind to query: %d\n", rc);
		return rc;
	}
	rc = sqlite3_step(pStmt);	// SQLITE_ROW(exist data)
	if(SQLITE_ROW==rc) {
		fprintf(stdout, "%s (%s)", sqlite3_column_text(pStmt, 0), mac);
	} else if(SQLITE_DONE==rc) {// SQLITE_DONE (no data)
		fprintf(stdout, "%s", mac);
	} else {
		fprintf(stderr, "Can't step database: %d\n", rc);
	}
	sqlite3_reset(pStmt);

	sqlite3_finalize(pStmt);
	rc = sqlite3_close(pDb);
	return rc;
}

int IfExistDBThenCreate()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(TABLE_NAME, &db);
	if( rc ) {	// not SQLITE_OK
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return rc;
	}

	rc = sqlite3_exec(db, CREATE_TABLE , NULL, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);

	return rc;
}