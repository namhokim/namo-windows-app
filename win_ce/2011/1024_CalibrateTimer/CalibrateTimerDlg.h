// CalibrateTimerDlg.h : 헤더 파일
//

#pragma once

// CCalibrateTimerDlg 대화 상자
class CCalibrateTimerDlg : public CDialog
{
// 생성입니다.
public:
	CCalibrateTimerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CALIBRATETIMER_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;
	INT   m_select;

	// 생성된 메시지 맵 함수
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
