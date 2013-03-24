#pragma once
#include "resource.h"
#include <atlmisc.h>	// for CString
#include <atlddx.h>		// for DDX/DDV (atlmisc.h �Ʒ��� �־�� ��)
#include <atlctrls.h>	// for CButton,CEdit
#include "SRTView.h"	// ��

class CAddStateDlg : public CDialogImpl<CAddStateDlg>,  public CWinDataExchange<CAddStateDlg>
{
private:
	CSRTView *m_view_ref;	// �信 ���� ���۷���

	CButton m_radioCurr, m_radioSpec;
	CEdit	m_editJob;
	CDateTimePickerCtrl m_datePicker;

	CString	m_job, m_group;	// for DDX
	INT		m_radio;		// for DDX

public:
	// ������ : �信 ���� ������ �� �� �ְ� ȣ���ʿ��� ���ڷ� �Ѱܾ� ��
	CAddStateDlg(CSRTView *view);

	enum { IDD = IDD_DIALOG_ADD_STATE };

	// �޼��� ��
	BEGIN_MSG_MAP(CAddStateDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancelCmd)
		NOTIFY_HANDLER(IDC_DATETIMEPICKER, DTN_DATETIMECHANGE, OnDtnDatetimechangeDatetimepicker)
	END_MSG_MAP()

	// DDX��
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
