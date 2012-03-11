#ifndef __NAMO_UNICODE_SELECTOR_H__
#define __NAMO_UNICODE_SELECTOR_H__

#include <string>	// for _STRING
#include <iostream>	// for _COUT
#include <direct.h>	// for _TGETCWD
#include <tchar.h>	// for _T

#ifdef _UNICODE
#define _COUT	std::wcout
#define _STRING	std::wstring
#define _TGETCWD	_wgetcwd
#else
#define _COUT	std::cout
#define _STRING	std::string
#define _TGETCWD	_getcwd
#endif
#endif