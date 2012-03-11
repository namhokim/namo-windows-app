// 표준 다이얼로그 기초코드
// 라이브러리 추가 할것(ComCtl32.lib)

#include "SendMail.h"
#include "IPInfo.h"
#include "Config.h"
#include <commctrl.h>
#include "resource.h"
#include "tray.h"

# pragma comment(lib, "ComCtl32.lib")

// 함수 프로토타입
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);	// Main dialog
void InitWindowHandle(HWND hDlg);
LRESULT CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
SYSTEMTIME addCurrentLocalTime(int addValue);

// 글로번 변수(인스턴스, 윈도우 핸들)
HINSTANCE g_hInst;
HICON hIcon;		// 아이콘 용
HWND hDlgMain;		// 메인 다이얼로그 핸들
HWND hReportList;	// 리포트 리스트 핸들
HANDLE hTimer;
HHOOK hhk;			// 키보드 훅
int servicePeriod;

// 시작 윈도우 함수
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
//	hAccel=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_MAIN), HWND_DESKTOP, MainDlgProc);
	
	return 0;
}

// 메인 윈도우 콜백 함수(메시지 처리)
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_INITDIALOG:
		InitWindowHandle(hDlg);	// 윈도우 핸들 초기화
		return FALSE;
	case WM_DESTROY:
		SetTray(hDlg, NULL, NIM_DELETE);
		break;
	case WM_CLOSE:
			KillTimer(hDlg,1);
			EndDialog(hDlg,0);
			break;
	case TRAY_NOTIFY:
		switch (lParam)
		{
		case WM_LBUTTONUP:	// @@@ 수정 by Namho Kim 2008.05.28 (<- WM_LBUTTONDOWN)
		case WM_RBUTTONUP:	// @@@ 수정 by Namho Kim 2008.05.28 (<- WM_RBUTTONDOWN)
			SetTray(hDlg, NULL, NIM_DELETE);
			break;
		}		
		break;
	case WM_KEYUP:
		switch (wParam) {
		case VK_ESCAPE:
			SetTray(hDlg, hIcon, NIM_ADD);	// @@@ 추가 by Namho Kim 2008.09.09
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		//case IDC_BUTTON_SVC_START:
		//	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_SVC_STOP), TRUE);
		//	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_SVC_START), FALSE);
		//	StartSendIPService(TRUE);
		//	break;
		//case IDC_BUTTON_SVC_STOP:
		//	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_SVC_START), TRUE);
		//	EnableWindow(GetDlgItem(hDlg,IDC_BUTTON_SVC_STOP), FALSE);
		//	StartSendIPService(FALSE);
		//	break;
		case IDCLOSE:
			EndDialog(hDlg,0);
			break;
		}
		return FALSE;
	}
	return FALSE;
}
// GetDlgItem(hDlg,IDC_STATIC_INFO)

// 사용할 윈도우 핸들을 초기화 한다.
// 1. 다이얼로그 메인 윈도우 -> 전역변수 지정
// 2. 기타 공통 컨트롤 핸들 구함
void InitWindowHandle(HWND hDlg) {
	hDlgMain = hDlg;	// 글로벌 변수로 등록
	hhk = SetWindowsHookEx(WH_KEYBOARD, KeybdProc, NULL, GetCurrentThreadId());
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON_SINDO));	// icon 가져온다.
	SendMessage(hDlg, WM_SETICON, TRUE, (LPARAM)hIcon);		// icon 부착
	InitCommonControls();		// 공통 컨트롤 라이브러리 초기화
	namo::Config config;
	if(config.isConfigFileOK() == FALSE) {
		//SetDlgItemText(hDlg, IDC_STATIC_INFO, _T("config.ini찾을 수 없음"));
		MessageBox(hDlg, _T("\"config.ini\"를 찾을 수 없습니다.\n프로그램을 종료합니다."), _T("알림"), MB_OK | MB_ICONEXCLAMATION);
		EndDialog(hDlg,0);
	}
	servicePeriod = config.getServicePeriod();
	// 다음 보낼 시간을 표시한다.
//	SetDlgItemText(hDlgMain, IDC_STATIC_INFO, staticMsg.c_str());
	hTimer = (HANDLE)SetTimer(hDlg, 1, servicePeriod,(TIMERPROC)TimerProc);
}

LRESULT CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0) return CallNextHookEx(hhk, code, wParam, lParam); // MS의 규약

	if(lParam & 0x80000000) SendMessage(hDlgMain,WM_KEYUP,	wParam,lParam);
	else					SendMessage(hDlgMain,WM_KEYDOWN,wParam,lParam);
	return CallNextHookEx(hhk, code, wParam, lParam);
}

#include "StringUtil.h"
void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	namo::SendMail mail;
	IPInfo info;

	_STRING staticMsg(_T(""));
	// 메일을 보내고
	if(mail.sendMail(L"IP Info", info.getMyIP())) staticMsg.append(_T("성공:"));
	else staticMsg.append(_T("실패:"));

#ifdef _UNICODE
	staticMsg.append(mbs_to_wcs(mail.getLastMessage()));
#else
	staticMsg.append(mail.getLastMessage());
#endif
	SYSTEMTIME st = addCurrentLocalTime(servicePeriod);
	TCHAR displayMsg[256];
	wsprintf(displayMsg, _T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
	staticMsg.append(displayMsg);
	
	// 다음 보낼 시간을 표시한다.
	SetDlgItemText(hDlgMain, IDC_STATIC_INFO, staticMsg.c_str());
}

INT64 FILETIMEtoINT64(const LPFILETIME filetime)
{
	INT64 _i64 = 0;
	_i64 = filetime->dwHighDateTime;
	_i64 = _i64 << 32;
	_i64 = _i64 | filetime->dwLowDateTime;
	return _i64;
}

FILETIME INT64toFILETIME(const PINT64 int64)
{
	FILETIME filetime;
	filetime.dwHighDateTime = (DWORD)(*int64 >> 32);
	filetime.dwLowDateTime  = (DWORD)(*int64 & 0xffffffff);
	return filetime;
}

SYSTEMTIME addCurrentLocalTime(int addMilliseconds)
{
	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);

	FILETIME currentFileTime, addedFileTime;
	SystemTimeToFileTime(&currentTime, &currentFileTime);

	INT64 addedTime = FILETIMEtoINT64(&currentFileTime) + (addMilliseconds * 10000);
	addedFileTime = INT64toFILETIME(&addedTime);

	SYSTEMTIME st;
	FileTimeToSystemTime(&addedFileTime, &st);
	
	return st;
}
