
// TOY-RDlg.h : ��� ����
//

#pragma once


// CTOYRDlg ��ȭ ����
class CTOYRDlg : public CDialog
{
// �����Դϴ�.
public:
	CTOYRDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TOYR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonLoadProg();
	afx_msg void OnBnClickedButtonPrefix();
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoadIc();
	afx_msg void OnBnClickedButtonCalc();
	afx_msg void OnBnClickedButtonQuit();
};