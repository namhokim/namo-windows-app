#include <stdio.h>	// for printf
#include <Urlmon.h>	// for URLDownloadToFile
#pragma comment(lib, "Urlmon.lib")

int main()
{
	HRESULT hr;

	LPUNKNOWN	lpUnknown = NULL;
	LPCWSTR		szURL = L"http://www.google.com";
	LPCWSTR		szFileName = L"index.html";
	DWORD		dwReserved = 0;
	LPBINDSTATUSCALLBACK lpfnCB = NULL;

	// S_OK, E_OUTOFMEMORY, INET_E_DOWNLOAD_FAILURE
	hr = URLDownloadToFile(
		lpUnknown,
		szURL,
		szFileName,
		dwReserved,
		lpfnCB);
	printf("URLDownloadToFile result: %d\n", hr);

	return 0;
}