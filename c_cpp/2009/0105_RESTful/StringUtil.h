#ifndef __NAMO_UNICODE_SELECTOR_H__
#define __NAMO_UNICODE_SELECTOR_H__

#include <string>	// for _STRING
#include <iostream>	// for _COUT
#include <direct.h>	// for _TGETCWD
#include <tchar.h>	// for _T
#include <algorithm>	// for Tokenize()
#include <vector>		// for Tokenize()
#include <stdio.h>

#ifdef _UNICODE
#define _COUT	std::wcout
#define _STRING	std::wstring
#define _TGETCWD	_wgetcwd
#define _STRINGSTREAM	std::wstringstream
#else
#define _COUT	std::cout
#define _STRING	std::string
#define _TGETCWD	_getcwd
#define _STRINGSTREAM	std::stringstream
#endif
#endif

#pragma once
namespace namo {
class StringUtil
{
public:
	StringUtil(void)	{	setlocale(LC_ALL, "Korean");	};
	~StringUtil(void)	{};
	std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = 0;
		std::vector<char> buf((str.size() + 1) * codecvt.max_length());
		wchar_t const* in_next = str.c_str();
		char* out_next = &buf[0];
		codecvt_t::result r = codecvt.out(state,
			str.c_str(), str.c_str() + str.size(), in_next,
			&buf[0], &buf[0] + buf.size(), out_next);
		return std::string(&buf[0]);
	}
	std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = 0;
		std::vector<wchar_t> buf(str.size() + 1);
		char const* in_next = str.c_str();
		wchar_t* out_next = &buf[0];
		codecvt_t::result r = codecvt.in(state,
			str.c_str(), str.c_str() + str.size(), in_next,
			&buf[0], &buf[0] + buf.size(), out_next);
		return std::wstring(&buf[0]);
	}
	void Tokenize(const _STRING& str, std::vector<_STRING>& tokens, const _STRING& delimiters = _T(" "))
	{
		// 맨 첫 글자가 구분자인 경우 무시
		_STRING::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// 구분자가 아닌 첫 글자를 찾는다
		_STRING::size_type pos     = str.find_first_of(delimiters, lastPos);

		while (_STRING::npos != pos || _STRING::npos != lastPos)
		{
			// token을 찾았으니 vector에 추가한다
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// 구분자를 뛰어넘는다.  "not_of"에 주의하라
			lastPos = str.find_first_not_of(delimiters, pos);
			// 다음 구분자가 아닌 글자를 찾는다
			pos = str.find_first_of(delimiters, lastPos);
		}
	}
	_STRING int_to_string(int valueInt)
	{
		_TCHAR bufferString[1024];
#ifdef _UNICODE
		_snwprintf_s(bufferString, 1024, _T("%d"), valueInt);
#else
		_snprintf_s(bufferString, 1024, _T("%d"), valueInt);
#endif
//		wsprintf(bufferString, _T("%d"), valueInt);
		return bufferString;
	}
	_STRING int_to_string(long valueInt)
	{
		_TCHAR bufferString[1024];
#ifdef _UNICODE
		_snwprintf_s(bufferString, 1024, _T("%d"), valueInt);
#else
		_snprintf_s(bufferString, 1024, _T("%d"), valueInt);
#endif
//		wsprintf(bufferString, _T("%d"), valueInt);
		return bufferString;
	}
	_STRING int_to_string(__int64 valueInt)
	{
		_TCHAR bufferString[1024];
#ifdef _UNICODE
		_snwprintf_s(bufferString, 1024, _T("%d"), valueInt);
#else
		_snprintf_s(bufferString, 1024, _T("%d"), valueInt);
#endif
//		wsprintf(bufferString, _T("%d"), valueInt);
		return bufferString;
	}
	_STRING int_to_string(unsigned long valueInt)
	{
		_TCHAR bufferString[1024];
#ifdef _UNICODE
		_snwprintf_s(bufferString, 1024, _T("%d"), valueInt);
#else
		_snprintf_s(bufferString, 1024, _T("%d"), valueInt);
#endif
//		wsprintf(bufferString, _T("%d"), valueInt);
		return bufferString;
	}
};
} // namespace namo