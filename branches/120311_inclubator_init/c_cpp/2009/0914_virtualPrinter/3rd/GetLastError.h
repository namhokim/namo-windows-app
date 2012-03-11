/*++

Copyright (c) 2008  Sindoricoh
All rights reserved

Author: Namho Kim
Date : 2009 / 1 / 21

Module Name:

   GetLastError.h

Abstract:

    ������ API�� �̿��� �� ������ �߻��ϸ�
	GetLastError() �Լ��� �̿��ؼ� ���� �ڵ带 ������ �� �ִµ�,
	���� �ڵ尡 ������ �ǹ��ϴ��� ã�ƺ��� �Ѵٴ� ������ �ֱ� ������
	���ڷ� �� �� �ְ� �ϴ� �Լ��� �������.

	������ API�� �����Ǵ� GetLastError()�� ���� ���ؼ��� ::GetLastError(); �� ȣ���� �ϰ�
	���� �̰��� ����� ���� Error��� ���ӽ����̽��� ����ؼ�
		Error::GetLastError(); �� ȣ���� �ؼ� ����ϼ���

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
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
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
		DWORD dwLastError = ::GetLastError();	// Error namespace�� GetLastError()�� �ƴ� ǥ��API�� GetLastError()�� ȣ��

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
		DWORD dwLastError = ::GetLastError();	// Error namespace�� GetLastError()�� �ƴ� ǥ��API�� GetLastError()�� ȣ��

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
