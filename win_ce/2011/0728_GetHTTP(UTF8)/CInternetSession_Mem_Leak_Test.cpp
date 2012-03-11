// CInternetSession_Mem_Leak_Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "CInternetSession_Mem_Leak_Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

#include <vector>
using namespace std;

BOOL GetHTTP(vector<BYTE>& v);
BOOL UTF8_to_CString(LPCSTR lpMultiByteStr, int cbMultiByte, CString& cstr);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
		vector<BYTE> buf;
		CString cont;

		GetHTTP(buf);
		UTF8_to_CString((LPCSTR)&buf[0], buf.size(), cont);
		wcout << LPCWSTR(cont) << endl;
		//copy(buf.begin(), buf.end(), ostream_iterator<BYTE>(cout));
		//cout << endl;
	}

	return nRetCode;
}

BOOL GetHTTP(vector<BYTE>& v)
{
	CInternetSession s(_T("Una HTTP Cliente Aplicación"));
	try {
		DWORD dwStatus;
		auto_ptr<CHttpConnection> pConn( s.GetHttpConnection(_T("10.16.11.84"), INTERNET_PORT(8080)) );
		auto_ptr<CHttpFile> pFile( pConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, _T("/mirus/config")) );

		if( pFile->SendRequest()
			&& pFile->QueryInfoStatusCode(dwStatus) && HTTP_STATUS_OK==dwStatus) {
			
			ULONGLONG ulLenth = pFile->GetLength();						// 컨텐츠 길이 구해(look-ahead)
			DWORD dwRead = 0;
			BYTE buf[1024+1] = {0};
			
			v.clear();
			if(ulLenth>0) v.reserve(vector<BYTE>::size_type(ulLenth));	// 크기 예측의 경우 reserve
				
			while( (dwRead=pFile->Read(buf, 1024))>0 ) {
				v.insert(v.end(), &buf[0], &buf[0] + dwRead);
			}
			return TRUE;
		}
		
	} catch(auto_ptr<CInternetException> pE) {
		cerr << "CInternetException: " << pE->m_dwError << endl;
	}

	v.clear();		// 실패시 빈 컨텐츠 보장
	return FALSE;
}

BOOL UTF8_to_CString(LPCSTR lpMultiByteStr, int cbMultiByte, CString& cstr)
{
	int needBuf = 0;

	needBuf = MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, cbMultiByte, NULL, NULL);
	if(needBuf) {
		vector<TCHAR> buf(needBuf+1);
		needBuf = MultiByteToWideChar(CP_UTF8, 0, lpMultiByteStr, cbMultiByte, &buf[0], needBuf);
		cstr = (LPCWSTR)&buf[0];
		return TRUE;
	} else {
		return FALSE;
	}
}