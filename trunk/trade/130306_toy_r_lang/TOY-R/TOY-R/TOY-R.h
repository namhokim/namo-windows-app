
// TOY-R.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTOYRApp:
// �� Ŭ������ ������ ���ؼ��� TOY-R.cpp�� �����Ͻʽÿ�.
//

class CTOYRApp : public CWinAppEx
{
public:
	CTOYRApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CTOYRApp theApp;