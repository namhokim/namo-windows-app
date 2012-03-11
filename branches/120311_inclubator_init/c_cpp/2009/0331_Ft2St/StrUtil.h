#ifndef _STR_UTIL_H_
#define _STR_UTIL_H_
#include <sstream>
#include <vector>

namespace Util
{
	/* String to Number
	// Example)
	// input : str / output : i
	 std::string str("500");
	 int i = Util::str2num<char, int>(str);
	*/
	template <typename _Elem, typename _Num>
	inline _Num str2num(std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > str)
	{
		std::basic_istringstream<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > iss(str);
		_Num n;
		iss >> n;
		return n;
	}

	/* Character to String
	// Example)
	// input : 'w' / output : str
	 std::wstring str = Util::ch2str<wchar_t>('w');
	*/
	template <typename _Elem>
	inline std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> >
		ch2str(_Elem c)
	{
		const int charSz = 2;
		const int charLoc = 0;

		_Elem ch[charSz];
		memset(ch, 0, charSz * sizeof(_Elem));
		ch[charLoc] = c;
		return ch;
	}

	/* String divided by one character
	// Example.1
	// input : str, ',' / output : v
	 std::string str = "a,b,c,d,e";
	 std::vector<std::string> v = Util::getTokenVec(str, ',');

	// Example.2
	// input : "a,b,c,d,e", ',' / output : v
	 std::vector<std::string> v = Util::getTokenVec<char>("a,b,c,d,e", ',');
	 */
	template <typename _Elem>
	inline std::vector<std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > >
		getTokenVec(std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > str,
					_Elem delimiter)
	{
		return Util::getTokenVec(str, Util::ch2str(delimiter));
	}

	/* String divided by some characters
	// Example.1
	// input : str, ",:" / output : v
	 std::string str = "a,b:c,d,e";
	 std::vector<std::string> v = Util::getTokenVec(str, ",:");

	// Example.2
	// input : "a,b:c,d,e", ",:" / output : v
	 std::vector<std::string> v = Util::getTokenVec<char>("a,b:c,d,e", ",:");
	 */
	template <typename _Elem>
	inline std::vector<std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > >
		getTokenVec(std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > str,
					std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > delimiters)
	{
		std::vector<std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > > tokVecRet;	// 반환 벡터
		std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> >::size_type lastPos;
		std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> >::size_type pos;	

		lastPos = str.find_first_not_of(delimiters, 0);	// 맨 첫 글자가 구분자인 경우 무시
		pos     = str.find_first_of(delimiters, lastPos);// 구분자가 아닌 첫 글자를 찾는다

		while (std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> >::npos != pos
			|| std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> >::npos != lastPos) {
			tokVecRet.push_back(str.substr(lastPos, pos - lastPos));	// token을 찾았으니 vector에 추가한다
			lastPos = str.find_first_not_of(delimiters, pos);			// 구분자를 뛰어넘는다.  "not_of"에 주의하라
			pos = str.find_first_of(delimiters, lastPos);				// 다음 구분자가 아닌 글자를 찾는다
		}
		return tokVecRet;
	}
}
#endif

/*
 * Copyright (c) 2009.03.16 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */
