#include "OutputDebugPrintf.h"

// Standard C++ Libraries
#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

#define REMOVE_MESSAGE_CR_LF

char* ErrorCodeToStringA(char* buf, unsigned long dwCode)
{
	LPVOID lpMsgBuf = NULL;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);

	// copy
#ifdef	REMOVE_MESSAGE_CR_LF
	char* lpDest;
	LPCSTR lpSrc;

	lpSrc = (LPCSTR)lpMsgBuf;
	lpDest = buf;
	while( (*lpDest) = (*lpSrc) ) {
		++lpDest;
		++lpSrc;
	}
	// remove <CR><LF>
	--lpDest;
	if( L'\n' == (*lpDest) ) {
		(*lpDest) = L'\0';
	}
	--lpDest;
	if( L'\r' == (*lpDest) ) {
		(*lpDest) = L'\0';
	}
#else
	lstrcpyA(buf, (LPCSTR)lpMsgBuf);
#endif

	// release
	LocalFree(lpMsgBuf);

	return buf;
}

wchar_t* ErrorCodeToStringW(wchar_t* buf, unsigned long dwCode)
{
	LPVOID lpMsgBuf;

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	// copy
#ifdef	REMOVE_MESSAGE_CR_LF
	wchar_t* lpDest;
	LPCWSTR lpSrc;

	lpSrc = (LPCWSTR)lpMsgBuf;
	lpDest = buf;
	while( (*lpDest) = (*lpSrc) ) {
		++lpDest;
		++lpSrc;
	}
	// remove <CR><LF>
	--lpDest;
	if( L'\n' == (*lpDest) ) {
		(*lpDest) = L'\0';
	}
	--lpDest;
	if( L'\r' == (*lpDest) ) {
		(*lpDest) = L'\0';
	}
#else
	lstrcpyW(buf, (LPCWSTR)lpMsgBuf);
#endif

	// release
	LocalFree(lpMsgBuf);

	return buf;
}

#include <strsafe.h>

const size_t init_alloc_size = 256;

void OutputDebugPrintfA(const char * pszFormat, ...)
{
	// start of format argument list
	va_list argList;
	va_start(argList, pszFormat);

	// Convert
	size_t buf_size = init_alloc_size;
	size_t size_str = STRSAFE_MAX_CCH;
	HRESULT hr = S_FALSE;

	while ( S_OK != hr ) {	// 성공적으로 변환이 될 때 까지 반복수행
		std::vector<char> buf(buf_size);
		size_t remaining;

		hr = ::StringCchVPrintfExA(
			reinterpret_cast<STRSAFE_LPSTR>(&buf[0]),
			buf_size,
			NULL,
			&remaining,
			STRSAFE_NO_TRUNCATION, 
			pszFormat, argList
			);

		if ( SUCCEEDED(hr) ) {	// 성공
			size_str = buf_size - remaining;
			std::string szOutput( buf.begin(), (buf.begin()+size_str) );

			::OutputDebugStringA( szOutput.c_str() );					// 전역 함수(Windows API)
			break;
		} else if ( STRSAFE_E_INSUFFICIENT_BUFFER==hr ) {
			buf_size *= 2;	// 2배로 버퍼길이 증가
		}else {
			// STRSAFE_E_INVALID_PARAMETER
			::OutputDebugStringA( "OutputDebugPrintfA Error" );	// 전역 함수(Windows API)
			break;
		}
	}

	// end of format argument list
	va_end(argList);
}

void OutputDebugPrintfW(const wchar_t * pszFormat, ...)
{
	// start of format argument list
	va_list argList;
	va_start(argList, pszFormat);

	// Convert
	size_t buf_size = init_alloc_size;
	size_t size_str = STRSAFE_MAX_CCH;
	HRESULT hr = S_FALSE;

	while ( S_OK != hr ) {	// 성공적으로 변환이 될 때 까지 반복수행
		std::vector<wchar_t> buf(buf_size);
		size_t remaining;

		hr = ::StringCchVPrintfExW(
			reinterpret_cast<STRSAFE_LPWSTR>(&buf[0]),
			buf_size,
			NULL,
			&remaining,
			STRSAFE_NO_TRUNCATION, 
			pszFormat, argList
			);

		if ( SUCCEEDED(hr) ) {	// 성공
			size_str = buf_size - remaining;

			std::wstring szOutput( buf.begin(), (buf.begin()+size_str) );
			::OutputDebugStringW( szOutput.c_str() );					// 전역 함수(Windows API)
			break;
		} else if ( STRSAFE_E_INSUFFICIENT_BUFFER==hr ) {
			buf_size *= 2;	// 2배로 버퍼길이 증가
		}else {
			// STRSAFE_E_INVALID_PARAMETER
			::OutputDebugStringW( L"OutputDebugPrintfW Error" );	// 전역 함수(Windows API)
			break;
		}
	}

	// end of format argument list
	va_end(argList);
}
