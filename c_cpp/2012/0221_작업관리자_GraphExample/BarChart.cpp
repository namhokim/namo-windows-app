// BarChart.cpp : implementation file
//

#include "stdafx.h"
#include "GraphExample.h"
#include "BarChart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarChart

CBarChart::CBarChart()
{
	m_crBk = RGB(0, 0, 0);
	m_crBarOff = RGB(0, 128, 0);
	m_crBarOn = RGB(0, 255, 0);

	m_nBarWidth = 16;
	m_nBarHeight = 2;
	m_nGap = 1;

	m_nVerBarNum = -1;
	m_nHorBarNumb = 2;
	
	m_Margin.SetRect(-1, 10, -1, 40);

	SetRange(0, 100);
	SetValue(0);
}

CBarChart::~CBarChart()
{
}


BEGIN_MESSAGE_MAP(CBarChart, CWnd)
	//{{AFX_MSG_MAP(CBarChart)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBarChart message handlers

void CBarChart::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rtClient;

	GetClientRect(rtClient);

	// 막대수 계산
	int height = rtClient.Height() - m_Margin.top - m_Margin.bottom;
	int m_nVerBarNum = height / (m_nBarHeight + m_nGap);
	int nBarOnNum = (int)(m_nVerBarNum * (m_dblValue / (m_dblMax - m_dblMin)));

	// 배경색을 칠합니다.
	dc.FillSolidRect(rtClient, m_crBk);
	
	// Do not call CWnd::OnPaint() for painting messages

	// Off
	DrawBar(&dc, rtClient.Width() / 2 - m_nBarWidth, rtClient.Height() - m_Margin.bottom, m_nVerBarNum, m_crBarOff);
	DrawBar(&dc, rtClient.Width() / 2 + 1, rtClient.Height() - m_Margin.bottom, m_nVerBarNum, m_crBarOff);

	// On
	DrawBar(&dc, rtClient.Width() / 2 - m_nBarWidth, rtClient.Height() - m_Margin.bottom, nBarOnNum, m_crBarOn);
	DrawBar(&dc, rtClient.Width() / 2 + 1, rtClient.Height() - m_Margin.bottom, nBarOnNum, m_crBarOn);

	CFont fnt, *pfnt;
	CRect rtTxt;
	CString strTxt;

	fnt.CreatePointFont(100, "돋움체");
	pfnt = dc.SelectObject( &fnt );


	rtTxt.SetRect(0, rtClient.Height() - m_Margin.bottom, rtClient.Width(), rtClient.Height());
	strTxt.Format("%3.f%%", m_dblValue / (m_dblMax - m_dblMin) * 100);
	dc.SetBkColor(m_crBk);
	dc.SetTextColor(m_crBarOn);
	dc.DrawText(strTxt, rtTxt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject(pfnt);
}


// y 좌표를 감소시키면서 그림.
//
void CBarChart::DrawBar(CDC *pdc, int x, int y, int num, COLORREF rgb)
{
	for (int i=0; i<num ; i++)
	{
		y -= (m_nBarHeight + m_nGap);
		pdc->FillSolidRect(x, y + 1, m_nBarWidth, m_nBarHeight, rgb );		
	}
}

void CBarChart::SetRange(double min, double max)
{
	m_dblMin = min;
	m_dblMax = max;
}

void CBarChart::SetValue(double val)
{
	m_dblValue = val;
}
