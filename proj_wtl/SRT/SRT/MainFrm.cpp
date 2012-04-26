#include "stdafx.h"
#include "MainFrm.h"
#include "AddStateDlg.h"	// 상태추가 다이얼로그 클래스
#include "WindowPosition.h"		// 윈도우 위치 기억
#include "NMWinBase.h"

#define IDR_SEL_CNT		202
#define IDR_SUM_TIME	203

#define	ID_TIMER_NO_HAS				0
#define	ID_TIMER_LOCK_WORKSTATION	1

#define	START_LOCK_DELAY	60		// 60초

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


	// 뷰 생성
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	m_view.SetFont(AtlGetDefaultGuiFont());
	m_view.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);	// 한행 전체 선택
	m_view.InitListColumnName();							// 컬럼명 초기화

	// 파일 불러오기
	SYSTEMTIME st;
	::GetLocalTime(&st);

	TCHAR filename[MAX_PATH];
	App::GetFilename(&st, filename);

	m_view.LoadFromFile(filename);

	// 시작 이벤트 기록
	DWORD tick = ::GetTickCount();	// 49.7 days 가 되면 다시 0으로 초기화
	if( tick< (2 *60 * 1000) )	// 2분
		m_view.InsertOneRecord(_T("시작"), _T("시스템"), &st);

	// 레지스트리 복원
	// 저장된 윈도우 위치 복원
	WindowPosition(this->m_hWnd, App_GUID, RESTORE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	// Lock the workstation 타이머 가동
	m_hTimer = SetTimer(ID_TIMER_LOCK_WORKSTATION, START_LOCK_DELAY*1000);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	
	// 윈도우의 위치, 크기 정보를 레지스트리에 기록
	WindowPosition(this->m_hWnd, App_GUID, BACKUP);			// 윈도우 위치 저장

	// TODO : 구현
	// 프로그램 자동시작 여부를 레지스트리에 기록

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
	// 종료 이벤트 기록
	// 날짜 정보
	SYSTEMTIME st;
	::GetLocalTime(&st);

	// 리스트뷰에 입력 & 파일저장
	m_view.InsertOneRecord(_T("종료"), _T("시스템"), &st);

	// 윈도우의 위치, 크기 정보를 레지스트리에 기록
	WindowPosition(this->m_hWnd, App_GUID, BACKUP);			// 윈도우 위치 저장

	return 1;
}

// 트레이 관련 이벤트처리
LRESULT CMainFrame::OnTray(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	SetAndKillTimer();

	switch (lParam)
	{
	case WM_LBUTTONUP:	// @@@ 수정 by Namho Kim 2008.05.28 (<- WM_LBUTTONDOWN)
	case WM_RBUTTONUP:	// @@@ 수정 by Namho Kim 2008.05.28 (<- WM_RBUTTONDOWN)
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
	// 현개 .exe의 경로를 구한다.
	TCHAR szPath[MAX_PATH] = {0,};
	DWORD dwLength;
	dwLength = ::GetModuleFilePath(szPath, MAX_PATH);
	if(0==dwLength) {
		MessageBox(_T("경로 획득에 실패하였습니다"), _T("에러"), MB_OK | MB_ICONERROR);
		return 0;
	}

	// ShellExecute 실행
	::ShellExecute(this->m_hWnd, _T("open"), szPath, NULL, NULL, SW_SHOW);

	return 0;
}

LRESULT CMainFrame::OnModifyByNotepad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 현재수행되는 파일 열기
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

	// 실행
	//::ShellExecute(this->m_hWnd, _T("open"), _T("notepad.exe"), filename, NULL, SW_SHOW);
	int ret = (int)::ShellExecuteEx(&execinfo);
	if (ret == 0) return 1;
	
	// 대기
	DWORD ec;
	do {
		// 대기중 메시지 처리(없으면 윈도우가 응답 없는 것 처럼 보이게 된다)
		MSG msg;
		if(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			BOOL bRet;
			bRet = ::GetMessage(&msg, NULL, 0, 0);	// 스레드 메시지 큐에 대기중인 메시지를 꺼내 가져옴(새로운 메시지가 없으면 무한 대기)

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
			Sleep(1);	// 양보
		}

		// 메모장 종료여부 확인
		::GetExitCodeProcess(execinfo.hProcess, &ec);
	} while (ec == STILL_ACTIVE);

	// 프로세스 핸들 닫음
	::CloseHandle(execinfo.hProcess);

	// 다시 로드
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
	// 타이머 제거
	SetAndKillTimer();

	// tray용 icon (16x16 아이콘만 트레이에 제대로 표시된다)
	HICON hIcon16 = WTL::AtlLoadIcon(IDI_ICON_TRAY);

	// 현재 작업 가져옴
	int idx = this->m_view.GetItemCount()-1;

	TCHAR tipMsg[128]={0,};
	if(-1 != idx) {
		BSTR job=NULL;
		BSTR group=NULL;
		this->m_view.GetItemText(idx, 1, job);
		this->m_view.GetItemText(idx, 2, group);
		if(NULL!=job) {
			StringCchPrintf(tipMsg, 128, TEXT("작업:%s\n"), job);			
			::SysFreeString(job);
		}
		if(NULL!=job) {
			StringCchPrintf(tipMsg+_tcslen(tipMsg), 128-_tcslen(tipMsg), TEXT("그룹:%s"), group);			
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

	// 선택 갱신
	OnListviewSelected(0);

	return 0;
}

LRESULT CMainFrame::OnClickAutorun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 상태 변수 확인
	//	체크시 : 변수 ON, 레지스트리 추가
	//	해제시 : 변수 OFF, 레지스트리 삭제

	// 프로그램 시작시 변수 세팅 & 체크
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

// [리스트뷰]-[추가정보 편집] : 리스트뷰 터블클릭 or 팝업메뉴
LRESULT CMainFrame::OnListviewSelected(LPNMHDR pnmh)
{
	CString selStr, sumStr;
	UINT selCnt = m_view.GetSelectedCountWithSum(sumStr);

	if(0==selCnt) {
		InitStatusBar();
	} else {
		// 선택된 항목
		selStr.Format(TEXT("%d 개체가 선택됨"), selCnt);
		m_status.SetPaneText(IDR_SEL_CNT, selStr);

		// 선택된 항목의 시간 합계
		m_status.SetPaneText(IDR_SUM_TIME, sumStr);
	}

	return 0;
}
