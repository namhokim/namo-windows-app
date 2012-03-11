// GetMyMACsDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "GetMyMACs.h"
#include "GetMyMACsDlg.h"
#include "NetworkUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGetMyMACsDlg ��ȭ ����

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


// CGetMyMACsDlg �޽��� ó����

BOOL CGetMyMACsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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


// Sample : MAC �ּ� �������� �κ�
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

	// �ڽ� ���ϸ�(������)
	TCHAR me[MAX_PATH] = {0};
	GetModuleFileName(NULL, me, MAX_PATH);

	// ���� ���� ���� ũ��
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
