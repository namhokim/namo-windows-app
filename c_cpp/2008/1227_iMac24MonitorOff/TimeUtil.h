#pragma once
#include <windows.h>

namespace namo {
class TimeUtil
{
public:
	TimeUtil(void) {}
	~TimeUtil(void) {}
	INT64 FILETIMEtoINT64(const LPFILETIME filetime)
	{
		INT64 _i64 = 0;
		_i64 = filetime->dwHighDateTime;
		_i64 = _i64 << 32;
		_i64 = _i64 | filetime->dwLowDateTime;
		return _i64;
	}

	FILETIME INT64toFILETIME(const PINT64 int64)
	{
		FILETIME filetime;
		filetime.dwHighDateTime = (DWORD)(*int64 >> 32);
		filetime.dwLowDateTime  = (DWORD)(*int64 & 0xffffffff);
		return filetime;
	}

	SYSTEMTIME addCurrentLocalTimeBySYSTEMTIME(int addMilliseconds)
	{
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);

		FILETIME currentFileTime, addedFileTime;
		SystemTimeToFileTime(&currentTime, &currentFileTime);

		INT64 addedTime = FILETIMEtoINT64(&currentFileTime) + (addMilliseconds * 10000);
		addedFileTime = INT64toFILETIME(&addedTime);

		SYSTEMTIME st;
		FileTimeToSystemTime(&addedFileTime, &st);

		return st;
	}
	FILETIME addCurrentLocalTimeByFILETIME(int seconds)
	{
		SYSTEMTIME currentTimeST;
		GetLocalTime(&currentTimeST);
		FILETIME currentTimeFT;
		SystemTimeToFileTime(&currentTimeST, &currentTimeFT);

		INT64 currentTime = FILETIMEtoINT64(&currentTimeFT);
		INT64 addValue = seconds * 10000000;
		INT64 resultValue = currentTime + addValue;

		return INT64toFILETIME(&resultValue);
	}
	bool lessEqual_FILETIME(const LPFILETIME time1, const LPFILETIME time2)
	{
		return (
			(time1->dwHighDateTime <= time2->dwHighDateTime)
			&& (time1->dwLowDateTime <= time2->dwLowDateTime));
	}
};
} // namespace namo