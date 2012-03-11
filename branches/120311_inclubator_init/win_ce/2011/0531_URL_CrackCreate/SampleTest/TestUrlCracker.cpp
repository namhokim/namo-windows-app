#include <gtest/gtest.h>

// user library for TEST
#include <Windows.h>
#include <WinInet.h>
#include <tchar.h>

#pragma comment(lib, "Wininet.lib")

TEST (TestUrlCracker, TestCrackingAndAssemble) {
	// IN
	LPCTSTR lpszUrl = _T("http://www.google.co.kr/search?q=Å×½ºÆ®");
	LPCTSTR lpszUrld = _T("http://10.16.11.19:8081/free4ws/touch/12-34-56-78-90-AB/123456789?q=1234");
	
	// OUT
	TCHAR scheme[INTERNET_MAX_SCHEME_LENGTH + 1];
	TCHAR hostname[INTERNET_MAX_HOST_NAME_LENGTH + 1];
	TCHAR urlpath[INTERNET_MAX_PATH_LENGTH + 1];
	TCHAR extra[INTERNET_MAX_PATH_LENGTH + 1];

	// OUT: container
	URL_COMPONENTS uc;
	ZeroMemory(&uc, sizeof(uc));

	uc.dwStructSize = sizeof(uc);
	uc.dwSchemeLength		= INTERNET_MAX_SCHEME_LENGTH;
	uc.dwHostNameLength		= INTERNET_MAX_HOST_NAME_LENGTH;
	uc.dwUrlPathLength		= INTERNET_MAX_PATH_LENGTH;
	uc.dwExtraInfoLength	= INTERNET_MAX_PATH_LENGTH;

	uc.lpszScheme = scheme;
	uc.lpszHostName = hostname;
	uc.lpszUrlPath = urlpath;
	uc.lpszExtraInfo = extra;

	/// Cracking
	// http://msdn.microsoft.com/en-us/library/aa920361.aspx
	BOOL bRes = ::InternetCrackUrl(lpszUrl, lstrlen(lpszUrl), ICU_DECODE, &uc);

	// TEST SETs
	ASSERT_EQ( TRUE, bRes ) << "GetLastError() : " << ::GetLastError();
	EXPECT_STREQ( _T("http"), scheme );
	EXPECT_STRNE( _T("https"), scheme );
	EXPECT_STREQ( _T("10.16.11.19"), hostname );
	EXPECT_STRNE( _T("10.16.11.191"), hostname );
	EXPECT_STREQ( _T("/free4ws/touch/12-34-56-78-90-AB/123456789"), urlpath );
	EXPECT_STRNE( _T("/free4ws/touch/12-34-56-78-90-AB/123456789/"), urlpath );
	EXPECT_STREQ( _T("?q=123"), extra );
	EXPECT_STRNE( _T("?q=1234"), extra );

	/// Assemble
	TCHAR url[INTERNET_MAX_URL_LENGTH];
	DWORD dwLen = INTERNET_MAX_URL_LENGTH;
	// http://msdn.microsoft.com/en-us/library/aa920323.aspx
	bRes = ::InternetCreateUrl(&uc, ICU_ESCAPE, url, &dwLen);

	// TEST SETs
	ASSERT_EQ( TRUE, bRes ) << "GetLastError() : " << ::GetLastError();
	EXPECT_STREQ( lpszUrl, url );
	EXPECT_STRNE( lpszUrld, url );
}
