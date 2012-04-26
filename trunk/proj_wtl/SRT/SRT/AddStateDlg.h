#pragma once
#include "resource.h"
#include <atlmisc.h>	// for CString
#include <atlddx.h>		// for DDX/DDV (atlmisc.h 아래에 있어야 함)
#include <atlctrls.h>	// for CButton,CEdit
#include "SRTView.h"	// 뷰

class CAddStateDlg : public CDialogImpl<CAddStateDlg>,  public CWinDataExchange<CAddStateDlg>
{
private:
	CSRTView *m_view_ref;	// 뷰에 대한 레퍼런스

	CButton m_radioCurr, m_radioSpec;
	CEdit	m_editJob;
	CDateTimePickerCtrl m_datePicker;

	CString	m_job, m_group;	// for DDX
	INT		m_radio;		// for DDX

public:
	// 생성자 : 뷰에 대한 접근을 할 수 있게 호출쪽에서 인자로 넘겨야 함
	CAddStateDlg(CSRTView *view);

	enum { IDD = IDD_DIALOG_ADD_STATE };

	// 메세지 맵
	BEGIN_MSG_MAP(CAddStateDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
		NOTIFY_HANDLER(IDC_DATETIMEPICKER, DTN_DATETIMECHANGE, OnDtnDatetimechangeDatetimepicker)
	END_MSG_MAP()

	// DDX맵
	BEGIN_DDX_MAP(CAddStateDlg)
		DDX_TEXT(IDC_EDIT_JOB,	m_job)
		DDX_TEXT(IDC_EDIT_GROUP,m_group)
		DDX_RADIO(IDC_RADIO_CURR_TIME, m_radio)
	END_DDX_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDtnDatetimechangeDatetimepicker(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
};
