#include <windows.h>
#include <stdio.h>

int main()
{
	LARGE_INTEGER st,ed,freq;
	memset(&st, 0, sizeof(LARGE_INTEGER));
	memset(&ed, 0, sizeof(LARGE_INTEGER));
	memset(&freq, 0, sizeof(LARGE_INTEGER));


	QueryPerformanceCounter(&st);
	// 1√  Ω¨±‚
	Sleep(1000);
	printf("QueryPerformanceCounter : %d\n", QueryPerformanceCounter(&ed));
	printf("QueryPerformanceFrequency : %d\n", QueryPerformanceFrequency(&freq));
	double diff = (double)ed.QuadPart-st.QuadPart;
	double fre  = (double)freq.QuadPart;
	printf("diff : %f\nfreq : %f\n, sec : %f", diff, fre, (diff/fre)*1000);
}