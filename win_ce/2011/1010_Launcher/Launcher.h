// Launcher.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CLauncherApp:
// �� Ŭ������ ������ ���ؼ��� Launcher.cpp�� �����Ͻʽÿ�.
//

class CLauncherApp : public CWinApp
{
public:
	CLauncherApp();
	
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CLauncherApp theApp;
