/*++

Copyright (c) 2008  Sindoricoh
All rights reserved

Author: Namho Kim
Date : 2009 / 1 / 21

Module Name:

   GetLastError.h

Abstract:

    윈도우 API를 이용할 때 에러가 발생하면
	GetLastError() 함수를 이용해서 에러 코드를 가져올 수 있는데,
	정작 코드가 무엇을 의미하는지 찾아봐야 한다는 불편이 있기 때문에
	문자로 알 수 있게 하는 함수를 만들었음.

	윈도우 API로 제공되는 GetLastError()를 쓰기 위해서는 ::GetLastError(); 로 호출을 하고
	지금 이것을 사용할 때는 Error라는 네임스페이스를 사용해서
		Error::GetLastError(); 로 호출을 해서 사용하세요

--*/

#pragma once
#include <string>
#ifndef __NAMO_ADDED_TSTRING__
#define __NAMO_ADDED_TSTRING__
namespace std {
	typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> >	tstring;
	typedef basic_stringstream<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstringstream;
}
#endif

#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_  /* Prevent inclusion of winsock.h in windows.h */
#endif

#ifndef _WINDOWS_
#include <windows.h>
#endif

namespace Error
{
	inline std::wstring GetLastErrorW();
	inline std::string GetLastErrorA();

	inline std::tstring GetLastError()
	{
#ifdef _UNICODE
		return GetLastErrorW();
#else
		return GetLastErrorA();
#endif // !UNICODE
	}

	inline std::wstring GetLastErrorW()
	{
		// Retrieve the system error message for the last-error code
		LPWSTR pBuffer = NULL;
		DWORD dwLastError = ::GetLastError();	// Error namespace의 GetLastError()가 아닌 표준API의 GetLastError()를 호출

		DWORD res = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
			, NULL
			, dwLastError
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
			, (LPWSTR) &pBuffer
			, 0
			, NULL
			);

		std::wstring retMessage;
		if(res == 0) {
			retMessage = L"FormatMessage Error!";
		}
		else {
			retMessage = pBuffer;
			LocalFree(pBuffer);
		}

		return retMessage;
	}

	inline std::string GetLastErrorA()
	{
		// Retrieve the system error message for the last-error code
		LPSTR pBuffer = NULL;
		DWORD dwLastError = ::GetLastError();	// Error namespace의 GetLastError()가 아닌 표준API의 GetLastError()를 호출

		DWORD res = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
			, NULL
			, dwLastError
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
			, (LPSTR) &pBuffer
			, 0
			, NULL
			);

		std::string retMessage;
		if(res == 0) {
			retMessage = "FormatMessage Error!";
		}
		else {
			retMessage = pBuffer;
			LocalFree(pBuffer);
		}

		return retMessage;
	}
}	/* namespace Error */

/*
 * Copyright (c) 2009 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */
