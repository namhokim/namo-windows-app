// UsingLibrary.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "UsingLibrary.h"
#include "..\Library\SingletonOperation.h"
#ifdef DEBUG
#pragma comment(lib, "../Debug/Library.lib")
#else
#pragma comment(lib, "../Release/Library.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
		_tprintf(_T("�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: ���� ���α׷��� ������ ���⿡�� �ڵ��մϴ�.
		string buf;
		cout << "my::add(1, 2) : " << my::add(1, 2) << endl;
		cin >> buf;
		cout << "my::add(1, 3) : " << my::add(1, 3) << endl;
		cin >> buf;
		cout << "my::add(1, 4) : " << my::add(1, 4) << endl;
		cin >> buf;
	}

	return nRetCode;
}
