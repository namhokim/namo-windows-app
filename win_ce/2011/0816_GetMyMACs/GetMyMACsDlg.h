// GetMyMACsDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

// CGetMyMACsDlg ��ȭ ����
class CGetMyMACsDlg : public CDialog
{
// �����Դϴ�.
public:
	CGetMyMACsDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GETMYMACS_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_out;
	afx_msg void OnBnClickedButtonGet();
};
