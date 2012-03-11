#pragma once
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_  /* Prevent inclusion of winsock.h in windows.h */
#endif
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif
#include <windows.h>
#include <string>

// for test code
#ifndef  NDEBUG
#include <iostream>
#endif

#ifdef UNICODE
#define getDateStringFromSYSTEMTIME		getDateStringFromSYSTEMTIMEW
#else
#define getDateStringFromSYSTEMTIME  getDateStringFromSYSTEMTIMEA
#endif // !UNICODE

/**
	Author : Namho Kim
	Date : 2009.01.06
	Desc : Collection of utilities of using SYSTEMTIME, FILETIME (defined in winbase.h <- windows.h)
*/
namespace Util {
class TimeUtil
{
public:
	enum TIME_OPERATORS
	{
		PLUS	= 0,
		MINUS	= 1,
		TIMES	= 2,
		DIVIDE	= 3,
		LESSorEQUAR = 4
	};

	TimeUtil(void) {}
	~TimeUtil(void) {}

	// SYSTEMTIME 구조체로 현재시간 가져오기
	SYSTEMTIME getCurrentLocalTime()
	{
		SYSTEMTIME currentTimeST;
		GetLocalTime(&currentTimeST);
		return currentTimeST;
	}
	// FILETIME 구조체로 현재시간 가져오기
	FILETIME getCurrentLocalTimeFT()
	{
		SYSTEMTIME st = getCurrentLocalTime();
		FILETIME ft;
		::SystemTimeToFileTime(&st, &ft);
		return ft;
	}
	// 날짜를 std::string으로 만들기 (SYSTEMTIME 구조체)
	std::string getDateStringFromSYSTEMTIMEA(const LPSYSTEMTIME lpSt)
	{
		char date[4+1+2+1+2+1];	// yyyy/mm/dd(null)
		wsprintfA(date, "%04d/%02d/%02d", lpSt->wYear, lpSt->wMonth, lpSt->wDay);
		return date;
	}
	// 시간을 std::string으로 만들기 (SYSTEMTIME 구조체)
	std::string getTimeStringFromSYSTEMTIMEA(const LPSYSTEMTIME lpSt)
	{
		char time[2+1+2+1+2+1];	// hh:mm:ss(null)
		wsprintfA(time, "%02d:%02d:%02d", lpSt->wHour, lpSt->wMinute, lpSt->wSecond);
		return time;
	}
	// 날짜를 std::wstring으로 만들기 (SYSTEMTIME 구조체)
	std::wstring getDateStringFromSYSTEMTIMEW(const LPSYSTEMTIME lpSt)
	{
		wchar_t date[4+1+2+1+2+1];
		wsprintfW(date, L"%04d/%02d/%02d", lpSt->wYear, lpSt->wMonth, lpSt->wDay);
		return date;
	}
	// 시간을 std::wstring으로 만들기 (SYSTEMTIME 구조체)
	std::wstring getTimeStringFromSYSTEMTIMEW(const LPSYSTEMTIME lpSt)
	{
		wchar_t time[2+1+2+1+2+1];
		wsprintfW(time, L"%02d:%02d:%02d", lpSt->wHour, lpSt->wMinute, lpSt->wSecond);
		return time;
	}

	// SYSTEMTIME 더하기1/2 (SYSTEMTIME ± UINT)
	SYSTEMTIME addSYSTEMTIME		(const LPSYSTEMTIME lpSt, const UINT second)		{	return operatingSYSTEMTIME(lpSt, PLUS, second);	}
	// SYSTEMTIME 더하기2/2 (SYSTEMTIME ± SYSTEMTIME)
	SYSTEMTIME addSYSTEMTIME		(const LPSYSTEMTIME lpSt1, const LPSYSTEMTIME lpSt2){	return operatingSYSTEMTIME(lpSt1, PLUS, lpSt2);	}
	// SYSTEMTIME 빼기1/2 (SYSTEMTIME ± UINT)
	SYSTEMTIME substractSYSTEMTIME	(const LPSYSTEMTIME lpSt1, const LPSYSTEMTIME lpSt2){	return operatingSYSTEMTIME(lpSt1, MINUS, lpSt2);}
	// SYSTEMTIME 빼기2/2 (SYSTEMTIME ± SYSTEMTIME)
	SYSTEMTIME substractSYSTEMTIME	(const LPSYSTEMTIME lpSt, const UINT second)		{	return operatingSYSTEMTIME(lpSt, MINUS, second);}
	
