// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include <string>
#include <map>
#include <Shellapi.h>
#include <Strsafe.h>

using namespace std;

// predefined lists
LPTSTR Operations[] = {_T("edit"), _T("explore"), _T("find"), _T("open"), _T("print"), _T("NULL"), NULL};
LPTSTR ShowCmds[] = {_T("SW_HIDE"), _T("SW_MAXIMIZE"), _T("SW_MINIMIZE"), _T("SW_RESTORE"),
					_T("SW_SHOW"), _T("SW_SHOWDEFAULT"), _T("SW_SHOWMAXIMIZED"), _T("SW_SHOWMINIMIZED"),
					_T("SW_SHOWMINNOACTIVE"), _T("SW_SHOWNA"), _T("SW_SHOWNOACTIVATE"), _T("SW_SHOWNORMAL"), NULL};

map<int, wstring> ShellExecuteErrMap;
map<wstring, int> ShowCmdMap;

void InitShellExecuteErrMap();
void InitShowCmdMap();

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	/// Init Dialog Items
	m_OperatorCombo	= GetDlgItem(IDC_COMBO_OP);
	m_FilePathEdit	= GetDlgItem(IDC_EDIT_FILE);
	m_ParamEdit		= GetDlgItem(IDC_EDIT_PARAM);
	m_DirEdit		= GetDlgItem(IDC_EDIT_DIR);
	m_ShowCmd		= GetDlgItem(IDC_COMBO_SW_CMD);
	m_OutputEdit	= GetDlgItem(IDC_EDIT_OUTPUT);

	/// Init Maps
	InitShellExecuteErrMap();
	InitShowCmdMap();

	// Operation Combo Init
	int i = 0;
	while(Operations[i]) {
		m_OperatorCombo.AddString(Operations[i]);
		++i;
	}
	m_OperatorCombo.SelectString(0, Operations[3]);	// Select 4th item	

	// Show Command
	i = 0;
	while(ShowCmds[i]) {
		m_ShowCmd.AddString(ShowCmds[i]);
		++i;
	}
	m_ShowCmd.SelectString(0, ShowCmds[5]);	// Select 6th item


	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// lpOperation
	TCHAR op[512] = {0,};
	LPTSTR lpOp = op;
	m_OperatorCombo.GetWindowText(op, 512);
	if( _tcscmp(op, _T("NULL"))==0 )	lpOp = NULL;	// NULL 비교

	// lpFile
	TCHAR file[MAX_PATH] = {0,};
	m_FilePathEdit.GetWindowText(file, MAX_PATH);

	// lpParameters
	TCHAR param[1024] = {0,};
	LPTSTR lpParam = param;
	m_ParamEdit.GetWindowText(param, 1024);
	if( _tcscmp(op, _T(""))==0 )	lpParam = NULL;		// 빈문자열 -> NULL

	// lpDirectory
	TCHAR dir[MAX_PATH] = {0,};
	LPTSTR lpDir = dir;
	m_DirEdit.GetWindowText(dir, MAX_PATH);
	if( _tcscmp(dir, _T(""))==0 )	lpDir = NULL;		// 빈문자열 -> NULL

	// nShowCmd
	TCHAR cmd[25] = {0,};
	m_ShowCmd.GetWindowText(cmd, 25);
	int nShow = SW_SHOWDEFAULT;
	map<wstring, int>::iterator p = ShowCmdMap.find(cmd);
	if(ShowCmdMap.end() != p) {
		nShow = (p->second);
	}


	// Shell Execute Info Structure
	SHELLEXECUTEINFO si;
	DWORD dwSize = sizeof(si);
	::ZeroMemory(&si, dwSize);
	si.cbSize		= dwSize;
	si.fMask		= SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC;
	si.hwnd			= this->m_hWnd;
	si.lpVerb		= lpOp;
	si.lpFile		= file;
	si.lpParameters	= lpParam;
	si.lpDirectory	= lpDir;
	si.nShow		= nShow;
	//si.hInstApp		// [out] : 성공시 32보다 큰 숫자로 set, 실패시 : SE_ERR_XXX 로 셋(eg.SE_ERR_FNF)
	//si.lpIDList
	//si.lpClass
	//si.hkeyClass
	//si.dwHotKey
	/// DUMMY UNION NAME
	//si.hIcon
	//si.hMonitor
	// si.hProcess		// [out] : 새로 시작된 애플리케이션의 핸들

	// Execute
	BOOL bResult = ::ShellExecuteEx(&si);

	const size_t BufSz = 1024;
	TCHAR buf[BufSz] = {0,};
	StringCchPrintf(buf, BufSz, TEXT("Run Result : %d (%s)"), bResult, (bResult ? TEXT("TRUE") : TEXT("FALSE")) );
	this->AddOutputText(buf);

	// Handling of Result
