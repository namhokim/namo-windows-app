#include "PercentEncoding.h"
#include <cctype>	// for isalpha

namespace namo
{
	// RFC 3986 section 2.3 Unreserved Characters
	inline bool IsUnreservedCharacter(unsigned char uc)
	{
		if (::isalpha(uc) || ::isdigit(uc)
			|| '-'==uc || '_'==uc || '.'==uc || '~'==uc) {
				return true;
		} else {
			return false;
		}
	}

	std::string ToPercentEncoding(const std::string& str)
	{
		std::string strBuf;
		strBuf.reserve(str.size());	// 출력되는 문자열 버퍼는 입력과 같거나 크다
		
		std::string::size_type pos, size=str.size();
		char cBuf[4];
		for(pos=0; pos<size; ++pos) {
			unsigned char uc = (unsigned char)str[pos];
			if(IsUnreservedCharacter(uc)) {
				sprintf_s(cBuf, 4, "%c", uc); 
			} else {	// RFC 3986 section 2.2 Reserved Characters
				sprintf_s(cBuf, 4, "%%%02X", uc);
			}
			strBuf.append(cBuf);
		}

		return strBuf;
	}

	std::string URIEscape(const std::string& uri)
	{
		std::string strBuf;
		strBuf.reserve(uri.size());	// 출력되는 문자열 버퍼는 입력과 같거나 크다

		std::string::size_type pos, size=uri.size();
		char cBuf[4];
		for(pos=0; pos<size; ++pos) {
			unsigned char uc = (unsigned char)uri.at(pos);
			if (::isalpha(uc) || ::isdigit(uc)
				|| ('$'==uc) || ('-'==uc) || ('_'==uc) || ('.'==uc)
				|| ('+'==uc) || ('!'==uc) || ('*'==uc) || ('\''==uc)
				|| ('('==uc) || (')'==uc) || (','==uc) )
			{
				sprintf_s(cBuf, 4, "%c", uc);
			}
			else
			{
				if(0==uc) continue;	// escape NULL character
				sprintf_s(cBuf, 4, "%%%02X", uc);
			}
			strBuf.append(cBuf);
		}

		return strBuf;
	}

}	/* end of namespace namo */