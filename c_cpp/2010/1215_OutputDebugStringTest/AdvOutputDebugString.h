#pragma once

#ifndef	_ADV_OUTPUT_DEBUG_STRING_H_
#define	_ADV_OUTPUT_DEBUG_STRING_H_

#include <Windows.h>
#include <string>
#include <vector>

#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
#undef	STRSAFE_NO_DEPRECATE

namespace Namo
{
	const size_t init_size = 500;

__inline void OutputDebugStringA(_In_z_ _Printf_format_string_ const char * pszFormat, ...)
{
	// start of format argument list
	va_list argList;
	va_start(argList, pszFormat);

	// Convert
	size_t buf_size = init_size;
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
			std::string szOutput( buf.begin(), (buf.begin()+(size_str*sizeof(char))) );

			::OutputDebugStringA( szOutput.c_str() );					// 전역 함수(Windows API)
			break;
		} else if ( STRSAFE_E_INSUFFICIENT_BUFFER==hr ) {
			buf_size *= 2;	// 2배로 버퍼길이 증가
		}else {
			// STRSAFE_E_INVALID_PARAMETER
			::OutputDebugStringA( "Namo:OutputDebugStringA Error" );	// 전역 함수(Windows API)
			break;
		}
	}

	// end of format argument list
	va_end(argList);
}

__inline void OutputDebugStringW(_In_z_ _Printf_format_string_ const wchar_t * pszFormat, ...)
{
	// start of format argument list
	va_list argList;
	va_start(argList, pszFormat);

	// Convert
	size_t buf_size = init_size;
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
			std::wstring szOutput( buf.begin(), (buf.begin()+(size_str*sizeof(wchar_t))) );
			//LPCWSTR msg = szOutput.c_str();
			::OutputDebugStringW( szOutput.c_str() );					// 전역 함수(Windows API)
			break;
		} else if ( STRSAFE_E_INSUFFICIENT_BUFFER==hr ) {
			buf_size *= 2;	// 2배로 버퍼길이 증가
		}else {
			// STRSAFE_E_INVALID_PARAMETER
			::OutputDebugStringW( L"Namo:OutputDebugStringA Error" );	// 전역 함수(Windows API)
			break;
		}
	}

	// end of format argument list
	va_end(argList);
}

}

#endif