#pragma once

#include <cassert>	// forassert

// refs. http://lunarblade.tistory.com/tag/UNICODE
#define WIDEN(x) L ## x
#define WIDEN2(x) WIDEN(x)
#define __WFILE__ WIDEN2(__FILE__)	// for UNICODE

// size_t has the same with as pointers on both 32-bit and 64-bit
// Windows platforms.
// See http://msdn.microsoft.com/en-us/library/tcxf1dw6.aspx.
size_t SplitLastBackslashBefore(
	const wchar_t* in, wchar_t* out);

size_t SplitLastBackslashBefore(
	const char* in, char* out);

//
// Purpose: 
//   Get directory which located execute file
//
// Parameters:
//   Output : directory name
// 
// Return value:
//   ERROR_SUCCESS(0), ERROR_INSUFFICIENT_BUFFER(122)
//
size_t GetExeDirectory(wchar_t *szDir);
size_t GetExeDirectory(char *szDir);


//
// Purpose: 
//   Get filename which located execute file
//
// Parameters:
//   Output : filename
// 
// Return value:
//   ERROR_SUCCESS(0), ERROR_INSUFFICIENT_BUFFER(122)
//
unsigned long GetExeFilename(wchar_t *szFilename);
unsigned long GetExeFilename(char *szFilename);
