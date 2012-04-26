// stdafx.cpp : source file that includes just the standard includes
//	SRT.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)


namespace App
{
	VOID GetFilename(__in const LPSYSTEMTIME st, __out LPTSTR filename)
	{
		wsprintf(filename, TEXT("%04d%02d%02d.txt"), st->wYear, st->wMonth, st->wDay);
	}
}	/* end of namespace App */

namespace Win
{
	LPTSTR TranslateErrorCode(DWORD dwError, LPTSTR msg, size_t size_of_msg)
	{
		HLOCAL hlocal = NULL;   // Buffer that gets the error message string

		// Use the default system locale since we look for Windows messages.
		// Note: this MAKELANGID combination has 0 as value
		DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
		BOOL fOk;

		// WinInet Library
		if(12000<=dwError && dwError<=12174) {
			HMODULE hDll = LoadLibraryEx(TEXT("wininet.dll"), NULL, 
				DONT_RESOLVE_DLL_REFERENCES);

			if (hDll != NULL) {
				fOk = FormatMessage(
					FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
					FORMAT_MESSAGE_ALLOCATE_BUFFER,
					hDll, dwError, systemLocale,
					(PTSTR) &hlocal, 0, NULL);
				FreeLibrary(hDll);
			}
		}
		// Others
		else {
			// Get the error code's textual description
			fOk = FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER, 
				NULL, dwError, systemLocale, 
				(PTSTR) &hlocal, 0, NULL);

			if (!fOk) {
				// Is it a network-related error?
				HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
					DONT_RESOLVE_DLL_REFERENCES);

				if (hDll != NULL) {
					fOk = FormatMessage(
						FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
						FORMAT_MESSAGE_ALLOCATE_BUFFER,
						hDll, dwError, systemLocale,
						(PTSTR) &hlocal, 0, NULL);
					FreeLibrary(hDll);
				}
			}
		}
		
		if (fOk && (hlocal != NULL)) {
			_tcscpy_s(msg, size_of_msg, (PCTSTR) LocalLock(hlocal));
			LocalFree(hlocal);
		} else {
			_tcscpy_s(msg, size_of_msg, TEXT("No text found for this error number."));
		}

		return msg;
	}
}	/* end of namespace Win */