#include <windows.h>

double GetHDTimer()
{
	LARGE_INTEGER now, freq;
	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&freq);
	return now.QuadPart / (double)freq.QuadPart;
}