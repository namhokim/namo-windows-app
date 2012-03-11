// GraphExample.h : main header file for the GRAPHEXAMPLE application
//

#if !defined(AFX_GRAPHEXAMPLE_H__FBDFF00F_A18F_4619_9B1B_68BD73AD821E__INCLUDED_)
#define AFX_GRAPHEXAMPLE_H__FBDFF00F_A18F_4619_9B1B_68BD73AD821E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGraphExampleApp:
// See GraphExample.cpp for the implementation of this class
//

class CGraphExampleApp : public CWinApp
{
public:
	CGraphExampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphExampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGraphExampleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHEXAMPLE_H__FBDFF00F_A18F_4619_9B1B_68BD73AD821E__INCLUDED_)
