// DlgArgTest.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "DlgArgTest.h"
#include "DlgArgTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDlgArgTestApp

BEGIN_MESSAGE_MAP(CDlgArgTestApp, CWinApp)
END_MESSAGE_MAP()


// CDlgArgTestApp ����
CDlgArgTestApp::CDlgArgTestApp()
	: CWinApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CDlgArgTestApp ��ü�Դϴ�.
CDlgArgTestApp theApp;

class CCheckCmdLineInfo : public CCommandLineInfo {
private:
	DWORD dwArgc;
	CArray <CString, CString&> m_argv;

public:
	CCheckCmdLineInfo();
	INT_PTR argc() const;
	CString&  GetAt(INT_PTR i);

	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);   // �������̵�
};

// CDlgArgTestApp �ʱ�ȭ

BOOL CDlgArgTestApp::InitInstance()
{

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ 
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	CString argc;
	argc.Format(L"__argc : %d", __argc);
	MessageBox(NULL, argc, _T("__argc"), MB_OK);

	CCheckCmdLineInfo rCmdInfo;
	ParseCommandLine(rCmdInfo);

	//CDlgArgTestDlg dlg;
	//m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
	//	//  �ڵ带 ��ġ�մϴ�.
	//}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
	return FALSE;
}

CCheckCmdLineInfo::CCheckCmdLineInfo():dwArgc(0), CCommandLineInfo()
{
}

void CCheckCmdLineInfo::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
	CString param(pszParam);
	MessageBox(NULL, param, _T("Param"), MB_OK);
	m_argv.Add(param);
	dwArgc++;
	return;
}

INT_PTR CCheckCmdLineInfo::argc() const
{
	return m_argv.GetCount();
}

CString& CCheckCmdLineInfo::GetAt(INT_PTR i)
{
	return m_argv.GetAt(i);
}