	// FILETIME 더하기1/2 (FILETIME ± UINT)
	FILETIME addFILETIME		(const LPFILETIME lpFt, const UINT second)		{	return operatingFILETIME(lpFt, PLUS, second);	}
	// FILETIME 더하기2/2 (FILETIME ± FILETIME)
	FILETIME addFILETIME		(const LPFILETIME lpFt1, const LPFILETIME lpFt2){	return operatingFILETIME(lpFt1, PLUS, lpFt2);	}
	// FILETIME 빼기1/2 (FILETIME ± UINT)
	FILETIME substractFILETIME	(const LPFILETIME lpFt, const UINT second)		{	return operatingFILETIME(lpFt, MINUS, second);	}
	// FILETIME 빼기1/2 (FILETIME ± FILETIME)
	FILETIME substractFILETIME	(const LPFILETIME lpFt1, const LPFILETIME lpFt2){	return operatingFILETIME(lpFt1, MINUS, lpFt2);	}

	// SYSTEMTIME1 <= SYSTEMTIME2 ?
	bool isLessEqualSYSTEMTIME(const LPSYSTEMTIME lpSt1, const LPSYSTEMTIME lpSt2)
	{
		bool res;
		operatingSYSTEMTIME(lpSt1, LESSorEQUAR, lpSt2, &res);
		return res;
	}
	// FILETIME1 <= FILETIME2 ?
	bool isLessEqualFILETIME(const LPFILETIME lpFt1, const LPFILETIME lpFt2)
	{
		bool res;
		operatingFILETIME(lpFt1, LESSorEQUAR, lpFt2, &res);
		return res;
	}

private:
	// 변환 : FILETIME -> ULARGE_INTEGER
	ULARGE_INTEGER FILETIME2ULARGE_INTEGER(const LPFILETIME lpFt)
	{
		ULARGE_INTEGER uInt;
		uInt.HighPart	= lpFt->dwHighDateTime;
		uInt.LowPart	= lpFt->dwLowDateTime;
		return uInt;
	}
	// 변환 : ULARGE_INTEGER -> FILETIME
	FILETIME ULARGE_INTEGER2FILETIME(const PULARGE_INTEGER pLi)
	{
		FILETIME ft;
		ft.dwHighDateTime	= pLi->HighPart;
		ft.dwLowDateTime	= pLi->LowPart;
		return ft;
	}

	// 연산 : SYSTEMTIME과 UINT 간
	SYSTEMTIME operatingSYSTEMTIME(const LPSYSTEMTIME lpSt, int op, const UINT second)
	{
		FILETIME ft, ftResult;
		if(SystemTimeToFileTime(lpSt, &ft) == 0) memset(&ft, 0, sizeof(FILETIME));
		
		switch(op) {
		case PLUS:	ftResult = addFILETIME(&ft, second);		break;
		case MINUS:	ftResult = substractFILETIME(&ft, second);	break;
		default:	ftResult = ft;
		}

		SYSTEMTIME resST;
		if(FileTimeToSystemTime(&ftResult, &resST) == 0) memset(&resST, 0, sizeof(SYSTEMTIME));
		return resST;
	}
	// 연산 : SYSTEMTIME 간
	SYSTEMTIME operatingSYSTEMTIME(const LPSYSTEMTIME lpSt1, int op, const LPSYSTEMTIME lpSt2, bool *isTrue=false)
	{
		FILETIME ft1, ft2, ftResult;
		if(SystemTimeToFileTime(lpSt1, &ft1) == 0
			|| SystemTimeToFileTime(lpSt2, &ft2) == 0) {
			memset(&ft1, 0, sizeof(FILETIME));
			memset(&ft2, 0, sizeof(FILETIME));
		}
		switch(op) {
		case PLUS:			ftResult = addFILETIME(&ft1, &ft2);			break;
		case MINUS:			ftResult = substractFILETIME(&ft1, &ft2);	break;
		case LESSorEQUAR:	*isTrue = (isLessEqualFILETIME(&ft1,&ft2));	// pass to default
		default:
			ftResult = ft1;
		}
		SYSTEMTIME resST;
		if(FileTimeToSystemTime(&ftResult, &resST) == 0) memset(&resST, 0, sizeof(SYSTEMTIME));
		return resST;
	}

