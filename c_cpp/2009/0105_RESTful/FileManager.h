#pragma once
#include "stringUtil.h"
#include <windows.h>

class FileManager
{
public:
	FileManager(void);
	~FileManager(void);
	_STRING getFilenameByTime(_STRING ext=_T(""), _STRING prefixFilename=_T(""), _STRING postfixFilename=_T(""));
	bool saveToFile(_STRING filename, _STRING content);
	bool removeFile(_STRING filename);
	bool copyFile(_STRING origin, _STRING dest);
	bool open(_STRING filename, DWORD dwDesiredAccess=GENERIC_READ,
		DWORD dwShareMode=FILE_SHARE_READ | FILE_SHARE_WRITE,
		DWORD dwCreationDisposition=OPEN_EXISTING,
		DWORD dwFlagsAndAttributes=FILE_ATTRIBUTE_NORMAL);
	bool openA(std::string filename, DWORD dwDesiredAccess=GENERIC_READ,
		DWORD dwShareMode=FILE_SHARE_READ | FILE_SHARE_WRITE,
		DWORD dwCreationDisposition=OPEN_EXISTING,
		DWORD dwFlagsAndAttributes=FILE_ATTRIBUTE_NORMAL);
	void close();
	std::string readByString();
	bool writeFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
private:
	HANDLE hFile;
	_STRING getTimeString();
	bool isOpenedHandle()	{	return (hFile != INVALID_HANDLE_VALUE);	}
};
