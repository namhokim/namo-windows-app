// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "SRTView.h"	// 뷰
#include "aboutdlg.h"	// 정보 다이얼로그
#include <atlctrlx.h>	// 상태바
#include <atlcrack.h>	// for NOTIFY_CODE_HANDLER_EX

#include "resource.h"
#include "tray.h"				// 트레이 관련

extern LPCTSTR App_GUID;

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
private:
	VOID InitStatusBar();
	UINT_PTR m_hTimer;
	VOID SetAndKillTimer(BOOL isLockWorkStation=FALSE);
public:
	DECLARE_FRAME_WND_CLASS(App_GUID, IDR_MAINFRAME)

	CMultiPaneStatusBarCtrl m_status;
	CSRTView m_view;

	void setHandle(HWND handle) {
		m_hWnd = handle;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_QUERYENDSESSION, OnQueryEndSession)	// 다른 종료요청 메시지
		MESSAGE_HANDLER(TRAY_NOTIFY, OnTray)			// 트레이 관련 이벤트 핸들러
		MESSAGE_HANDLER(WM_TIMER, OnTimer)

		COMMAND_ID_HANDLER(ID_FILE_EXPLORE_OPEN, OnOpenExplore)
		COMMAND_ID_HANDLER(ID_FILE_MODIFY, OnModifyByNotepad)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_STATE_ADD, OnStateAdd)
//		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_TRAY_ADD, OnSetToTray)
		COMMAND_ID_HANDLER(ID_SEL_ALL, OnSelectAll)
		COMMAND_ID_HANDLER(ID_OPTION_AUTORUN, OnClickAutorun)

		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)

		// 마우스 이벤트
		NOTIFY_CODE_HANDLER_EX(NM_CLICK, OnListviewSelected)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnQueryEndSession(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnTray(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnOpenExplore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnModifyByNotepad(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStateAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
//	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetToTray(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSelectAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClickAutorun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void	SetPanelWidths(int* arrWidths, int nPanes);
	LRESULT CMainFrame::OnListviewSelected(LPNMHDR pnmh);
};
