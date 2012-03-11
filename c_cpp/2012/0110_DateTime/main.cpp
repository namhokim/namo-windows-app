#include <Windows.h>
#include <stdio.h>

int main()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	printf("%d-%02d-%02d %02d:%02d:%02d.%03d",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return 0;
}