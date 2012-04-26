#include "stdafx.h"
#include "MainFrm.h"
#include "AddStateDlg.h"	// �����߰� ���̾�α� Ŭ����
#include "WindowPosition.h"		// ������ ��ġ ���
#include "NMWinBase.h"

#define IDR_SEL_CNT		202
#define IDR_SUM_TIME	203

#define	ID_TIMER_NO_HAS				0
#define	ID_TIMER_LOCK_WORKSTATION	1

#define	START_LOCK_DELAY	60		// 60��

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CreateSimpleStatusBar();
	// subclass the status bar as multipane
    m_status.SubclassWindow(m_hWndStatusBar);

	// set status bar panes. ID_DEFAULT_PANE is defined by WTL
    int arrPanes[] = { ID_DEFAULT_PANE, IDR_SEL_CNT, IDR_SUM_TIME };

	m_status.SetPanes(arrPanes, sizeof(arrPanes) / sizeof(int), false);

	// set status bar pane widths using local workaround
    int arrWidths[] = { 0, 160, 90 };
	SetPanelWidths(arrWidths, sizeof(arrWidths) / sizeof(int));

	// initialize status bar
	InitStatusBar();


	// �� ����
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	m_view.SetFont(AtlGetDefaultGuiFont());
	m_view.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);	// ���� ��ü ����
	m_view.InitListColumnName();							// �÷��� �ʱ�ȭ

	// ���� �ҷ�����
	SYSTEMTIME st;
	::GetLocalTime(&st);

	TCHAR filename[MAX_PATH];
	App::GetFilename(&st, filename);

	m_view.LoadFromFile(filename);

	// ���� �̺�Ʈ ���
	DWORD tick = ::GetTickCount();	// 49.7 days �� �Ǹ� �ٽ� 0���� �ʱ�ȭ
	if( tick< (2 *60 * 1000) )	// 2��
		m_view.InsertOneRecord(_T("����"), _T("�ý���"), &st);

	// ������Ʈ�� ����
	// ����� ������ ��ġ ����
	WindowPosition(this->m_hWnd, App_GUID, RESTORE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	// Lock the workstation Ÿ�̸� ����
	m_hTimer = SetTimer(ID_TIMER_LOCK_WORKSTATION, START_LOCK_DELAY*1000);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	
	// �������� ��ġ, ũ�� ������ ������Ʈ���� ���
	WindowPosition(this->m_hWnd, App_GUID, BACKUP);			// ������ ��ġ ����

	// TODO : ����
	// ���α׷� �ڵ����� ���θ� ������Ʈ���� ���

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnQueryEndSession(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// ���� �̺�Ʈ ���
	// ��¥ ����
	SYSTEMTIME st;
	::GetLocalTime(&st);

	// ����Ʈ�信 �Է� & ��������
	m_view.InsertOneRecord(_T("����"), _T("�ý���"), &st);

	// �������� ��ġ, ũ�� ������ ������Ʈ���� ���
	WindowPosition(this->m_hWnd, App_GUID, BACKUP);			// ������ ��ġ ����

	return 1;
}

// Ʈ���� ���� �̺�Ʈó��
LRESULT CMainFrame::OnTray(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	SetAndKillTimer();

	switch (lParam)
	{
	case WM_LBUTTONUP:	// @@@ ���� by Namho Kim 2008.05.28 (<- WM_LBUTTONDOWN)
	case WM_RBUTTONUP:	// @@@ ���� by Namho Kim 2008.05.28 (<- WM_RBUTTONDOWN)
		SetTray(this->m_hWnd, NULL, NIM_DELETE);
		break;
	}

	return 0;
}

LRESULT CMainFrame::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	switch(wParam) {
		case ID_TIMER_LOCK_WORKSTATION:
			SetAndKillTimer(TRUE);
			break;
	}

	return 0;
}

LRESULT CMainFrame::OnOpenExplore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// ���� .exe�� ��θ� ���Ѵ�.
	TCHAR szPath[MAX_PATH] = {0,};
	DWORD dwLength;
	dwLength = ::GetModuleFilePath(szPath, MAX_PATH);
	if(0==dwLength) {
		MessageBox(_T("��� ȹ�濡 �����Ͽ����ϴ�"), _T("����"), MB_OK | MB_ICONERROR);
		return 0;
	}

	// ShellExecute ����
	::ShellExecute(this->m_hWnd, _T("open"), szPath, NULL, NULL, SW_SHOW);

	return 0;
}

