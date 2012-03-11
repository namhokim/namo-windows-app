// MFC_CStringToLPSTR.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MFC_CStringToLPSTR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;

void MFCSTringToLPSTR(__in const CString& in_str, __out LPSTR out_str, __in int out_str_len);

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
		CString origin_str = _T("Original String");
		int len = origin_str.GetLength();
		if(len > 0) {
			int alloc_size = len + 1;
			LPSTR tmp_str = (LPSTR)malloc(alloc_size);		// 할당
			if(NULL!=tmp_str) {
				MFCSTringToLPSTR(origin_str, tmp_str, alloc_size);	// 변환
				printf("%s\n", tmp_str);					// 쓰는 곳
				free(tmp_str);								// 해제
			}
		}
	}

	return nRetCode;
}

void MFCSTringToLPSTR(__in const CString& in_str, __out LPSTR out_str, __in int out_str_len)
{
	ZeroMemory(out_str, out_str_len);
	if (out_str_len == 0) return;

	LPCWSTR in_wstr = (LPCWSTR)in_str;

	// Calculate necessary buffer size
	int len = ::WideCharToMultiByte(
		CP_ACP, 0, in_wstr, in_str.GetLength(), NULL, 0, NULL, NULL);

	// Perform actual conversion
	if (len < out_str_len) {
		len = ::WideCharToMultiByte(
			CP_ACP, 0, in_wstr, in_str.GetLength(), out_str, out_str_len, NULL, NULL);
	}
}