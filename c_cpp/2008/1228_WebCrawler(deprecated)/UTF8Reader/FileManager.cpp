#include "FileManager.h"
#include "UTF8Conv.h"

FileManager::FileManager(void)
:hFile(INVALID_HANDLE_VALUE)
{
}

FileManager::~FileManager(void)
{
	close();
}

_STRING FileManager::getFilenameByTime(_STRING ext, _STRING prefixFilename, _STRING postfixFilename)
{
	_STRING resultFilename(_T(""));
	if(!prefixFilename.empty()) resultFilename.append(prefixFilename);

	resultFilename.append(getTimeString().substr(2,13));

	if(!postfixFilename.empty()) resultFilename.append(postfixFilename);
	if(!ext.empty()) {
		resultFilename.append(_T("."));
		resultFilename.append(ext);
	}

	return resultFilename;
}


_STRING FileManager::getTimeString()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	_TCHAR sTime [MAX_PATH];
	wsprintf(sTime,_T("%d%02d%02d%02d%02d%02d%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return sTime;
}

bool FileManager::saveToFile(_STRING filename, _STRING content)
{
	open(filename, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN);
	if(INVALID_HANDLE_VALUE == hFile) return false;

	std::string outputContentMultiByte;
#ifdef _UNICODE
	namo::StringUtil su;
	outputContentMultiByte = su.wcs_to_mbs(content);
#else
		outputContentMultiByte = content;
#endif
	DWORD writtenLength;
	BOOL resWriteFile = WriteFile(hFile, 
		outputContentMultiByte.c_str(), (DWORD)content.size(), &writtenLength, NULL);
	
	CloseHandle(hFile);
	
	return (resWriteFile==TRUE);
}

bool FileManager::removeFile(_STRING filename)
{
	return (DeleteFile(filename.c_str())==TRUE);
}

bool FileManager::copyFile(_STRING origin, _STRING dest)
{
	return (CopyFile(origin.c_str(), dest.c_str(), TRUE) == TRUE);
}

bool FileManager::open(_STRING filename, DWORD dwDesiredAccess, DWORD dwShareMode,
					   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes)
{
	hFile = CreateFile(filename.c_str(), dwDesiredAccess, dwShareMode, NULL,
		dwCreationDisposition, dwFlagsAndAttributes, NULL);	// ¼û±èÆÄÀÏ·Î ¸¸µç´Ù
	return isOpenedHandle();
}

bool FileManager::writeFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
{
	if(!isOpenedHandle()) return false;

	return (WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL) == TRUE);
}

_STRING FileManager::readByString(DWORD codePage)
{
	UTF8_CONVERSION;
	if(!isOpenedHandle()) return TEXT("");
	const int BufferSize = 1024 * 10;
	BYTE byteBuffer[BufferSize];
	DWORD numberOfBytesRead;

	std::string retString("");
	BOOL res = ReadFile(hFile, byteBuffer, BufferSize - 1, &numberOfBytesRead, NULL);
	while(numberOfBytesRead != 0) {
		byteBuffer[numberOfBytesRead] = '\0';
		retString.append((CHAR *)byteBuffer);
		ReadFile(hFile, byteBuffer, BufferSize - 1, &numberOfBytesRead, NULL);
	}

	if(codePage == CP_UTF8) {
		_TCHAR *utf8 = UTF82T(retString.c_str());
		return utf8;
	}
	else {
#ifdef _UNICODE
	namo::StringUtil su;
	return su.mbs_to_wcs(retString);
#else
	return retString;
#endif
	}
}