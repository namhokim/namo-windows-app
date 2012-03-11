#include <windows.h>
#include <wininet.h>
#include "common.h"
#include "httpModule.h"
#include "logfileModule.h"
#include <iostream>
using namespace std;

HINTERNET hInternet	= NULL;
HINTERNET hHttp		= NULL;

INT HTTPDown(PCHAR host)
{
	DWORD Size;
	INTERNET_DIAGNOSTIC_SOCKET_INFO info;

	// Initializes an application's use of the WinINet functions
	if(NULL == hInternet) {
		hInternet=InternetOpen("naverAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
		if (hInternet == NULL) {				// initialize WinINet fails
			cout << "init WinINet functions Error!\n";
			return CONNECT_ERROR;
		}
		else cout << "new WinINet functions\n";
	}

	// Opens an HTTP session
	if(NULL == hHttp) {
		hHttp = InternetConnect(hInternet,host,INTERNET_DEFAULT_HTTP_PORT,"","",INTERNET_SERVICE_HTTP,0,0);
		if (hHttp==NULL) {
			cout << "Opens an HTTP session Error!\n";
			return CONNECT_ERROR;	// open session fails
		}
		else cout << "Opens an HTTP session\n";
	}

	
	InternetQueryOption(hHttp, INTERNET_OPTION_DIAGNOSTIC_SOCKET_INFO, &info, &Size);
	cout << "info.Socket : " << info.Socket << endl;
	cout << "info.SourcePort : " << info.SourcePort << endl;
	cout << "info.DestPort : " << info.DestPort << endl;
	cout << "info.Flags : " << info.Flags << endl;
	
	CleanUpConnectResources();

	return TRUE;
}


void CleanUpConnectResources()
{
	if(NULL != hHttp) InternetCloseHandle(hHttp);
	hHttp=NULL;
	if(NULL != hInternet) InternetCloseHandle(hInternet);
	hInternet=NULL;
}