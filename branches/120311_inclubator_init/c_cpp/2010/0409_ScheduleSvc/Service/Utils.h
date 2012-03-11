namespace Time {
	inline void FillSystemTime(
		__out LPSYSTEMTIME lpSystemTime,
		WORD year,
		WORD month,
		WORD day,
		WORD hour,
		WORD minute,
		WORD second,
		WORD milliseconds)
	{
		lpSystemTime->wYear = year;
		lpSystemTime->wMonth = month;
		lpSystemTime->wDay = day;
		lpSystemTime->wHour = hour;
		lpSystemTime->wMinute = minute;
		lpSystemTime->wSecond = second;
		lpSystemTime->wMilliseconds = milliseconds;
		lpSystemTime->wDayOfWeek = 0;
	}

	inline DWORD ToMilliseconds(WORD hour, WORD minute, WORD secound, WORD milliseconds)
	{
		return (milliseconds + (secound*1000) + (minute*1000*60) + (hour*1000*60*60));
	}

	inline void GetLocalDueTime(
		__out PLARGE_INTEGER lpLargeInteger,
		__in CONST WORD hour,
		__in CONST WORD minute,
		__in CONST WORD second,
		__in CONST WORD milliseconds)
	{
		// 현재지역시간
		SYSTEMTIME stLocal;
		GetLocalTime(&stLocal);

		// 비교
		DWORD dwNow = ToMilliseconds(stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);
		DWORD dwDue = ToMilliseconds(hour, minute, second, milliseconds);
		BOOL bIsTomorow = (dwDue <= dwNow) ? TRUE : FALSE;

		// 원하는 지역 시간
		SYSTEMTIME st = stLocal;
		st.wHour = hour;
		st.wMinute = minute;
		st.wSecond = second;
		st.wMilliseconds = milliseconds;

		// 파일 시간
		FILETIME ftLocal;
		SystemTimeToFileTime(&st, &ftLocal);

		// 지역시간 -> UTC시간
		FILETIME ftUTC;
		LocalFileTimeToFileTime(&ftLocal, &ftUTC);

		// Big Integer
		lpLargeInteger->LowPart = ftUTC.dwLowDateTime;
		lpLargeInteger->HighPart = ftUTC.dwHighDateTime;

		if(bIsTomorow) {
			lpLargeInteger->QuadPart += (LONGLONG)10000000 * 6 * 60 * 60 * 1000;
		}
	}
}