#include <Windows.h>
#include <Strsafe.h>

#define DATE_LEN	120
#define TIME_LEN	120
#define DATETIME_LEN	250

int main()
{
	// Get the current date and hour.
	TCHAR date[DATE_LEN], time[TIME_LEN], date_time[DATETIME_LEN];
	GetDateFormat(GetThreadLocale(), 0, NULL, NULL, date, DATE_LEN);
	GetTimeFormat(GetThreadLocale(), 0, NULL, NULL, time, TIME_LEN);
	StringCchPrintf(date_time, DATETIME_LEN, TEXT("%s %s"), date, time);
	MessageBox(NULL, date_time, TEXT("날짜와 시간"), MB_OK);
	return 0;
}