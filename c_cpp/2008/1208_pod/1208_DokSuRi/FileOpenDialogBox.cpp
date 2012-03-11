#include "FileOpenDialogBox.h"


FileOpenDialogBox::FileOpenDialogBox(void)
: m_handel(NULL)
{
	memset(m_lpstrFile, 0, sizeof(_TCHAR) * MAX_PATH);

	memset(&m_OFN, 0, sizeof(OPENFILENAME));
	m_OFN.lStructSize = sizeof(OPENFILENAME);
	m_OFN.lpstrFilter=_T("PJL파일 (*.pjl;*.prn)\0*.pjl;*.prn\0모든 파일(*.*)\0*.*\0");
	m_OFN.nFilterIndex=1;
	/*m_OFN.lpstrCustomFilter=CustFilter;
	m_OFN.nMaxCustFilter=40;*/
	m_OFN.lpstrFile=m_lpstrFile;
	m_OFN.nMaxFile=MAX_PATH;
}

FileOpenDialogBox::~FileOpenDialogBox(void)
{
}

void FileOpenDialogBox::setWindowHandle(HWND handle)
{
	m_handel = handle;
	m_OFN.hwndOwner = m_handel;
}

_STRING  FileOpenDialogBox::showAndGetFilename()
{
	if(m_handel == NULL) return _T("");

	if (GetOpenFileName(&m_OFN)!=0) {
		return m_OFN.lpstrFile;
	}
	else {
		m_errorCode = CommDlgExtendedError();
		return _T("");
	}
}