//WAIT_FOR_SYNC:
	this->AddOutputText(TEXT("WaitForSingleObject(SHELLEXECUTEINFO.hProcess,INFINITE);"));
	DWORD dwWaitRes = ::WaitForSingleObject(si.hProcess, 5000);

	if(WAIT_FAILED==dwWaitRes) {
		dwWaitRes = ::GetLastError();
//		if(6==dwWaitRes) goto WAIT_FOR_SYNC;
	} else {
		TerminateProcess(si.hProcess, 0);
	}
	StringCchPrintf(buf, BufSz, TEXT("Wait Result : %d"), dwWaitRes);
	this->AddOutputText(buf);

	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog fileOpen(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("All Files(*.*)\0*.*\0Executable File(*.exe)\0*.exe\0PDF File(*.pdf)\0*pdf\0"));

	if(IDOK == fileOpen.DoModal()) {	// 파일이 선택되었으면
		// 선택한 파일의 값을 콘트롤에 반영한다
		GetDlgItem(IDC_EDIT_FILE).SetWindowText(fileOpen.m_szFileName);
	}

	return 0;
}

// 현재 시간
inline void AppendCurrentTime(__out std::tstring& szBuf)
{
	SYSTEMTIME st;
	::GetLocalTime(&st);

	const size_t BufSz = 12+1;	// eg. "11:59:35.046"
	TCHAR buf[BufSz] = {0};
	HRESULT hRes = StringCchPrintf(buf, BufSz, TEXT("%02d:%02d:%02d.%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	szBuf.append(buf);
}

// 출력 메시지 창의 앞에 붙일 헤더 문자열을 붙임
inline void AppendHeader(__out std::tstring& szBuf)
{
	szBuf.append(TEXT(">"));
	AppendCurrentTime(szBuf);
	szBuf.append(TEXT(" - "));
}

void CMainDlg::AddOutputText(LPTSTR szText)
{
	AppendHeader(m_outputMsg);
	m_outputMsg.append(szText);
	m_outputMsg.append(TEXT("\r\n"));
	m_OutputEdit.SetWindowText(m_outputMsg.c_str());
}

LRESULT CMainDlg::OnNMClickSyslinkMsdn(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	PNMLINK p = (PNMLINK)pNMHDR;
	ShellExecuteW(0, L"open", L"iexplore.exe", p->item.szUrl, NULL, SW_SHOWNORMAL);

	return 0;
}

#define	SH_EMAP(K,V)	ShellExecuteErrMap.insert(pair<int, wstring>((K), (V)))

void InitShellExecuteErrMap()
{
	// 비어있을 경우에만 초기화(한번만 수행)
	if(!ShellExecuteErrMap.empty()) return;

	//ShellExecuteErrMap.insert(pair<int, wstring>(0, L"The operating system is out of memory or resources."));
	SH_EMAP(0, L"The operating system is out of memory or resources.");
	SH_EMAP(ERROR_FILE_NOT_FOUND, L"The specified file was not found.");
	SH_EMAP(ERROR_PATH_NOT_FOUND, L"The specified path was not found.");
	SH_EMAP(ERROR_BAD_FORMAT, L"The .exe file is invalid (non-Win32 .exe or error in .exe image).");
	SH_EMAP(SE_ERR_ACCESSDENIED, L"The operating system denied access to the specified file.");
	SH_EMAP(SE_ERR_ASSOCINCOMPLETE, L"The file name association is incomplete or invalid.");
	SH_EMAP(SE_ERR_DDEBUSY, L"The DDE transaction could not be completed because other DDE transactions were being processed.");
	SH_EMAP(SE_ERR_DDEFAIL, L"The DDE transaction failed.");
	SH_EMAP(SE_ERR_DDETIMEOUT, L"The DDE transaction could not be completed because the request timed out.");
	SH_EMAP(SE_ERR_DLLNOTFOUND, L"The specified DLL was not found.");
	SH_EMAP(SE_ERR_FNF, L"The specified file was not found.");
	SH_EMAP(SE_ERR_NOASSOC, L"There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable.");
	SH_EMAP(SE_ERR_OOM, L"There was not enough memory to complete the operation.");
	SH_EMAP(SE_ERR_PNF, L"The specified path was not found.");
	SH_EMAP(SE_ERR_SHARE, L"A sharing violation occurred.");
}

#define	SH_CMAP(I)	ShowCmdMap.insert( pair<wstring, int>( (L#I), (I)) )

void InitShowCmdMap()
{
	if(!ShowCmdMap.empty()) return;

	//ShowCmdMap.insert( pair<wstring, int>(L"SW_HIDE", SW_HIDE) );
	SH_CMAP(SW_HIDE);
	SH_CMAP(SW_MAXIMIZE);
	SH_CMAP(SW_MINIMIZE);
	SH_CMAP(SW_RESTORE);
	SH_CMAP(SW_SHOW);
	SH_CMAP(SW_SHOWDEFAULT);
	SH_CMAP(SW_SHOWMAXIMIZED);
	SH_CMAP(SW_SHOWMINIMIZED);
	SH_CMAP(SW_SHOWMINNOACTIVE);
	SH_CMAP(SW_SHOWNA);
	SH_CMAP(SW_SHOWNOACTIVATE);
	SH_CMAP(SW_SHOWNORMAL);
}