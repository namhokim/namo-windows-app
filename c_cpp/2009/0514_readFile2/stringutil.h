#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>	// for std::tstring
#include <iostream>	// for _COUT
#include <direct.h>	// for _TGETCWD
#include <tchar.h>	// for _T
#include <vector>	// for getTokensWithVectors()
#include <map>		// for kvString2Map()
#include <set>		// for TstringSet

#include "TString.h"

typedef std::vector<int>			IndexVector, IntVec;
typedef std::vector<std::string>	StringVector, StrVec;
typedef std::vector<std::wstring>	WstringVector, WStrVec;
typedef std::vector<std::tstring>	TstringVector, TStrVec;
typedef std::set<std::tstring>		TstringSet, TStrSet;
typedef std::set<int>				IndexSet, IntSet;
typedef std::map<std::tstring, std::tstring>	TstringMap, TStrMap;
typedef std::vector<TStrMap>		VecTStrMap;

#ifndef _NAMO_KV_TYPEDEF_STRUCT
#define	_NAMO_KV_TYPEDEF_STRUCT
typedef struct _keyAndValue {
	std::tstring key;
	std::tstring value;
} keyAndValue, *pKeyAndValue;
typedef std::vector<keyAndValue>	KV_Vector;
#endif	// end of _NAMO_KV_TYPEDEF_STRUCT

#ifdef _UNICODE
#define _COUT	std::wcout
#define _tgetcwd	_wgetcwd
#define _STRINGSTREAM	std::wstringstream
#define integer2tstring	integer2wstring
#else
#define _COUT	std::cout
#define _tgetcwd	_getcwd
#define _STRINGSTREAM	std::stringstream
#define integer2tstring	integer2string
#endif

#pragma once
namespace Util {
class StringUtil
{
public:
	StringUtil(void)	{	setlocale(LC_ALL, "Korean");	};
	~StringUtil(void)	{};

	std::string tstring2string(std::tstring const& str, std::locale const& loc = std::locale("korean"))
	{
#ifdef _UNICODE
		return wstring2string(str, loc);
#else
		return str;
#endif
	}
	std::wstring tstring2wtring(std::tstring const& str, std::locale const& loc = std::locale("korean"))
	{
#ifdef _UNICODE
		return str;
#else
		return string2wstring(str, loc);
#endif
	}
	std::tstring string2tstring(std::string const& str, std::locale const& loc = std::locale("korean"))
	{
#ifdef _UNICODE
		return string2wstring(str, loc);
#else
		return str;
#endif
	}
	std::tstring wstring2tstring(std::wstring const& str, std::locale const& loc = std::locale("korean"))
	{
#ifdef _UNICODE
		return str;
#else
		return wstring2string(str, loc);
#endif
	}
	std::string wstring2string(std::wstring const& str, std::locale const& loc = std::locale("korean"))
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
	std::wstring string2wstring(std::string const& str, std::locale const& loc = std::locale("korean"))
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

	template <typename T>
	std::vector<T> getTokensWithVector(T str, T delimiters)
	{
		std::vector<T> tokensList;

		T::size_type lastPos = str.find_first_not_of(delimiters, 0);	// 맨 첫 글자가 구분자인 경우 무시
		T::size_type pos     = str.find_first_of(delimiters, lastPos);// 구분자가 아닌 첫 글자를 찾는다

		while (T::npos != pos || T::npos != lastPos) {
			tokensList.push_back(str.substr(lastPos, pos - lastPos));	// token을 찾았으니 vector에 추가한다
			lastPos = str.find_first_not_of(delimiters, pos);			// 구분자를 뛰어넘는다.  "not_of"에 주의하라
			pos = str.find_first_of(delimiters, lastPos);				// 다음 구분자가 아닌 글자를 찾는다
		}
		return tokensList;
	}

