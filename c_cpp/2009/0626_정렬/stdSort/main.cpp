#include <algorithm>
#include <Windows.h>

inline bool intGreater(int i1, int i2)
{
	return (i1 < i2);
}

int main(void) { 
	int sort[100], i; 

	for(i=0; i<100; i++) 
		sort[i] = rand(); 

	LARGE_INTEGER t1, t2, fr, diff;
	::QueryPerformanceFrequency(&fr);
	::QueryPerformanceCounter(&t1);
	std::sort(sort, sort+100, intGreater);
	::QueryPerformanceCounter(&t2);

	//for(i=0; i<100; i++) printf("%d\n", sort[i]); 

	diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
	double ㎲ = (double)diff.QuadPart / ((double)fr.QuadPart);
	fprintf(stdout, "수행시간 : %f ㎳\n", ㎲*1000);
	return 0; 
} 