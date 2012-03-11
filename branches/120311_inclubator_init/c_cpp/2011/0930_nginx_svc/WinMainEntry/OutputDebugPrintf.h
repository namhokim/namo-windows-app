#pragma once

/* 

 이 라이브러리는
 <WinBase.h>에 있는 OutputDebugString 함수를 Format 형식으로 사용하도록 편의상 제작함

 내부 호출 API 함수
	OutputDebugStringW
	OutputDebugStringA

 작성자 : Namho Kim
 작성일 : 2011.4.29 15:05

 */

#ifndef	_OUTPUT_DEBUG_PRINTF_H_
#define	_OUTPUT_DEBUG_PRINTF_H_

#define	MESSAGE_FORMAT_MAX	1024

// 에러코드 문자열 변환 함수
char* ErrorCodeToStringA(char* buf, unsigned long dwCode);
wchar_t* ErrorCodeToStringW(wchar_t* buf, unsigned long dwCode);

#ifdef UNICODE
#define ErrorCodeToString  ErrorCodeToStringW
#else
#define ErrorCodeToString  ErrorCodeToStringA
#endif // !UNICODE


// OutputDebugString 헬퍼 함수
void OutputDebugPrintfA(const char * pszFormat, ...);
void OutputDebugPrintfW(const wchar_t * pszFormat, ...);

#ifdef UNICODE
#define OutputDebugPrintf  OutputDebugPrintfW
#else
#define OutputDebugPrintf  OutputDebugPrintfA
#endif // !UNICODE


#endif	/* _OUTPUT_DEBUG_PRINTF_H_ */
