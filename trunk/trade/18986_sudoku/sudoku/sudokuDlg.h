
// sudokuDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "../algorithm/SudokuSolver.h"

#define MAZE_SIZE 4		// 최대크기 2X2가 2개
#define NOT_SELECTED	(-1)	// 좌표 선택이 되지 않았음

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
	SudokuLoader m_loader;	// 데이터 로더
	Sudoku *m_data;			// 데이터
	int m_x, m_y;			// 선택좌표 

	// 내부용 메소드입니다.
	void ClearButtonValues();	// 버튼 값을 초기화(새 게임 시)
	void LoadFromFile(LPCTSTR file);	// 파일에서 데이터를 불러옴
	void DisplayToUI(Sudoku *data);		// UI에 데이터 표시
	void SetButtonValue(CButton&button, char value);	// 버튼 값을 설정
	void SaveToFile(LPCTSTR file);	// 버튼 값을 파일로 저장
	void InitPlayMode();			// 플레이어 모드로 초기화
	void SelectPosition(int x, int y);	// 좌표 선택
	void EnableSelectButton(BOOL bEnable);	// 번호선택 활성화

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

	afx_msg void OnChangeRadioMode();	// 게임모드 변경시

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
