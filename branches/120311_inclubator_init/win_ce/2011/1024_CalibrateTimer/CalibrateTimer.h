// CalibrateTimer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CCalibrateTimerApp:
// �� Ŭ������ ������ ���ؼ��� CalibrateTimer.cpp�� �����Ͻʽÿ�.
//

class CCalibrateTimerApp : public CWinApp
{
public:
	CCalibrateTimerApp();
	
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CCalibrateTimerApp theApp;
