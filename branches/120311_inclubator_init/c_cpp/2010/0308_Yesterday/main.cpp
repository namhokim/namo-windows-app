#include <Windows.h>
#include "StopWatch.h"
#include <iostream>

using namespace std;

void GetYesterdayTime(__out  LPSYSTEMTIME lpSystemTime);
void GetYesterdayTime2(__out  LPSYSTEMTIME lpSystemTime);
void GetYesterdayTime3(__out  LPSYSTEMTIME lpSystemTime);


int main()
{
	StopWatch sw_t, sw_y, sw_y2, sw_y3;

	SYSTEMTIME st_t, st_y, st_y2, st_y3;

	sw_t.Start();
	::GetLocalTime(&st_t);
	sw_t.Stop();

	sw_y.Start();
	GetYesterdayTime(&st_y);
	sw_y.Stop();

	sw_y2.Start();
	GetYesterdayTime2(&st_y2);
	sw_y2.Stop();

	sw_y3.Start();
	GetYesterdayTime3(&st_y3);
	sw_y3.Stop();

	cout << fixed << sw_t.GetDiff() * 1000000 << endl
		<< sw_y.GetDiff() *  1000000 << endl
		<< sw_y2.GetDiff() * 1000000 << endl
		<< sw_y3.GetDiff() * 1000000 << endl;

	return 0;
}

void GetYesterdayTime(__out  LPSYSTEMTIME lpSystemTime) {

	SYSTEMTIME st;
	::GetLocalTime(&st);
	FILETIME ft;
	::SystemTimeToFileTime(&st, &ft);
	ULARGE_INTEGER ui, ui_y;
	ui.HighPart = ft.dwHighDateTime;
	ui.LowPart = ft.dwLowDateTime;
	ui_y.QuadPart = ui.QuadPart - 864000000000;
	ft.dwHighDateTime = ui_y.HighPart;
	ft.dwLowDateTime  = ui_y.LowPart;
	::FileTimeToSystemTime(&ft, lpSystemTime);
}

void GetYesterdayTime2(__out  LPSYSTEMTIME lpSystemTime) {

	// 오늘 날짜를 구해(SYSTEMTIME)
	SYSTEMTIME st;
	::GetSystemTime(&st);

	// 현재의 대한민국 표준시의 어제 날짜를 구해
	TIME_ZONE_INFORMATION tz;
	::ZeroMemory(&tz, sizeof(tz));
	tz.Bias = 900;
//	::wcscpy_s(tz.StandardName, 32, L"어제의 대한민국 표준시");
//	::wcscpy_s(tz.DaylightName, 32, L"어제의 대한민국 표준시");
	::SystemTimeToTzSpecificLocalTime(&tz, &st, lpSystemTime);
}

void GetYesterdayTime3(__out  LPSYSTEMTIME lpSystemTime) {

	SYSTEMTIME st;
	::GetLocalTime(&st);
	FILETIME ft;
	::SystemTimeToFileTime(&st, &ft);
	UINT64 ui, ui_y;
	ui = (((UINT64)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
	ui_y = ui - 864000000000;
	ft.dwHighDateTime = (ui_y >> 32);
	ft.dwLowDateTime  = (ui_y & 0xffffffff);	// ft.dwLowDateTime  = (ui_y && 0xffffffff); 가 아니다!!!
	::FileTimeToSystemTime(&ft, lpSystemTime);
}
