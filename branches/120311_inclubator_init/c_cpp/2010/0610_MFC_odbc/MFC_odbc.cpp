// MFC_odbc.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "MFC_odbc.h"

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
		CDatabase db;
		BOOL res = db.OpenEx(TEXT("Description=KT_DB;DRIVER=SQL Server;SERVER=10.16.11.9;UID=sa;PWD=sindo;APP=Microsoft Data Access Components;WSID=�賲ȣ;DATABASE=SINDOH;Network=DBMSSOCN"));
		if(res) {
			db.ExecuteSQL(TEXT("Insert into ABBA.dbo.user_info (id, name) values ('1', 'value');"));
		}
	}

	return nRetCode;
}
