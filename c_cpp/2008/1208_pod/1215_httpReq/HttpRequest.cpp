#include "HttpRequest.h"
#pragma comment(lib, "wininet.lib")

HttpRequest::HttpRequest(void)
: applicationName(_T("HTTPNAMO"))
{
}

HttpRequest::HttpRequest(_STRING appName)
: applicationName(appName)
{
}


bool HttpRequest::open(_STRING host, INTERNET_PORT port)
{
	hSession = InternetOpen(applicationName.c_str(), INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL, CACHEGROUP_SEARCH_ALL);
	if (!hSession) return false;

	hConnect = InternetConnect(hSession,host.c_str(),port,_T(""),_T(""),INTERNET_SERVICE_HTTP,0,0);
	if (!hConnect) return false;
	return true;
}

void HttpRequest::close()
{
	if (hReq) {
		InternetCloseHandle(hReq);
		hReq = NULL;
	}

	if (hConnect) {
		InternetCloseHandle(hConnect);
		hConnect = NULL;
	}
	if (hSession) {
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
}

std::string HttpRequest::requestText(_STRING reqObject, _STRING reqType)
{
	if(!isOpened()) return "not conected";

	hReq=HttpOpenRequest(hConnect,reqType.c_str(),reqObject.c_str(),NULL,NULL,NULL,0,0);
	if (!hReq) return "not conected";

	if(HttpSendRequest(hReq,NULL,0,NULL,0) != TRUE) return "request error";

	const int bufferSize = 65000;
	char buf[bufferSize];
	DWORD Size, dwRead;
	std::string returnDoc("");
	do {
		if(InternetQueryDataAvailable(hReq,&Size,0,0) != TRUE) return "InternetQueryDataAvailable Error";
		if(InternetReadFile(hReq,buf,Size,&dwRead) != TRUE) return "InternetReadFile Error";
		buf[dwRead]='\0';
		returnDoc.append(buf);
	} while (dwRead != 0);

	InternetCloseHandle(hReq);
	hReq = NULL;

	return returnDoc;
}

void HttpRequest::putTest()
{
	if(!isOpened()) return;
	hReq=HttpOpenRequest(hConnect,L"PUT",L"/test/page.htm",NULL,NULL,NULL,0,0);
	_TCHAR szContent[] = L"Hello, World!";
	BOOL res = HttpSendRequest(hReq, NULL, 0, szContent, lstrlen(szContent));	// 제한된 크기의 단일 버퍼에만 사용
}