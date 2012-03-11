#include <Windows.h>
#include <WinInet.h>
#include <Urlmon.h>

#include <stdio.h>

#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Urlmon.lib")

int main() {
	TCHAR *url = TEXT("http://ko.wikipedia.org/wiki/애플 (기업)");
	TCHAR enc[1024] = {0,};
	DWORD dwBufferLength = 1024;
	BOOL bSuccess = InternetCanonicalizeUrl(url, enc, &dwBufferLength, 0);
	if(FALSE==bSuccess) {
		DWORD dwCode = GetLastError();
		printf("Error : %u\n", dwCode);
	}

	WCHAR szOut[INTERNET_MAX_URL_LENGTH];
	DWORD cchDecodedUrl = INTERNET_MAX_URL_LENGTH;
	HRESULT hResult = CoInternetParseUrl(url, PARSE_CANONICALIZE, 0, szOut, INTERNET_MAX_URL_LENGTH, &cchDecodedUrl, 0);
	return 0;
}