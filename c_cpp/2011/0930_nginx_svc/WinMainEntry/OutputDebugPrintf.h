#pragma once

/* 

 �� ���̺귯����
 <WinBase.h>�� �ִ� OutputDebugString �Լ��� Format �������� ����ϵ��� ���ǻ� ������

 ���� ȣ�� API �Լ�
	OutputDebugStringW
	OutputDebugStringA

 �ۼ��� : Namho Kim
 �ۼ��� : 2011.4.29 15:05

 */

#ifndef	_OUTPUT_DEBUG_PRINTF_H_
#define	_OUTPUT_DEBUG_PRINTF_H_

#define	MESSAGE_FORMAT_MAX	1024

// �����ڵ� ���ڿ� ��ȯ �Լ�
char* ErrorCodeToStringA(char* buf, unsigned long dwCode);
wchar_t* ErrorCodeToStringW(wchar_t* buf, unsigned long dwCode);

#ifdef UNICODE
#define ErrorCodeToString  ErrorCodeToStringW
#else
#define ErrorCodeToString  ErrorCodeToStringA
#endif // !UNICODE


// OutputDebugString ���� �Լ�
void OutputDebugPrintfA(const char * pszFormat, ...);
void OutputDebugPrintfW(const wchar_t * pszFormat, ...);

#ifdef UNICODE
#define OutputDebugPrintf  OutputDebugPrintfW
#else
#define OutputDebugPrintf  OutputDebugPrintfA
#endif // !UNICODE


#endif	/* _OUTPUT_DEBUG_PRINTF_H_ */
