#pragma once
#include "StringUtil.h"
#include <windows.h>
#include <wininet.h>

class FTPRequest
{
public:
	FTPRequest(void);
	FTPRequest(_STRING appName);
	~FTPRequest(void)							{	close();					}
	void setApplicationName(_STRING appName)	{	applicationName = appName;	}
	bool open(_STRING host, _STRING id=_T(""), _STRING password=_T(""), INTERNET_PORT port=INTERNET_DEFAULT_FTP_PORT);
	bool isOpened()								{	return (hSession != NULL && hConnect != NULL);	}
	void close();

	// command
	bool put(_STRING filename, DWORD mode=FTP_TRANSFER_TYPE_ASCII);
	bool get(_STRING filename, _STRING localpath=_T(""), DWORD mode=FTP_TRANSFER_TYPE_ASCII);
	_STRING dir();
private:
	_STRING applicationName;
	HINTERNET hSession;
	HINTERNET hConnect;
	HINTERNET hReq;
};
