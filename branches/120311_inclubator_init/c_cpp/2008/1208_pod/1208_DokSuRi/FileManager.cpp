#include "FileManager.h"
#include <windows.h>

FileManager::FileManager(void)
{
}

FileManager::~FileManager(void)
{
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
	HANDLE hFile;
	hFile = CreateFile(filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);	// ¼û±èÆÄÀÏ·Î ¸¸µç´Ù
	if(INVALID_HANDLE_VALUE == hFile) return false;

	std::string outputContentMultiByte;
#ifdef _UNICODE
	namo::StringUtil su;
	outputContentMultiByte = su.wcs_to_mbs(content,std::locale("Korean"));
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