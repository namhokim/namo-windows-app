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
	bool open(_STRING host, INTERNET_PORT port=INTERNET_DEFAULT_HTTP_PORT);
	bool isOpened()								{	return (hSession != NULL && hConnect != NULL);	}
	void close();
	std::string requestText(_STRING reqObject, _STRING reqType = _T("GET"));
	void putTest();
private:
	_STRING applicationName;
	HINTERNET hSession;
	HINTERNET hConnect;
	HINTERNET hReq;
};