	template <typename integerT>
	std::string integer2string(integerT integerValue)
	{
		const int bufferSize = 1024;
		char buffer[bufferSize];
		_snprintf_s(buffer, bufferSize, "%d", integerValue);
		return buffer;
	}
	template <typename integerT>
	std::wstring integer2wstring(integerT integerValue, bool isUnsigned=false)
	{
		const int bufferSize = 1024;
		wchar_t buffer[bufferSize];
		int size = sizeof(integerT);

		if(size == 8) {
			if(isUnsigned) _snwprintf_s(buffer, bufferSize, L"%I64u", integerValue);
			else _snwprintf_s(buffer, bufferSize, L"%I64d", integerValue);
		}
		else {
			if(isUnsigned) _snwprintf_s(buffer, bufferSize, L"%u", integerValue);
			else _snwprintf_s(buffer, bufferSize, L"%d", integerValue);			
		}
		
		//wsprintfW(buffer, L"%d", integerValue);
		return buffer;
	}
	template <typename floatT>
	std::tstring float2tstring(floatT floatValue)
	{
		const int bufferSize = 1024;
		TCHAR buffer[bufferSize];
		_sntprintf_s(buffer, bufferSize, _T("%f"), floatValue);
		return buffer;
	}
	// tstring2integer()는 .c_str()과 _tstoi / _tstoi64 / _tstol를 이용할 수 있기 때문에 안만들었음
	// http://msdn.microsoft.com/en-us/library/hc25t012(VS.71).aspx 참고!

	// 템플릿으로 만들다 보니 인수가 많아졌음
	// lineMarks : 줄바꿈 기호(보통 : '\n'이지만 txt파일의 경우 '\r'이 필요)
	// equalMark : 등호 기호(key와 value를 구분하는 구분자)
	// delimiters: 등호 기호 + white space 문자
	//
	// 사용예) kvString2Map<std::string>(updateInfo, "\r\n", "=", "=\t\n\v\f\r ");
	template <typename T>
	std::map<T, T> kvString2Map(T content, T lineMarks, T equalMark, T delimiters)
	{
		std::map<T, T> aMap;

		std::vector<T> tokenVec;
		tokenVec = getTokensWithVector<T>(content,lineMarks);
		std::vector<T>::iterator It;
		for(It = tokenVec.begin(); It != tokenVec.end(); ++It) {
			T oneLine = *It;

			T::size_type eqMarkPos = oneLine.find(equalMark);
			if(eqMarkPos == T::npos) continue;	// K = V에서 '='가 없다면 다음 라인으로

			std::vector<T> kv = getTokensWithVector<T>(oneLine, delimiters);
			if(kv.size() < 2) continue;	// (K,V)구조가 아니라면 다음 라인처리
			T afterEquar = oneLine.substr(eqMarkPos+1);

			aMap[kv[0]] = afterEquar.substr(afterEquar.find(kv[1]));
		}	
		return aMap;
	}

private:

	// test code
#ifndef  NDEBUG
public:
	void testStringUtilAll()
	{
		testString();
		testGetTokensWithVectors();
		testInteger2string();
	}
	void testString()
	{
		std::tstring string1 = _T("string:");	// string와 wstring 전환을 위해 TEXT()나 _T() 매크로를 쓴다
		std::tstring string2 = _T("문자열");
		_COUT << string1 << string2 << std::endl;
	}
	void testGetTokensWithVectors()
	{
		std::vector<std::string> tokens;
		tokens = getTokensWithVector<std::string>("abc\n#bcd\n#edd\n", "#");

		std::vector<std::string>::iterator it;
		for(it = tokens.begin(); it != tokens.end() ; it++) {
			std::cout << *it;			// 토큰을 가지고 할 작업(출력)
		}
	}
	void testInteger2string()
	{
		_COUT << integer2tstring<long long>(9223372036854775807) << std::endl;
		//_COUT << int_to_string(9223372036854775807) << std::endl;
	}
#endif	/* end of NDEBUG */
};
} // namespace Util
#endif		// _STRING_UTIL_H_

/*
 * Copyright (c) 2009 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */
