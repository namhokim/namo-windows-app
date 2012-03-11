#pragma once
#include "StringUtil.h"
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif	/* WIN32_LEAN_AND_MEAN */
#include <windows.h>	// for HWND
#include <commdlg.h>	// for OPENFILENAME

class FileOpenDialogBox
{
public:
	FileOpenDialogBox(void);
	~FileOpenDialogBox(void);
	void setWindowHandle(HWND handle);
	_STRING showAndGetFilename();
	DWORD getErrorCode()				{	return m_errorCode;	}
private:
	HWND m_handel;
	OPENFILENAME m_OFN;
	_TCHAR m_lpstrFile[MAX_PATH];
	DWORD m_errorCode;
};
