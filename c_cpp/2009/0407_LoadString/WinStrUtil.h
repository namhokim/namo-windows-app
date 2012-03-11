#ifndef _WIN_STR_UTIL_H_
#define _WIN_STR_UTIL_H_
#include <windows.h>
#include <string>

#ifndef _TSTRING_H_
#define _TSTRING_H_

#include <string>
#include <tchar.h>	// for _T

namespace std {
	typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> >	tstring;
	typedef basic_stringstream<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstringstream;
}
#endif	// _TSTRING_H_

namespace Util
{
#ifdef _TSTRING_H_
	std::tstring LoadString(UINT uID, HINSTANCE hInstance = NULL)
	{
		const int CharFudge = 1;
		const int GrowingSize = 256;
		// try fixed buffer first (to avoid wasting space in the heap)
		TCHAR szTemp[GrowingSize];
		int nCount =  sizeof(szTemp) / sizeof(szTemp[0]);
		int nLen = ::LoadString(NULL, uID, szTemp, nCount);
		if (nCount - nLen > CharFudge)
		{
			return szTemp;
		}

		// try buffer size of Double GrowingSize(512), then larger size until entire string is retrieved
		int nSize = GrowingSize;
		TCHAR *buf = NULL;
		std::tstring str;
		do
		{
			nSize += GrowingSize;
			if(buf != NULL) delete []buf;
			buf = new TCHAR[nSize];
			if(buf == NULL)	// alloc fail
			{
				nLen = 0;
				break;
			}
			nLen = ::LoadString(NULL, uID, buf, nSize);
		} while (nSize - nLen <= CharFudge);
		str.assign(buf);
		if(buf != NULL) delete []buf;
		return str;
	}
#endif	// namespace Util
}
#endif	// _WIN_STR_UTIL_H_