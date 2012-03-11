/******************************************************************************
Module:  ServiceMain.h
Notices: Copyright (c) 2010 Namho Kim
Purpose: 서비스 공통 헤더
See : botton of this code
******************************************************************************/

#pragma once   // Include this header file once per compilation unit

#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

///////////////////////////////////////////////////////////////////////////////
// 서비스 정보 설정
#define SVC_NAME			TEXT("SvcName")				 // 서비스 이름
#define SVC_DISPLAY_NAME	TEXT("Service Display Name") // 서비스 표시명(최대 256자)
#define SVC_DESCRIPTION		TEXT("서비스의 설명을 여기에 기술합니다.")	// 설명(최대 1024자)

// 서비스 동작 유형
// SERVICE_AUTO_START	: 자동
// SERVICE_DEMAND_START	: 수동
// SERVICE_DISABLED		: 사용안함
#define SVC_START_TYPE		SERVICE_DEMAND_START	// 수동

// 서비스 설치후 자동 시행 여부(TRUE/FALSE)
#define SVC_START_AFTER_INSTALL		FALSE

// 서비스 제거전 중지 시행 여부(TRUE/FALSE)
#define SVC_STOP_BEFORE_UNINSTALL	TRUE

///////////////////////////////////////////////////////////////////////////////
// MessageId: SVC_ERROR
//
// MessageText:
//
//  An error has occurred (%2).
//  
//
#define SVC_ERROR                        ((DWORD)0xC0020001L)

///////////////////////////////////////////////////////////////////////////////
// in ServiceConfig.cpp
#define	PROVIDE_SERVICE_CONFIG		// 설치, 제거를 포함(미포함하게 하려면 주석)

#ifdef	PROVIDE_SERVICE_CONFIG
VOID SvcInstall();			// 서비스 설치함수
VOID SvcUninstall();		// 서비스 제거함수
#endif	/* PROVIDE_SERVICE_CONFIG */

VOID SvcReportEvent(		// 서비스 리포트 함수
	__in LPTSTR szFunction
	);

///////////////////////////////////////////////////////////////////////////////
// in ServiceMain.cpp
extern SERVICE_STATUS          gSvcStatus; 
extern SERVICE_STATUS_HANDLE   gSvcStatusHandle; 

VOID WINAPI ServiceMain(	// 서비스 시작함수
	__in  DWORD dwArgc,
	__in  LPTSTR *lpszArgv
);


/*
Copyright (c) 2010  Namho Kim

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */