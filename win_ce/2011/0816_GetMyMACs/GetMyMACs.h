// GetMyMACs.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CGetMyMACsApp:
// �� Ŭ������ ������ ���ؼ��� GetMyMACs.cpp�� �����Ͻʽÿ�.
//

class CGetMyMACsApp : public CWinApp
{
public:
	CGetMyMACsApp();
	
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CGetMyMACsApp theApp;
