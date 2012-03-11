#pragma once
#include "StringUtil.h"
#include <windows.h>
#include <wininet.h>

#define	CANNOT_CONECT		4
#define	CANNOT_REQUEST		5
#define	ERROR_QUERYDATA		6
#define	ERROR_READFILE		7

class HttpRequest
{
public:
	HttpRequest(void);
	HttpRequest(_STRING appName);
	~HttpRequest(void)							{	close();					}
	void setApplicationName(_STRING appName)	{	applicationName = appName;	}
	bool open(std::string host, INTERNET_PORT port=INTERNET_DEFAULT_HTTP_PORT);
	bool isOpened()								{	return (hSession != NULL && hConnect != NULL);	}
	void close();
	std::string requestText(std::string reqObject, std::string reqType = "GET", bool islimit=false, DWORD limitByte=-1);
	bool requestSaveToFile(std::string reqObject, std::string filename, std::string reqType = "GET");
	void putTest();
private:
	_STRING applicationName;
	HINTERNET hSession;
	HINTERNET hConnect;
	HINTERNET hReq;
	_STRING lastErrorMsg;
};
