
// sudoku.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CsudokuApp:
// �� Ŭ������ ������ ���ؼ��� sudoku.cpp�� �����Ͻʽÿ�.
//

class CsudokuApp : public CWinAppEx
{
public:
	CsudokuApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonNew();
};

extern CsudokuApp theApp;