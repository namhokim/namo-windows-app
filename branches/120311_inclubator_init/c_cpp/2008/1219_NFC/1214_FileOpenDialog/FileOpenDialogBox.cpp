#include "FileOpenDialogBox.h"


FileOpenDialogBox::FileOpenDialogBox(void)
: m_handel(NULL)
{
	memset(m_lpstrFile, 0, sizeof(_TCHAR) * MAX_PATH);

	memset(&m_OFN, 0, sizeof(OPENFILENAME));
	m_OFN.lStructSize = sizeof(OPENFILENAME);
	m_OFN.lpstrFilter=_T("모든 파일(*.*)\0*.*\0\0");
	m_OFN.nFilterIndex=1;
	/*m_OFN.lpstrCustomFilter=CustFilter;
	m_OFN.nMaxCustFilter=40;*/
	m_OFN.lpstrFile=m_lpstrFile;
	m_OFN.nMaxFile=MAX_PATH;

	// filter default
	//addFilter(_T("모든 파일(*.*)"), _T("*.*"));
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
	// 필터 설정
	/*_TCHAR filter[1024];
	memset(filter, 0, sizeof(_TCHAR)*1024);
	getStrFilter(filter);*/

	if (GetOpenFileName(&m_OFN)!=0) {
		return m_OFN.lpstrFile;
	}
	else {
		m_errorCode = CommDlgExtendedError();
		return _T("");
	}
}

//void FileOpenDialogBox::addFilter(_STRING description, _STRING ext)
//{
//	PAIR_OF_STRING pos;
//	pos.s1 = description;
//	pos.s2 = ext;
//	filterList.push_back(pos);
//}
//
//void FileOpenDialogBox::getStrFilter(_TCHAR *str)
//{
//	PAIR_OF_STRING pos;
//	while(!filterList.empty())
//	{
//		pos = filterList.end();
//		filterList.pop_back();
//		wcscat(str, pos.s1.c_str());
//	}
//}