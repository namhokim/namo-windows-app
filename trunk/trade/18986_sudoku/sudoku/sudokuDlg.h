
// sudokuDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "../algorithm/SudokuSolver.h"

#define MAZE_SIZE 4		// �ִ�ũ�� 2X2�� 2��
#define NOT_SELECTED	(-1)	// ��ǥ ������ ���� �ʾ���

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
	SudokuLoader m_loader;	// ������ �δ�
	Sudoku *m_data;			// ������
	int m_x, m_y;			// ������ǥ 

	// ���ο� �޼ҵ��Դϴ�.
	void ClearButtonValues();	// ��ư ���� �ʱ�ȭ(�� ���� ��)
	void LoadFromFile(LPCTSTR file);	// ���Ͽ��� �����͸� �ҷ���
	void DisplayToUI(Sudoku *data);		// UI�� ������ ǥ��
	void SetButtonValue(CButton&button, char value);	// ��ư ���� ����
	void SaveToFile(LPCTSTR file);	// ��ư ���� ���Ϸ� ����
	void InitPlayMode();			// �÷��̾� ���� �ʱ�ȭ
	void SelectPosition(int x, int y);	// ��ǥ ����
	void EnableSelectButton(BOOL bEnable);	// ��ȣ���� Ȱ��ȭ

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

	afx_msg void OnChangeRadioMode();	// ���Ӹ�� �����

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton33();
	afx_msg void OnBnClickedButton34();
	afx_msg void OnBnClickedButton41();
	afx_msg void OnBnClickedButton42();
	afx_msg void OnBnClickedButton43();
	afx_msg void OnBnClickedButton44();
public:
	CButton m_Select1;
	CButton m_Select2;
	CButton m_Select3;
	CButton m_Select4;
	CStatic m_postion;
};
