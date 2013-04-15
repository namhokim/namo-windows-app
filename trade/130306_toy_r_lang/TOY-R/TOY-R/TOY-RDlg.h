
// TOY-RDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include <string>		// �߰��ڵ�
#include <vector>		// �߰��ڵ�

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

private:
	void ClearEditControl();	// ȭ�������
	void LoadFromFile();		// ���� �ҷ�����
	void ConvertToPrefix();		// ǥ��� ��ȯ(to prefix)
	void CreateIMCode();		// �߰��ڵ� ����

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
	CEdit m_program;
	CEdit m_convert;
	CEdit m_result;
	CString m_convert_content;
	std::vector<std::string> out_imc;
};
