// GetMyMACsDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GetMyMACs.h"
#include "GetMyMACsDlg.h"
#include "NetworkUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGetMyMACsDlg 대화 상자

CGetMyMACsDlg::CGetMyMACsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetMyMACsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetMyMACsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OUT, m_out);
}

BEGIN_MESSAGE_MAP(CGetMyMACsDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GET, &CGetMyMACsDlg::OnBnClickedButtonGet)
END_MESSAGE_MAP()


// CGetMyMACsDlg 메시지 처리기

BOOL CGetMyMACsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CGetMyMACsDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_GETMYMACS_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_GETMYMACS_DIALOG));
	}
}
#endif


// Sample : MAC 주소 가져오는 부분
void CGetMyMACsDlg::OnBnClickedButtonGet()
{
	
	/*
	if(GetMyMACs(str)) {
		m_out.SetWindowTextW(str);
	}*/
	/*CWinApp *pWinApp = AfxGetApp();
	str = pWinApp->GetProfileString(_T("URL"), _T("Entry"), _T("Value Default"));
	m_out.SetWindowTextW(str);
	pWinApp->WriteProfileString(_T("URL"), _T("Entry"), _T("Value"));*/

	// 자신 파일명(절대경로)
	TCHAR me[MAX_PATH] = {0};
	GetModuleFileName(NULL, me, MAX_PATH);

	// 파일 버전 정보 크기
	CString str;
	DWORD dwHandle, dwSize;
	dwSize = GetFileVersionInfoSize(me, &dwHandle);
	if(0!=dwSize) {
		LPVOID pData = malloc(dwSize);
		if(GetFileVersionInfo(me, dwHandle, dwSize, pData)) {
			VS_FIXEDFILEINFO *pFileInfo;
			UINT BufLen;
			if(VerQueryValue(pData, _T("\\"), (LPVOID *)&pFileInfo, (PUINT)&BufLen ) ) {
				str.Format(_T("%d.%d.%d.%d"),
					HIWORD(pFileInfo->dwFileVersionMS),
					LOWORD(pFileInfo->dwFileVersionMS),
					HIWORD(pFileInfo->dwFileVersionLS),
					LOWORD(pFileInfo->dwFileVersionLS));
			}
		}
		free(pData);
	}
	m_out.SetWindowTextW(str);
}
