#pragma once

#define	WM_UPDATE_VIEW	(WM_USER+2)

#define	TYPE_LIMIT_BY_UNKNOWN			0
#define	TYPE_LIMIT_BY_DAILY_QUOTA		1
#define	TYPE_LIMIT_BY_MONTHLY_QUOTA		2

#include <string>
#include <map>
typedef struct _USER_INFOA
{
	std::string card_number;
	DWORD		dwQuota;
	DWORD		dwUsed;
	DWORD		dwLimitType;
	std::string touch_time;
} USER_INFOA, *LPUSER_INFOA;

typedef struct _USER_INFOW
{
	std::wstring card_number;
	DWORD		dwQuota;
	DWORD		dwUsed;
	DWORD		dwLimitType;
	std::wstring touch_time;
} USER_INFOW, *LPUSER_INFOW;

#ifdef UNICODE
#define USER_INFO  USER_INFOW
#define LPUSER_INFO  LPUSER_INFOW
#else
#define USER_INFO  USER_INFOA
#define LPUSER_INFO  LPUSER_INFOA
#endif // !UNICODE


typedef struct _CONF_INFO
{
	DWORD		dwLoginTime;
	DWORD		dwDispLogoTime;
	DWORD		dwDispExceedQuataScreen;
} CONF_INFO, *LPCONF_INFO;
//typedef struct _CONF_INFOA
//{
//	std::string card_number;
//	DWORD		dwQuota;
//	DWORD		dwUsed;
//	std::string touch_time;
//} CONF_INFOA, *LPCONF_INFOA;
//
//typedef struct _CONF_INFOW
//{
//	std::wstring card_number;
//	DWORD		dwQuota;
//	DWORD		dwUsed;
//	std::wstring touch_time;
//} CONF_INFOW, *LPCONF_INFOW;
//
//#ifdef UNICODE
//#define CONF_INFO  CONF_INFOW
//#define LPCONF_INFO  LPCONF_INFOW
//#else
//#define CONF_INFO  CONF_INFOA
//#define LPCONF_INFO  LPCONF_INFOA
//#endif // !UNICODE

namespace std
{
	typedef basic_string<TCHAR, char_traits<TCHAR>,
		allocator<TCHAR> > tstring;
}

namespace DB
{
	typedef std::map<std::tstring, USER_INFO> UserInfoDB;

	const UserInfoDB* GetDB();
}

namespace WebServer
{
	extern HWND hWnd;

	BOOL GetUserInfo(LPUSER_INFO);
	BOOL SetUserInfo(CONST LPUSER_INFO);
	BOOL GetConfigurationInfo(LPCONF_INFO lpConfInfo);
}