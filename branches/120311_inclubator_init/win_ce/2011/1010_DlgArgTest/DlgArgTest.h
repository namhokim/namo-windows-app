// DlgArgTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CDlgArgTestApp:
// �� Ŭ������ ������ ���ؼ��� DlgArgTest.cpp�� �����Ͻʽÿ�.
//

class CDlgArgTestApp : public CWinApp
{
public:
	CDlgArgTestApp();
	
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDlgArgTestApp theApp;