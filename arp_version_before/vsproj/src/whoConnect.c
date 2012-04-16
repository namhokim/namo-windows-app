#include "sqlite3.h"	// for SQLite3

int OpenDB(sqlite3 **ppDb, sqlite3_stmt **ppStmt);
void PrintIfMatch(sqlite3_stmt *pStmt, const char*ip, int repeatCount);
void CloseDB(sqlite3 *pDb, sqlite3_stmt *pStmt);

int GetWhoIsIP(const char*ip);
int IfExistDBThenCreate();

#ifdef _MSC_VER
// GetTcpTable function 
// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/aa366026(v=vs.85).aspx

// Need to link with Iphlpapi.lib and Ws2_32.lib
#include <winsock2.h>	// for UCHAR, USHORT, Word
#include <iphlpapi.h>	// for PMIB_TCPTABLE, MIB_TCPTABLE
#include <stdio.h>		// for fprint, stdout, stderr

#pragma comment(lib, "iphlpapi.lib")	// for GetTcpTable
#pragma comment(lib, "ws2_32.lib")		// for ntohs, inet_ntoa

/* Note: could also use malloc() and free() */
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define ERR_NO_ERROR	0
#define	ERR_NO_ARG		1
#define	ERR_INVALID_ARG	2
#define	ERR_MEM_ALLOC	3

void showUsageAndExit(int code)
{
	IfExistDBThenCreate();
	fprintf(stderr, "whoConnect.exe [my port]\n");
	exit(code);
}

int main(int argc, char* argv[])
{
	// Declare and initialize variables
	PMIB_TCPTABLE pTcpTable;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	struct in_addr IpAddr;

	int i, port, c;

	if(argc!=2) {
		showUsageAndExit(ERR_NO_ARG);
	} else {
		port = atoi(argv[1]);
		if(0==port) exit(ERR_INVALID_ARG);
	}

	pTcpTable = (MIB_TCPTABLE *) MALLOC(sizeof (MIB_TCPTABLE));
	if (pTcpTable == NULL) {
		fprintf(stderr, "Error allocating memory\n");
		return ERR_MEM_ALLOC;
	}

	dwSize = sizeof (MIB_TCPTABLE);
	// Make an initial call to GetTcpTable to
	// get the necessary size into the dwSize variable
	if ((dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) ==
		ERROR_INSUFFICIENT_BUFFER) {
			FREE(pTcpTable);
			pTcpTable = (MIB_TCPTABLE *) MALLOC(dwSize);
			if (pTcpTable == NULL) {
				fprintf(stderr, "Error allocating memory\n");
				return ERR_MEM_ALLOC;
			}
	}
	// Make a second call to GetTcpTable to get
	// the actual data we require
	if ((dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE)) == NO_ERROR) {
		sqlite3			*db;
		sqlite3_stmt	*stmt;
		OpenDB(&db, &stmt);
		for (i = 0, c = 0; i < (int) pTcpTable->dwNumEntries; i++) {
			PMIB_TCPROW row = &(pTcpTable->table[i]);

			if( MIB_TCP_STATE_ESTAB == (row->dwState)		// only ESTABLISHED
				&& ntohs((u_short)row->dwLocalPort) == port)// and wanted port
			{
				char* remoteIP;
				IpAddr.S_un.S_addr = (u_long) row->dwRemoteAddr;
				remoteIP = inet_ntoa(IpAddr);
				
				PrintIfMatch(stmt, remoteIP, c);

				c++;
			}
		}
		CloseDB(db, stmt);
	} else {
		fprintf(stderr, "\tGetTcpTable failed with %d\n", dwRetVal);
		FREE(pTcpTable);
		return 1;
	}

	if (pTcpTable != NULL) {
		FREE(pTcpTable);
		pTcpTable = NULL;
	}

	return 0;    
}

#else	// for dummy
#include <stdio.h>
#include <unistd.h>

int main() {
    sleep(1);
    printf("127.0.0.1\n");
    return 0;
}

#endif	// _MSC_VER

const char* TABLE_NAME = "ip_map.db";
const char* MSG_NO_TABLE = "no such table: map";
const char* CREATE_TABLE = "CREATE TABLE IF NOT EXISTS map(ip TEXT PRIMARY KEY ASC, name TEXT)";
const char* SELECT_NAME = "SELECT name FROM map WHERE ip = ?";

int OpenDB(sqlite3 **ppDb, sqlite3_stmt **ppStmt)
{
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(TABLE_NAME, ppDb);
	if( rc ) {	// not SQLITE_OK
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*ppDb));
		sqlite3_close(*ppDb);
		return rc;
	}

	rc = sqlite3_prepare_v2(*ppDb, SELECT_NAME, (int)strlen(SELECT_NAME), ppStmt, NULL);
	if( rc ) {
		const char* errmsg = sqlite3_errmsg(*ppDb);
		if ( 0==strcmp(MSG_NO_TABLE, errmsg) ) {	// if not created table
			rc = sqlite3_exec(*ppDb, CREATE_TABLE , NULL, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
		} else {
			fprintf(stderr, "Can't prepare database: %s\n", sqlite3_errmsg(*ppDb));
		}
		sqlite3_close(*ppDb);
	}
	return rc;
}

void PrintIfMatch(sqlite3_stmt *pStmt, const char*ip, int repeatCount)
{
	int rc;

	if(NULL==pStmt) {	// if something using DB
		if(repeatCount) fprintf(stdout, " ");
		fprintf(stdout, "%s", ip);
		return;
	}

	rc = sqlite3_bind_text(pStmt, 1, ip, (int)strlen(ip), NULL);
	if( rc ) {	// if not SQLITE_OK
		fprintf(stderr, "Can't bind to query: %d\n", rc);
		return;
	}
	rc = sqlite3_step(pStmt);	// SQLITE_ROW(exist data)
	if(SQLITE_ROW==rc) {
		if(repeatCount) fprintf(stdout, ",");
		fprintf(stdout, "%s (%s)", ip, sqlite3_column_text(pStmt, 0));
	} else if(SQLITE_DONE==rc) {// SQLITE_DONE (no data)
		if(repeatCount) fprintf(stdout, ",");
		fprintf(stdout, "%s", ip);
	} else {
		fprintf(stderr, "Can't step database: %d\n", rc);
	}
	sqlite3_reset(pStmt);
}

void CloseDB(sqlite3 *pDb, sqlite3_stmt *pStmt)
{
	sqlite3_finalize(pStmt);
	sqlite3_close(pDb);
}

int GetWhoIsIP(const char*ip)
{
	sqlite3			*db;
	sqlite3_stmt	*stmt;

	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(TABLE_NAME, &db);
	if( rc ) {	// not SQLITE_OK
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return rc;
	}

	rc = sqlite3_prepare(db, SELECT_NAME, (int)strlen(SELECT_NAME), &stmt, NULL);
	if( rc ) {
		fprintf(stderr, "Can't prepare database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return rc;
	}

	//
	rc = sqlite3_bind_text(stmt, 1, ip, (int)strlen(ip), NULL);
	rc = sqlite3_step(stmt);	// SQLITE_DONE (no data), SQLITE_ROW(exist data)
	if(SQLITE_ROW==rc) {
		fprintf(stdout, "name: %s\n", sqlite3_column_text(stmt, 0));
	}
	rc = sqlite3_reset(stmt);
	//

	rc = sqlite3_finalize(stmt);
	sqlite3_close(db);
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