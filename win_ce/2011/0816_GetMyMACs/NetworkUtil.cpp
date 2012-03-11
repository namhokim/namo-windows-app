#include "stdafx.h"
#include "NetworkUtil.h"
#include <iphlpapi.h>	// for IP Helper Functions
#pragma comment(lib, "iphlpapi.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

BOOL GetMyMACs(CString &str)
{
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
        str = _T("Error allocating memory needed to call GetIfTable");
        return FALSE;
    }
    // Make an initial call to GetIfTable to get the
    // necessary size into dwSize
    dwSize = sizeof (MIB_IFTABLE);
    if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        FREE(pIfTable);
        pIfTable = (MIB_IFTABLE *) MALLOC(dwSize);
        if (pIfTable == NULL) {
            str = _T("Error allocating memory needed to call GetIfTable");
            return FALSE;
        }
    }
    // Make a second call to GetIfTable to get the actual
    // data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
		//printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
		str.Empty();
		for (i = 0; (i < pIfTable->dwNumEntries); i++) {
			pIfRow = (MIB_IFROW *) & pIfTable->table[i];
			if (IF_TYPE_ETHERNET_CSMACD==(pIfRow->dwType)) {
				for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
					CString t;	
					if (j == (pIfRow->dwPhysAddrLen - 1)) {
						t.Format(_T("%.2X"), (int) pIfRow->bPhysAddr[j]);
					} else {
						if(j == 0 && str.GetLength() > 0) {
							t.Format(_T(";%.2X-"), (int) pIfRow->bPhysAddr[j]);
						} else {
							t.Format(_T("%.2X-"), (int) pIfRow->bPhysAddr[j]);
						}
					}
					str.Append(t);
				}
			}
        }
    } else {
		str.Format(_T("GetIfTable failed with error: %u"), dwRetVal);
        if (pIfTable != NULL) {
            FREE(pIfTable);
            pIfTable = NULL;
        }  
        return FALSE;
        // Here you can use FormatMessage to find out why it failed.
    }
    if (pIfTable != NULL) {
        FREE(pIfTable);
        pIfTable = NULL;
    }

	return TRUE;
}