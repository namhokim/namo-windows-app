// LauncherDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "LibProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLauncherDlg 대화 상자

CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CLauncherDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLauncherDlg 메시지 처리기

BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CString path(_T("\\Storage Card\\DlgArgTest.exe"));
	GetDlgItem(IDC_EDIT1)->SetWindowText(path);
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CLauncherDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_LAUNCHER_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_LAUNCHER_DIALOG));
	}
}
#endif


void CLauncherDlg::OnBnClickedButton1()
{
	CString path;
	GetDlgItem(IDC_EDIT1)->GetWindowText(path);
	process::Execute(path, 1);
}
