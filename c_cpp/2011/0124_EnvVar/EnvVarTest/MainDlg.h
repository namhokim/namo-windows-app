// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_GET, BN_CLICKED, OnBnClickedButtonGet)
		COMMAND_HANDLER(IDC_BUTTON_SET, BN_CLICKED, OnBnClickedButtonSet)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	CMainDlg();
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonGet(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonSet(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	VOID InitListView();
	VOID InitItemsSize(WORD wWidth, WORD wHeight);
	VOID SetDataListView();

	// controls
	CListViewCtrl	m_list_env_var;
	CStatic			m_keyCap;
	CStatic			m_valueCap;
	CEdit			m_key;
	CEdit			m_value;
	CButton			m_btnGet;
	CButton			m_btnSet;

	// control positions
	POINT			m_mainDlgPtr;

	POINT			m_listViewPtr;

	POINT			m_keyCapPos;
	POINT			m_valueCapPos;
	POINT			m_btnGetPos;
	POINT			m_btnSetPos;
	RECT			m_keyRect;
	RECT			m_valueRect;
	
	// flags
	BOOL			m_control_init_ok;
	BOOL			m_is_first;
};
