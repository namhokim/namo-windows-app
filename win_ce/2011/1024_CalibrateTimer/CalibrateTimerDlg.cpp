// CalibrateTimerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "CalibrateTimer.h"
#include "CalibrateTimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCalibrateTimerDlg 대화 상자

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


// CCalibrateTimerDlg 메시지 처리기

BOOL CCalibrateTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CheckRadioButton(IDC_RADIO_SLEEP, IDC_RADIO_WAIT_FOR_SINGLE_OBJECT,
		IDC_RADIO_SLEEP);
	m_select = IDC_RADIO_SLEEP;
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
	// 곱하고
	if(GetDlgItem(IDC_EDIT_CAL1)->GetWindowTextW(szBuf, 20)>0) {
		dwCalVal = _wtol(szBuf);
	} else {
		dwCalVal = 1;
	}
	dwWait = dwWait * dwCalVal;

	// 나누는
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
		// 대기중 메시지 처리(없으면 윈도우가 응답 없는 것 처럼 보이게 된다)
		MSG msg;
		if(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			BOOL bRet;
			bRet = ::GetMessage(&msg, NULL, 0, 0);	// 스레드 메시지 큐에 대기중인 메시지를 꺼내 가져옴(새로운 메시지가 없으면 무한 대기)

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
