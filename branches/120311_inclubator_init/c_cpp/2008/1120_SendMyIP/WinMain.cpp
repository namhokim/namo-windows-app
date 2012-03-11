// ǥ�� ���̾�α� �����ڵ�
// ���̺귯�� �߰� �Ұ�(ComCtl32.lib)

#include "SendMail.h"
#include "IPInfo.h"
#include "Config.h"
#include <commctrl.h>
#include "resource.h"
#include "tray.h"

# pragma comment(lib, "ComCtl32.lib")

// �Լ� ������Ÿ��
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);	// Main dialog
void InitWindowHandle(HWND hDlg);
LRESULT CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
SYSTEMTIME addCurrentLocalTime(int addValue);

// �۷ι� ����(�ν��Ͻ�, ������ �ڵ�)
HINSTANCE g_hInst;
HICON hIcon;		// ������ ��
HWND hDlgMain;		// ���� ���̾�α� �ڵ�
HWND hReportList;	// ����Ʈ ����Ʈ �ڵ�
HANDLE hTimer;
HHOOK hhk;			// Ű���� ��
int servicePeriod;

// ���� ������ �Լ�
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
//	hAccel=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_MAIN), HWND_DESKTOP, MainDlgProc);
	
	return 0;
}

// ���� ������ �ݹ� �Լ�(�޽��� ó��)
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_INITDIALOG:
		InitWindowHandle(hDlg);	// ������ �ڵ� �ʱ�ȭ
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
		case WM_LBUTTONUP:	// @@@ ���� by Namho Kim 2008.05.28 (<- WM_LBUTTONDOWN)
		case WM_RBUTTONUP:	// @@@ ���� by Namho Kim 2008.05.28 (<- WM_RBUTTONDOWN)
			SetTray(hDlg, NULL, NIM_DELETE);
			break;
		}		
		break;
	case WM_KEYUP:
		switch (wParam) {
		case VK_ESCAPE:
			SetTray(hDlg, hIcon, NIM_ADD);	// @@@ �߰� by Namho Kim 2008.09.09
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

// ����� ������ �ڵ��� �ʱ�ȭ �Ѵ�.
// 1. ���̾�α� ���� ������ -> �������� ����
// 2. ��Ÿ ���� ��Ʈ�� �ڵ� ����
void InitWindowHandle(HWND hDlg) {
	hDlgMain = hDlg;	// �۷ι� ������ ���
	hhk = SetWindowsHookEx(WH_KEYBOARD, KeybdProc, NULL, GetCurrentThreadId());
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON_SINDO));	// icon �����´�.
	SendMessage(hDlg, WM_SETICON, TRUE, (LPARAM)hIcon);		// icon ����
	InitCommonControls();		// ���� ��Ʈ�� ���̺귯�� �ʱ�ȭ
	namo::Config config;
	if(config.isConfigFileOK() == FALSE) {
		//SetDlgItemText(hDlg, IDC_STATIC_INFO, _T("config.iniã�� �� ����"));
		MessageBox(hDlg, _T("\"config.ini\"�� ã�� �� �����ϴ�.\n���α׷��� �����մϴ�."), _T("�˸�"), MB_OK | MB_ICONEXCLAMATION);
		EndDialog(hDlg,0);
	}
	servicePeriod = config.getServicePeriod();
	// ���� ���� �ð��� ǥ���Ѵ�.
//	SetDlgItemText(hDlgMain, IDC_STATIC_INFO, staticMsg.c_str());
	hTimer = (HANDLE)SetTimer(hDlg, 1, servicePeriod,(TIMERPROC)TimerProc);
}

LRESULT CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0) return CallNextHookEx(hhk, code, wParam, lParam); // MS�� �Ծ�

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
	// ������ ������
	if(mail.sendMail(L"IP Info", info.getMyIP())) staticMsg.append(_T("����:"));
	else staticMsg.append(_T("����:"));

#ifdef _UNICODE
	staticMsg.append(mbs_to_wcs(mail.getLastMessage()));
#else
	staticMsg.append(mail.getLastMessage());
#endif
	SYSTEMTIME st = addCurrentLocalTime(servicePeriod);
	TCHAR displayMsg[256];
	wsprintf(displayMsg, _T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
	staticMsg.append(displayMsg);
	
	// ���� ���� �ð��� ǥ���Ѵ�.
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
