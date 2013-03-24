// refs. vadefs.h (_W64)
#if !defined(_W64_NAMO)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64_NAMO __w64
#else
#define _W64_NAMO
#endif
#endif

// refs. vadefs.h (uintptr_t)
#ifndef _ENV_POINTER_DEFINED
#ifdef  _WIN64
typedef unsigned __int64    env_u_ptr;
#else
typedef _W64_NAMO unsigned int   env_u_ptr;
#endif
#define _ENV_POINTER_DEFINED
#endif

#include <assert.h>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

template <typename T>
size_t SplitLastBackslashBefore(
	const T* in, T* out, T Sep, T EndMark)
{
	assert(in);
	assert(out);

	bool bExist = false;
	T * cp = out;			// Backup
	while( *cp = *in ) {	// Copy (in to out)
		T ch = *in;
		if(!bExist && ch==Sep) bExist = true;
		cp++;	in++;
	}
	if(bExist) {
		while( Sep != *cp ) --cp;
		*cp = EndMark;
	}
	return ((cp-out)/sizeof(T));
}

size_t SplitLastBackslashBefore(
	const wchar_t* in, wchar_t* out)
{
	return SplitLastBackslashBefore<wchar_t>(in, out, L'\\', L'\0');
}

size_t SplitLastBackslashBefore(
	const char* in, char* out)
{
	return SplitLastBackslashBefore<char>(in, out, '\\', '\0');
}

size_t GetExeDirectory(wchar_t *szDir)
{
	wchar_t szPath[MAX_PATH] = {0,};
	DWORD dwRes;

	// Get Exe file path
	// length of the string that is copied  or 
	dwRes = ::GetModuleFileNameW(NULL, szPath, MAX_PATH);
	if(dwRes>=MAX_PATH) return dwRes;

	return SplitLastBackslashBefore(szPath, szDir);
}

size_t GetExeDirectory(char *szDir)
{
	char szPath[MAX_PATH] = {0,};
	DWORD dwRes;

	// Get Exe file path
	// length of the string that is copied  or 
	dwRes = ::GetModuleFileNameA(NULL, szPath, MAX_PATH);
	if(dwRes>=MAX_PATH) return dwRes;

	return SplitLastBackslashBefore(szPath, szDir);
}

unsigned long GetExeFilename(wchar_t *szFilename)
{
	TCHAR szPath[MAX_PATH] = {0,};
	PTCHAR pch, pDest;
	env_u_ptr pos_ptr;
	DWORD dwRes;
	
	pos_ptr = 0;

	// Get Exe file path
	dwRes = ::GetModuleFileName(NULL, szPath, MAX_PATH);
	if(dwRes>=MAX_PATH) return dwRes;

	// Find the last of backslash
	pch = szPath;
	while( *pch ) {
		if(TEXT('\\') == (*pch)) {
			pos_ptr = pch - szPath;
		};
		++pch;
	}

	// Copy
	pch = szPath + pos_ptr + 1;
	pDest = szFilename;
	while( ( (*pDest)=(*pch) ) ) {
		if(TEXT('\\') == (*pch)) {
			pos_ptr = pch - szPath;
		}
		++pDest;
		++pch;
	}

	return dwRes;
}

unsigned long GetExeFilename(char *szFilename)
{
	char szPath[MAX_PATH] = {0,};
	PCHAR pch, pDest;
	env_u_ptr pos_ptr;
	DWORD dwRes;
	
	pos_ptr = 0;

	// Get Exe file path
	dwRes = ::GetModuleFileNameA(NULL, szPath, MAX_PATH);
	if(dwRes>=MAX_PATH) return dwRes;

	// Find the last of backslash
	pch = szPath;
	while( *pch ) {
		if('\\' == (*pch)) {
			pos_ptr = pch - szPath;
		};
		++pch;
	}

	// Copy
	pch = szPath + pos_ptr + 1;
	pDest = szFilename;
	while( ( (*pDest)=(*pch) ) ) {
		if('\\' == (*pch)) {
			pos_ptr = pch - szPath;
		}
		++pDest;
		++pch;
	}

	return dwRes;
}
