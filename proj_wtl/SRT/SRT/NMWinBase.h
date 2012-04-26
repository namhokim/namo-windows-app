/************************************************************************
*                                                                       *
*   NMWinBase.h -- For Utilities of Windows Base APIs                   *
*                                                                       *
*   Author : Namho Kim (http://wapi.springnote.com)                     *
*                                                                       *
************************************************************************/
#ifndef _NAMO_WINBASE_
#define _NAMO_WINBASE_

#if _MSC_VER > 1000
#pragma once
#endif

DWORD GetModuleFilePathA(
    __out_ecount_part(nSize, return + 1) LPCH lpFilepath,
    __in     DWORD nSize
	);

DWORD GetModuleFilePathW(
    __out_ecount_part(nSize, return + 1) LPWCH lpFilepath,
    __in     DWORD nSize
	);

#ifdef UNICODE
#define GetModuleFilePath  GetModuleFilePathW
#else
#define GetModuleFilePath  GetModuleFilePathA
#endif // !UNICODE

#endif	/* end of _NAMO_WINBASE_ */