#include "HttpRequest.h"
#pragma comment(lib, "wininet.lib")
#include "FileManager.h"

HttpRequest::HttpRequest(void)
: applicationName(_T("HTTPNAMO"))
{
}

HttpRequest::HttpRequest(_STRING appName)
: applicationName(appName)
{
}


bool HttpRequest::open(std::string host, INTERNET_PORT port)
{
	hSession = InternetOpen(applicationName.c_str(), INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL, CACHEGROUP_SEARCH_ALL);
	if (!hSession) return false;

	hConnect = InternetConnectA(hSession,host.c_str(),port,"","",INTERNET_SERVICE_HTTP,0,0);
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

std::string HttpRequest::requestText(std::string reqObject, std::string reqType,
									 bool islimit, DWORD limitByte)
{
	if(!isOpened()) return "not conected";

	hReq=HttpOpenRequestA(hConnect,reqType.c_str(),reqObject.c_str(),NULL,NULL,NULL,0,0);
	if (!hReq) return "not conected";

	if(HttpSendRequest(hReq,NULL,0,NULL,0) != TRUE) return "request error";

	const int bufferSize = 65000;
	char buf[bufferSize];
	DWORD Size, dwRead, totalRead=0;
	std::string returnDoc("");
	do {
		if(InternetQueryDataAvailable(hReq,&Size,0,0) != TRUE) return "InternetQueryDataAvailable Error";
		if(InternetReadFile(hReq,buf,Size,&dwRead) != TRUE) return "InternetReadFile Error";
		totalRead += dwRead;
		buf[dwRead]='\0';
		returnDoc.append(buf);
		if(islimit && totalRead >= limitByte) break;
	} while (dwRead != 0);

	InternetCloseHandle(hReq);
	hReq = NULL;

	return returnDoc;
}

bool HttpRequest::requestSaveToFile(std::string reqObject, std::string filename, std::string reqType)
{
	if(!isOpened()) {
		lastErrorMsg = TEXT("not conected");
		return false;
	}

	// file handle open
	FileManager fileManager;
	bool res = fileManager.openA(filename, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL);
	if(res != true) {
		lastErrorMsg = TEXT("file create error");
		DWORD code = GetLastError();
		return false;
	}

	hReq=HttpOpenRequestA(hConnect,reqType.c_str(),reqObject.c_str(),NULL,NULL,NULL,0,0);
	if (!hReq) {
		lastErrorMsg = TEXT("not conected");
		return false;
	}

	if(HttpSendRequest(hReq,NULL,0,NULL,0) != TRUE) {
		lastErrorMsg = TEXT("request error");
		return false;
	}

	const int bufferSize = 65000;
	BYTE buf[bufferSize];
	DWORD Size, dwRead, dwWritten;
	
	do {
		if(InternetQueryDataAvailable(hReq,&Size,0,0) != TRUE) {
			lastErrorMsg = TEXT("InternetQueryDataAvailable Error");
			return false;
		}
		if(InternetReadFile(hReq,buf,Size,&dwRead) != TRUE) {
			lastErrorMsg = TEXT("InternetReadFile Error");
			return false;
		}
		fileManager.writeFile(buf, dwRead, &dwWritten);
	} while (dwRead != 0);

	fileManager.close();	// 없어도 소멸자에 의해 파일핸들이 닫아진다.
	InternetCloseHandle(hReq);
	hReq = NULL;

	return true;
}

void HttpRequest::putTest()
{
	if(!isOpened()) return;
	hReq=HttpOpenRequest(hConnect,L"PUT",L"/test/page.htm",NULL,NULL,NULL,0,0);
	_TCHAR szContent[] = L"Hello, World!";
	BOOL res = HttpSendRequest(hReq, NULL, 0, szContent, lstrlen(szContent));	// 제한된 크기의 단일 버퍼에만 사용
}