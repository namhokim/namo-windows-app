
// sudokuDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#define MAZE_SIZE 4

// CsudokuDlg 대화 상자
class CsudokuDlg : public CDialog
{
// 생성입니다.
public:
	CsudokuDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SUDOKU_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 맴버입니다.
private:
	void LoadFromFile(LPCTSTR file);
	void ParseLine(LPCSTR line,CString& v1,CString& v2,CString& v3,CString& v4);
	void SetButtonValue(CButton&button, const CString& value);
	void ClearButtonValues();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