	// 연산 :FILETIME과 UINT 간
	FILETIME operatingFILETIME(const LPFILETIME lpFt, int op, const UINT second)
	{
		const int unitOneSecondInFILETIME = 10000000;

		ULARGE_INTEGER uInt = FILETIME2ULARGE_INTEGER(lpFt);
		ULARGE_INTEGER uIntSecond, uIntResult;

		uIntSecond.QuadPart = second;
		uIntSecond.QuadPart = uIntSecond.QuadPart * unitOneSecondInFILETIME;

		switch(op) {
		case PLUS:	uIntResult.QuadPart = uInt.QuadPart + uIntSecond.QuadPart;	break;
		case MINUS:	uIntResult.QuadPart = uInt.QuadPart - uIntSecond.QuadPart;	break;
		default:	uIntResult.QuadPart = 0;
		}
		return ULARGE_INTEGER2FILETIME(&uIntResult);
	}
	// 연산 : FILETIME 간
	FILETIME operatingFILETIME(const LPFILETIME lpFt1, int op, const LPFILETIME lpFt2, bool *isTrue=false)
	{
		ULARGE_INTEGER uInt1 = FILETIME2ULARGE_INTEGER(lpFt1);
		ULARGE_INTEGER uInt2 = FILETIME2ULARGE_INTEGER(lpFt2);
		ULARGE_INTEGER uIntResult;

		switch(op) {
		case PLUS:			uIntResult.QuadPart = uInt1.QuadPart + uInt2.QuadPart;	break;
		case MINUS:			uIntResult.QuadPart = uInt1.QuadPart - uInt2.QuadPart;	break;
		case LESSorEQUAR:	*isTrue = (uInt1.QuadPart <= uInt2.QuadPart);	// pass to default
		default:			uIntResult.QuadPart = 0;
		}

		return ULARGE_INTEGER2FILETIME(&uIntResult);
	}

#ifndef  NDEBUG
	// test codes
public:
	void testTimeUtilAll();
	void testAddSYSTEMTIME();
	void testSubstractSYSTEMTIME();
	void testIsLessEqualSYSTEMTIME();
#endif	/* end of NDEBUG */
};

// test codes
#ifndef  NDEBUG
	inline void TimeUtil::testTimeUtilAll() {
		testAddSYSTEMTIME();
		testSubstractSYSTEMTIME();
		testIsLessEqualSYSTEMTIME();
	}
	inline void TimeUtil::testAddSYSTEMTIME() {
		SYSTEMTIME st1, st2;
		st1 = getCurrentLocalTime();
		st2 = addSYSTEMTIME(&st1, 24*60*60);	// 하루(86400초)를 더한다

		std::cout << getDateStringFromSYSTEMTIMEA(&st1) << " " << getTimeStringFromSYSTEMTIMEA(&st1) << std::endl;
		std::cout << getDateStringFromSYSTEMTIMEA(&st2) << " " << getTimeStringFromSYSTEMTIMEA(&st2) << std::endl;
	}
	inline void TimeUtil::testSubstractSYSTEMTIME() {
		SYSTEMTIME st1, st2;
		st1 = getCurrentLocalTime();
		st2 = substractSYSTEMTIME(&st1, 24*60*60);	// 하루(86400초)를 더한다

		std::cout << getDateStringFromSYSTEMTIMEA(&st1) << " " << getTimeStringFromSYSTEMTIMEA(&st1) << std::endl;
		std::cout << getDateStringFromSYSTEMTIMEA(&st2) << " " << getTimeStringFromSYSTEMTIMEA(&st2) << std::endl;
	}
	inline void TimeUtil::testIsLessEqualSYSTEMTIME() {
		SYSTEMTIME st1, st2;
		st2 = getCurrentLocalTime();	// st2 -> st1
		Sleep(1000);
		st1 = getCurrentLocalTime();	// st1 -> st2 도 테스트 해본다

		if(isLessEqualSYSTEMTIME(&st1, &st2)) std::cout << getTimeStringFromSYSTEMTIMEA(&st1) << " <= " << getTimeStringFromSYSTEMTIMEA(&st2) << std::endl;
		else std::cout << getTimeStringFromSYSTEMTIMEA(&st1) << " > " << getTimeStringFromSYSTEMTIMEA(&st2) << std::endl;
	}
#endif	/* end of NDEBUG */
} // namespace Util

/*
 * Copyright (c) 2009 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */
