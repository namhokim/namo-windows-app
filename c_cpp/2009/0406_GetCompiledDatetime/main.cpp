#include <Windows.h>
#include <stdio.h>

inline SYSTEMTIME GetCompiledDatetime()
{
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));

	// date
	const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char date[] = __DATE__;

	st.wYear = atoi(&date[7]);
	date[6] = '\0';
	st.wDay = atoi(&date[4]);
	date[3] = '\0';
	for (unsigned int i = 0; i < 12; i++) {
		if (!strcmp(date, months[i])) {
			st.wMonth = i + 1;
			break;
		}
	}

	// time
	char time[] = __TIME__;
	st.wSecond = atoi(&time[6]);
	time[5] = '\0';
	st.wMinute = atoi(&time[3]);
	time[2] = '\0';
	st.wHour = atoi(&time[0]);

	return st;
}

int main()
{
	SYSTEMTIME st = GetCompiledDatetime();

	printf("%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);
}