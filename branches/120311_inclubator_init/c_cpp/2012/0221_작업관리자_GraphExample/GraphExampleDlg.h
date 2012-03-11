// GraphExampleDlg.h : header file
//

#if !defined(AFX_GRAPHEXAMPLEDLG_H__3B08E55B_B8A1_4100_ADB9_B25526CBD502__INCLUDED_)
#define AFX_GRAPHEXAMPLEDLG_H__3B08E55B_B8A1_4100_ADB9_B25526CBD502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarChart.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphExampleDlg dialog

class CGraphExampleDlg : public CDialog
{
// Construction
public:
	CGraphExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGraphExampleDlg)
	enum { IDD = IDD_GRAPHEXAMPLE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphExampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CWnd *m_EditWnd;
// Implementation
protected:
	HICON m_hIcon;

	CBarChart m_wndBarChart;

	// Generated message map functions
	//{{AFX_MSG(CGraphExampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonInput();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHEXAMPLEDLG_H__3B08E55B_B8A1_4100_ADB9_B25526CBD502__INCLUDED_)
