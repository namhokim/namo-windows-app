#pragma once
#include "StringUtil.h"
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
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
