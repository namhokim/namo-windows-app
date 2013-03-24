
// sudokuDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#define MAZE_SIZE 4		// 최대크기 2X2가 2개

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


private:
	// 맴버입니다.
	HACCEL m_hAccelTable;
	CButton btn[MAZE_SIZE][MAZE_SIZE];	// 버튼 값 설정/가져오기 위한 객체
	CButton m_ButtonUndo;	// 되돌리기 버튼
	bool m_hasUndo;			// 되돌리기 여부

	// 내부용 메소드입니다.
	void ClearButtonValues();	// 버튼 값을 초기화(새 게임 시)
	void LoadFromFile(LPCTSTR file);	// 파일에서 데이터를 불러옴
	void ParseLine(LPCSTR line,CString& v1,CString& v2,CString& v3,CString& v4);	// 줄 단위의 값을 가져옴
	void SetButtonValue(CButton&button, const CString& value);	// 버튼 값을 설정
	void SaveToFile(LPCTSTR file);	// 버튼 값을 파일로 저장

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();	
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);	// 단축키를 이용하기 위한 처리
	afx_msg void OnBnClickedButtonNew();	// 새 게임
	afx_msg void OnBnClickedButtonSave();	// 게임 저장하기
	afx_msg void OnBnClickedButtonQuit();	// 끝내기

	void OnChangeRadioMode();	// 게임모드 변경시
};
