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
		// ���������ð�
		SYSTEMTIME stLocal;
		GetLocalTime(&stLocal);

		// ��
		DWORD dwNow = ToMilliseconds(stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);
		DWORD dwDue = ToMilliseconds(hour, minute, second, milliseconds);
		BOOL bIsTomorow = (dwDue <= dwNow) ? TRUE : FALSE;

		// ���ϴ� ���� �ð�
		SYSTEMTIME st = stLocal;
		st.wHour = hour;
		st.wMinute = minute;
		st.wSecond = second;
		st.wMilliseconds = milliseconds;

		// ���� �ð�
		FILETIME ftLocal;
		SystemTimeToFileTime(&st, &ftLocal);

		// �����ð� -> UTC�ð�
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