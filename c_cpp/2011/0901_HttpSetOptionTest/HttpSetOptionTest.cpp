// HttpSetOptionTest.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "HttpSetOptionTest.h"
#include <afxinet.h>	// also included <WinInet.h>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;
void Usage();
BOOL Request(__in LPCTSTR lpszUrl,
			__in DWORD dwTimout = 60000,
			__in DWORD dwRetries = 5,
			__in int nVerb = CHttpConnection::HTTP_VERB_GET);

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
		switch(argc) {
			case 2:
				return (Request(argv[1]) ? 0 : 1);
			case 3:
				return (Request(argv[1], _tstol(argv[2])) ? 0 : 1);
			case 4:
				return (Request(argv[1], _tstol(argv[2]), _tstol(argv[3])) ? 0 : 1);
			default:
				Usage();
		}

	}

	return nRetCode;
}

void Usage()
{
	_tprintf(_T("Usage: .exe [URL] [INTERNET_OPTION_CONNECT_TIMEOUT] [INTERNET_OPTION_CONNECT_RETRIES]"));
}

BOOL Request(__in LPCTSTR lpszUrl, __in DWORD dwTimout,
			__in DWORD dwRetries, __in int nVerb)
{
	// 1. URL CRAKING
	TCHAR scheme[INTERNET_MAX_SCHEME_LENGTH + 1];
	TCHAR hostname[INTERNET_MAX_HOST_NAME_LENGTH + 1];
	TCHAR urlpath[INTERNET_MAX_PATH_LENGTH + 1];
	TCHAR extra[INTERNET_MAX_PATH_LENGTH + 1];

	URL_COMPONENTS uc;
	ZeroMemory(&uc, sizeof(uc));
	uc.dwStructSize = sizeof(uc);
	uc.dwSchemeLength		= INTERNET_MAX_SCHEME_LENGTH;
	uc.dwHostNameLength		= INTERNET_MAX_HOST_NAME_LENGTH;
	uc.dwUrlPathLength		= INTERNET_MAX_PATH_LENGTH;
	uc.dwExtraInfoLength	= INTERNET_MAX_PATH_LENGTH;

	uc.lpszScheme = scheme;
	uc.lpszHostName = hostname;
	uc.lpszUrlPath = urlpath;
	uc.lpszExtraInfo = extra;

	BOOL bRes = ::InternetCrackUrl(lpszUrl, lstrlen(lpszUrl), ICU_DECODE, &uc);
	if(!bRes) {
		DWORD dw = ::GetLastError();
		CInternetException e(dw);
		e.m_bAutoDelete = TRUE;
		TCHAR errMsg[1024]={0};
		e.GetErrorMessage(errMsg, 1024);
		_tprintf(_T("InternetCrackUrl Error: %s (%d)\n"), errMsg, dw);
		return FALSE;
	}

	// 2. REQUESTING
	CInternetSession s;
	s.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, dwTimout);
	s.SetOption(INTERNET_OPTION_CONNECT_RETRIES, dwRetries);
	try {
		auto_ptr<CHttpConnection> pConn( s.GetHttpConnection(uc.lpszHostName, uc.nPort) );
		CString objectName(urlpath);
		objectName.Append(extra);
		auto_ptr<CHttpFile> pFile( pConn->OpenRequest(nVerb, objectName) );

		CString contentType;
		if( pFile->SendRequest() ) {
			BYTE buf[1024+1] = {0};
			DWORD dwRead = 0;

			while( (dwRead=pFile->Read(buf, 1024))>0 );

			pFile->Close();
			pConn->Close();
			return TRUE;
		}
	} catch(CInternetException* pE) {
		TCHAR errMsg[1024]={0};
		pE->GetErrorMessage(errMsg, 1024);
		_tprintf(_T("CInternetException: %s (%d)\n"), errMsg, pE->m_dwError);

		pE->Delete();
	}

	return FALSE;
}