LRESULT CMainFrame::OnModifyByNotepad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// �������Ǵ� ���� ����
	SYSTEMTIME st;
	::GetLocalTime(&st);

	TCHAR filename[MAX_PATH];
	App::GetFilename(&st, filename);

	// ShellExecuteEx
	SHELLEXECUTEINFO execinfo;
	ZeroMemory( &execinfo, sizeof(execinfo) );
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = _T("open");
	execinfo.lpFile = _T("notepad.exe");
	execinfo.lpParameters = filename;
	execinfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;

	// ����
	//::ShellExecute(this->m_hWnd, _T("open"), _T("notepad.exe"), filename, NULL, SW_SHOW);
	int ret = (int)::ShellExecuteEx(&execinfo);
	if (ret == 0) return 1;
	
	// ���
	DWORD ec;
	do {
		// ����� �޽��� ó��(������ �����찡 ���� ���� �� ó�� ���̰� �ȴ�)
		MSG msg;
		if(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			BOOL bRet;
			bRet = ::GetMessage(&msg, NULL, 0, 0);	// ������ �޽��� ť�� ������� �޽����� ���� ������(���ο� �޽����� ������ ���� ���)

			if(bRet == -1)
			{
				ATLTRACE2(atlTraceUI, 0, _T("::GetMessage returned -1 (error)\n"));
				continue;   // error, don't process
			}
			else if(!bRet)
			{
				ATLTRACE2(atlTraceUI, 0, _T("CMessageLoop::Run - exiting\n"));
				break;   // WM_QUIT, exit message loop
			}

			if(!PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		} else {
			Sleep(1);	// �纸
		}

		// �޸��� ���Ῡ�� Ȯ��
		::GetExitCodeProcess(execinfo.hProcess, &ec);
	} while (ec == STILL_ACTIVE);

	// ���μ��� �ڵ� ����
	::CloseHandle(execinfo.hProcess);

	// �ٽ� �ε�
	m_view.DeleteAllItems();
	m_view.LoadFromFile(filename);

	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnStateAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetAndKillTimer();

	CAddStateDlg dlg(&m_view);
	return dlg.DoModal();
}

//LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	CAboutDlg dlg;
//	dlg.DoModal();
//	return 0;
//}

#include <StrSafe.h>
LRESULT CMainFrame::OnSetToTray(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Ÿ�̸� ����
	SetAndKillTimer();

	// tray�� icon (16x16 �����ܸ� Ʈ���̿� ����� ǥ�õȴ�)
	HICON hIcon16 = WTL::AtlLoadIcon(IDI_ICON_TRAY);

	// ���� �۾� ������
	int idx = this->m_view.GetItemCount()-1;

	TCHAR tipMsg[128]={0,};
	if(-1 != idx) {
		BSTR job=NULL;
		BSTR group=NULL;
		this->m_view.GetItemText(idx, 1, job);
		this->m_view.GetItemText(idx, 2, group);
		if(NULL!=job) {
			StringCchPrintf(tipMsg, 128, TEXT("�۾�:%s\n"), job);			
			::SysFreeString(job);
		}
		if(NULL!=job) {
			StringCchPrintf(tipMsg+_tcslen(tipMsg), 128-_tcslen(tipMsg), TEXT("�׷�:%s"), group);			
			::SysFreeString(job);
		}
	}
	SetTray(this->m_hWnd, hIcon16, NIM_ADD, tipMsg);

	return 0;
}

LRESULT CMainFrame::OnSelectAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int items = m_view.GetItemCount();

	for(int i=0; i<items; ++i) {
		m_view.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}

	// ���� ����
	OnListviewSelected(0);

	return 0;
}

LRESULT CMainFrame::OnClickAutorun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// ���� ���� Ȯ��
	//	üũ�� : ���� ON, ������Ʈ�� �߰�
	//	������ : ���� OFF, ������Ʈ�� ����

	// ���α׷� ���۽� ���� ���� & üũ
	return 0;
}

void CMainFrame::SetPanelWidths(int* arrWidths, int nPanes)
{ 
    // find the size of the borders
    int arrBorders[3];
    m_status.GetBorders(arrBorders);

    // calculate right edge of default pane (0)
    arrWidths[0] += arrBorders[2];
    for (int i = 1; i < nPanes; i++)
        arrWidths[0] += arrWidths[i];

    // calculate right edge of remaining panes (1 thru nPanes-1)
    for (int j = 1; j < nPanes; j++)
        arrWidths[j] += arrBorders[2] + arrWidths[j - 1];

    // set the pane widths
    m_status.SetParts(m_status.m_nPanes, arrWidths); 
}

VOID CMainFrame::InitStatusBar()
{ 
    // Pane 1: Display the date
    m_status.SetPaneText(IDR_SEL_CNT, TEXT(""));

    // Pane 2: Display the time
    m_status.SetPaneText(IDR_SUM_TIME, TEXT(""));
}

VOID CMainFrame::SetAndKillTimer(BOOL isLockWorkStation)
{
	if(m_hTimer==ID_TIMER_LOCK_WORKSTATION) {
		KillTimer(ID_TIMER_LOCK_WORKSTATION);
		m_hTimer = ID_TIMER_NO_HAS;

		if(isLockWorkStation) LockWorkStation();
	}
}

// [����Ʈ��]-[�߰����� ����] : ����Ʈ�� �ͺ�Ŭ�� or �˾��޴�
LRESULT CMainFrame::OnListviewSelected(LPNMHDR pnmh)
{
	CString selStr, sumStr;
	UINT selCnt = m_view.GetSelectedCountWithSum(sumStr);

	if(0==selCnt) {
		InitStatusBar();
	} else {
		// ���õ� �׸�
		selStr.Format(TEXT("%d ��ü�� ���õ�"), selCnt);
		m_status.SetPaneText(IDR_SEL_CNT, selStr);

		// ���õ� �׸��� �ð� �հ�
		m_status.SetPaneText(IDR_SUM_TIME, sumStr);
	}

	return 0;
}
