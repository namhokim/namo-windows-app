#include "FTPRequest.h"
#pragma comment(lib, "wininet.lib")

FTPRequest::FTPRequest(void)
: applicationName(_T("FTPNAMO"))
{
	std::wcout.imbue(std::locale("korean"));
}

FTPRequest::FTPRequest(_STRING appName)
: applicationName(appName)
{
	std::wcout.imbue(std::locale("korean"));
}

bool FTPRequest::open(_STRING host, _STRING id, _STRING password, INTERNET_PORT port)
{
	hSession = InternetOpen(applicationName.c_str(), INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL, CACHEGROUP_SEARCH_ALL);
	if (!hSession) return false;

	hConnect = InternetConnect(hSession,host.c_str(),port,id.c_str(),password.c_str(),INTERNET_SERVICE_FTP,0,0);
	if (!hConnect) return false;
	return true;
}

bool FTPRequest::put(_STRING filename, DWORD mode)
{
	if(!isOpened()) return false;

	// 원격 파일(로컬파일명에서 파일명.확장자만 분리)
	_STRING::size_type position = filename.find_last_of(_T("\\"));
	_STRING onlyFilename;
	if(position == _STRING::npos) onlyFilename = filename;
	else onlyFilename = filename.substr(position, filename.size());

	DWORD dwFlags = mode | INTERNET_FLAG_RELOAD;
	return (FtpPutFile(hConnect, filename.c_str(), onlyFilename.c_str(), dwFlags,0) == TRUE);
}

bool FTPRequest::get(_STRING filename, _STRING localpath, DWORD mode)
{
	HINTERNET hRemote;
	HANDLE hLocal;
	DWORD dwRead,dwWritten,Prog;
	BOOL Result;
	
	const int bufferSize = 1000;
	char buf[bufferSize];

	// 서버의 파일을 오픈한다.
	hRemote = FtpOpenFile(hConnect, filename.c_str(),GENERIC_READ,mode,0);
	if (!hRemote) return false;

	// 로컬 파일을 만든다.
	_STRING localFullFilename(localpath);
	localFullFilename.append(filename);
	hLocal=CreateFile(localFullFilename.c_str(),GENERIC_WRITE,0,NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	Prog=0;
	while(TRUE) {
		Result=InternetReadFile(hRemote,buf,bufferSize,&dwRead);
		if ((Result==TRUE) && (dwRead==0)) break;
		WriteFile(hLocal,buf,dwRead,&dwWritten,NULL);
		Prog+=dwWritten;
	}
	CloseHandle(hLocal);
	InternetCloseHandle(hRemote);
	return true;
}


_STRING FTPRequest::dir()
{
	if(!isOpened()) return _T("Not Connected");

	HINTERNET hFind;
	WIN32_FIND_DATA wfd;
	BOOL bResult=TRUE;

	_STRING fileList(_T(""));
	hFind=FtpFindFirstFile(hConnect,_T("*.*"),&wfd,0,0);
	if (hFind) {
		fileList.append(wfd.cFileName);
		while (bResult) {
			bResult=InternetFindNextFile(hFind,&wfd);
			if (bResult) {
				fileList.append(_T("\n"));
				fileList.append(wfd.cFileName);
			}
		}
		InternetCloseHandle(hFind);
	}
	return fileList;
}

void FTPRequest::close()
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