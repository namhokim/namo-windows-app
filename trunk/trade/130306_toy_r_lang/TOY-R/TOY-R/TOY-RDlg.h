
// TOY-RDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <string>		// 중간코드
#include <vector>		// 중간코드

// CTOYRDlg 대화 상자
class CTOYRDlg : public CDialog
{
// 생성입니다.
public:
	CTOYRDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOYR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	void ClearEditControl();	// 화면지우기
	void LoadFromFile();		// 파일 불러오기
	void ConvertToPrefix();		// 표기법 변환(to prefix)
	void CreateIMCode();		// 중간코드 생성
	void SaveTheIMCode();		// 중간코드 저장(m_imc)
	void LoadTheIMCode();		// 중간코드 불러오기(m_imc)
	void Evaluation();			// 연산하기(m_imc)

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
	CEdit m_program;				// 프로그램 에디트 컨트롤
	CEdit m_convert;				// 중간코드 에디트 컨트롤
	CEdit m_result;					// 연산결과 에디트 컨트롤
	CString m_convert_content;		// 전위로 전환한 데이터
	std::vector<std::string> m_imc;	// 중간코드 저장할 벡터
};
