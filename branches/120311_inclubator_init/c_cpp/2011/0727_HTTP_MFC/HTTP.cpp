// HTTP.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "HTTP.h"
#include "WebServer.h"

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
		USER_INFO ui;
		ui.card_number = _T("0123456789");
		if(WebServer::GetUserInfo(&ui)) {
			wcout << L"card number: " << ui.card_number << endl;
			wcout << L"Used/Quota: " << ui.dwUsed << L"/" << ui.dwQuota << endl;
		} else {
			cout << "failed" << endl;
		}
		
	}

	return nRetCode;
}
