#pragma once

#include "resource.h"

VOID Usage();
VOID MekanderV(LPCTSTR url);
VOID MekanderInet(int id, LPCTSTR url);
BOOL MekanderSocket(int id, LPCSTR host, unsigned short port, LPCSTR object);

VOID CALLBACK CallBack(
		__in HINTERNET hInternet,
		__in DWORD_PTR dwContext,
		__in DWORD dwInternetStatus,
		__in_bcount(dwStatusInformationLength) LPVOID lpvStatusInformation,
		__in DWORD dwStatusInformationLength
		);


typedef struct _NOTIFY_CONTEXT {
	int id;
	HINTERNET hFile;
	INTERNET_BUFFERS lpIB;
	BYTE buf[1024];
	BOOL bNotRead;

} NOTIFY_CONTEXT, *PNOTIFY_CONTEXT;