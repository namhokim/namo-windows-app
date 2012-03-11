// MFC_CStringToLPSTR.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "MFC_CStringToLPSTR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;

using namespace std;

void MFCSTringToLPSTR(__in const CString& in_str, __out LPSTR out_str, __in int out_str_len);

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
		CString origin_str = _T("Original String");
		int len = origin_str.GetLength();
		if(len > 0) {
			int alloc_size = len + 1;
			LPSTR tmp_str = (LPSTR)malloc(alloc_size);		// �Ҵ�
			if(NULL!=tmp_str) {
				MFCSTringToLPSTR(origin_str, tmp_str, alloc_size);	// ��ȯ
				printf("%s\n", tmp_str);					// ���� ��
				free(tmp_str);								// ����
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