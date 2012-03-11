// CalibrateTimerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "CalibrateTimer.h"
#include "CalibrateTimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCalibrateTimerDlg ��ȭ ����

CCalibrateTimerDlg::CCalibrateTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibrateTimerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalibrateTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCalibrateTimerDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_SLEEP, &CCalibrateTimerDlg::OnBnClickedRadioSleep)
	ON_BN_CLICKED(IDC_RADIO_WAIT_FOR_SINGLE_OBJECT, &CCalibrateTimerDlg::OnBnClickedRadioWaitForSingleObject)
	ON_BN_CLICKED(IDC_BUTTON, &CCalibrateTimerDlg::OnBnClickedButton)
END_MESSAGE_MAP()


// CCalibrateTimerDlg �޽��� ó����

BOOL CCalibrateTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CheckRadioButton(IDC_RADIO_SLEEP, IDC_RADIO_WAIT_FOR_SINGLE_OBJECT,
		IDC_RADIO_SLEEP);
	m_select = IDC_RADIO_SLEEP;
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CCalibrateTimerDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_CALIBRATETIMER_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_CALIBRATETIMER_DIALOG));
	}
}
#endif


void CCalibrateTimerDlg::OnBnClickedRadioSleep()
{
	m_select = IDC_RADIO_SLEEP;
}

void CCalibrateTimerDlg::OnBnClickedRadioWaitForSingleObject()
{
	m_select = IDC_RADIO_WAIT_FOR_SINGLE_OBJECT;
}

DWORD GetDiffTime(
	__in LPSYSTEMTIME lpST1,
	__in LPSYSTEMTIME lpST2)
{
	DWORD dw1, dw2;

	dw1 = (lpST1->wHour*3600 + lpST1->wMinute*60 + lpST1->wSecond)*1000 + lpST1->wMilliseconds;
	dw2 = (lpST2->wHour*3600 + lpST2->wMinute*60 + lpST2->wSecond)*1000 + lpST2->wMilliseconds;

	return dw2-dw1;
}

void CCalibrateTimerDlg::OnBnClickedButton()
{
	const int Repeat = 10;

	DWORD dwWait = 1000;
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	SYSTEMTIME st1, st2;
	DWORD dwSum, dwAvg, dwCalVal;

	WCHAR szBuf[20]={0};
	// ���ϰ�
	if(GetDlgItem(IDC_EDIT_CAL1)->GetWindowTextW(szBuf, 20)>0) {
		dwCalVal = _wtol(szBuf);
	} else {
		dwCalVal = 1;
	}
	dwWait = dwWait * dwCalVal;

	// ������
	if(GetDlgItem(IDC_EDIT_CAL2)->GetWindowTextW(szBuf, 20)>0) {
		dwCalVal = _wtol(szBuf);
	} else {
		dwCalVal = 1;
	}
	dwWait = dwWait / dwCalVal;

	CWnd* s(GetDlgItem(IDC_STATIC_OUT));
	CString msg;
	dwSum=0;
	for(int i=1; i<=10; ++i) {
		msg.Format(_T("Try - %i"), i);
		s->SetWindowText(msg);
		// ����� �޽��� ó��(������ �����찡 ���� ���� �� ó�� ���̰� �ȴ�)
		MSG msg;
		if(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			BOOL bRet;
			bRet = ::GetMessage(&msg, NULL, 0, 0);	// ������ �޽��� ť�� ������� �޽����� ���� ������(���ο� �޽����� ������ ���� ���)

			if(bRet == -1)
			{
				continue;   // error, don't process
			}
			else if(!bRet)
			{
				break;   // WM_QUIT, exit message loop
			}

			if(!PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		switch(m_select) {
			case IDC_RADIO_SLEEP:
				GetLocalTime(&st1);
				Sleep(dwWait);
				GetLocalTime(&st2);
				dwSum += GetDiffTime(&st1, &st2);
				break;
			case IDC_RADIO_WAIT_FOR_SINGLE_OBJECT:
				GetLocalTime(&st1);
				WaitForSingleObject(hEvent, dwWait);
				GetLocalTime(&st2);
				dwSum += GetDiffTime(&st1, &st2);
				break;
		}
	}

	dwAvg = dwSum / Repeat;
	msg.Format(_T("Average : %u"), dwAvg);
	s->SetWindowText(msg);

	CloseHandle(hEvent);
}
