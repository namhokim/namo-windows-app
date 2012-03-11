/******************************************************************************
Module:  ServiceMain.h
Notices: Copyright (c) 2010 Namho Kim
Purpose: ���� ���� ���
See : botton of this code
******************************************************************************/

#pragma once   // Include this header file once per compilation unit

#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

///////////////////////////////////////////////////////////////////////////////
// ���� ���� ����
#define SVC_NAME			TEXT("SvcName")				 // ���� �̸�
#define SVC_DISPLAY_NAME	TEXT("Service Display Name") // ���� ǥ�ø�(�ִ� 256��)
#define SVC_DESCRIPTION		TEXT("������ ������ ���⿡ ����մϴ�.")	// ����(�ִ� 1024��)

// ���� ���� ����
// SERVICE_AUTO_START	: �ڵ�
// SERVICE_DEMAND_START	: ����
// SERVICE_DISABLED		: ������
#define SVC_START_TYPE		SERVICE_DEMAND_START	// ����

// ���� ��ġ�� �ڵ� ���� ����(TRUE/FALSE)
#define SVC_START_AFTER_INSTALL		FALSE

// ���� ������ ���� ���� ����(TRUE/FALSE)
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
#define	PROVIDE_SERVICE_CONFIG		// ��ġ, ���Ÿ� ����(�������ϰ� �Ϸ��� �ּ�)

#ifdef	PROVIDE_SERVICE_CONFIG
VOID SvcInstall();			// ���� ��ġ�Լ�
VOID SvcUninstall();		// ���� �����Լ�
#endif	/* PROVIDE_SERVICE_CONFIG */

VOID SvcReportEvent(		// ���� ����Ʈ �Լ�
	__in LPTSTR szFunction
	);

///////////////////////////////////////////////////////////////////////////////
// in ServiceMain.cpp
extern SERVICE_STATUS          gSvcStatus; 
extern SERVICE_STATUS_HANDLE   gSvcStatusHandle; 

VOID WINAPI ServiceMain(	// ���� �����Լ�
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