#if !defined(AFX_BARCHART_H__91797299_454B_4D6D_97CB_ACD0C0C87B94__INCLUDED_)
#define AFX_BARCHART_H__91797299_454B_4D6D_97CB_ACD0C0C87B94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarChart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBarChart window

class CBarChart : public CWnd
{
// Construction
public:
	CBarChart();

// Attributes
public:
	COLORREF	m_crBk, m_crBarOff, m_crBarOn;
	int			m_nBarWidth, m_nBarHeight, m_nGap;
	int			m_nVerBarNum;
	int			m_nHorBarNumb;
	CRect		m_Margin;

	double		m_dblMin, m_dblMax, m_dblValue;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarChart)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetValue(double val);
	void SetRange(double min, double max);
	void DrawBar(CDC *pdc, int x, int y, int num, COLORREF rgb);
	virtual ~CBarChart();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBarChart)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCHART_H__91797299_454B_4D6D_97CB_ACD0C0C87B94__INCLUDED_)
