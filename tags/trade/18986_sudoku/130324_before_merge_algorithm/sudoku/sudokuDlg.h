
// sudokuDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#define MAZE_SIZE 4		// �ִ�ũ�� 2X2�� 2��

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


private:
	// �ɹ��Դϴ�.
	HACCEL m_hAccelTable;
	CButton btn[MAZE_SIZE][MAZE_SIZE];	// ��ư �� ����/�������� ���� ��ü
	CButton m_ButtonUndo;	// �ǵ����� ��ư
	bool m_hasUndo;			// �ǵ����� ����

	// ���ο� �޼ҵ��Դϴ�.
	void ClearButtonValues();	// ��ư ���� �ʱ�ȭ(�� ���� ��)
	void LoadFromFile(LPCTSTR file);	// ���Ͽ��� �����͸� �ҷ���
	void ParseLine(LPCSTR line,CString& v1,CString& v2,CString& v3,CString& v4);	// �� ������ ���� ������
	void SetButtonValue(CButton&button, const CString& value);	// ��ư ���� ����
	void SaveToFile(LPCTSTR file);	// ��ư ���� ���Ϸ� ����

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();	
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);	// ����Ű�� �̿��ϱ� ���� ó��
	afx_msg void OnBnClickedButtonNew();	// �� ����
	afx_msg void OnBnClickedButtonSave();	// ���� �����ϱ�
	afx_msg void OnBnClickedButtonQuit();	// ������

	void OnChangeRadioMode();	// ���Ӹ�� �����
};
