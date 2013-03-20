
// sudokuDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#define MAZE_SIZE 4

// CsudokuDlg ��ȭ ����
class CsudokuDlg : public CDialog
{
// �����Դϴ�.
public:
	CsudokuDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SUDOKU_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �ɹ��Դϴ�.
private:
	void LoadFromFile(LPCTSTR file);
	void ParseLine(LPCSTR line,CString& v1,CString& v2,CString& v3,CString& v4);
	void SetButtonValue(CButton&button, const CString& value);
	void ClearButtonValues();

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonNew();
	HACCEL m_hAccelTable;
	BOOL PreTranslateMessage(MSG* pMsg);
	CButton btn[MAZE_SIZE][MAZE_SIZE];
};
