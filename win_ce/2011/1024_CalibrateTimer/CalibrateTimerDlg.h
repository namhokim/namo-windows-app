// CalibrateTimerDlg.h : ��� ����
//

#pragma once

// CCalibrateTimerDlg ��ȭ ����
class CCalibrateTimerDlg : public CDialog
{
// �����Դϴ�.
public:
	CCalibrateTimerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CALIBRATETIMER_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;
	INT   m_select;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioSleep();
	afx_msg void OnBnClickedRadioWaitForSingleObject();
	afx_msg void OnBnClickedButton();
};
