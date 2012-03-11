/******************************************************************************
Module:  ServiceImpl.h
Notices: Copyright (c) 2010 Namho Kim
Purpose: 서비스 구현 헤더
See : botton of this code
******************************************************************************/

#pragma once   // Include this header file once per compilation unit

///////////////////////////////////////////////////////////////////////////////
// 서비스 체크용 이벤트 핸들
//  사용시 Windows.h헤더를 추가하고 HANDLE 타입으로 캐스팅하여
// WaitForSingleObject() 함수를 이용한다
//
//  정의 위치 : ServiceMain.cpp
//
extern void *ghSvcStopEvent;	// 서비스 정지
extern void *ghSvcPauseEvent;	// 일시 정지

///////////////////////////////////////////////////////////////////////////////
// 서비스 시그널 체크용 함수
//
//  정의 위치 : ServiceMain.cpp
//
//  서비스 구현시 수행 시간이 오래 걸리는 구간은 이 함수를 이용해서
// 서비스 종료 신호시 가급적 신속하게 반환하도록 해야한다.
//
bool isStopSignaled();	// 서비스 정지
bool isPauseSignaled();	// 일시 정지

///////////////////////////////////////////////////////////////////////////////
// ServiceMain.cpp에서 호출하는 인터페이스
//
//
void DoService();

///////////////////////////////////////////////////////////////////////////////


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
