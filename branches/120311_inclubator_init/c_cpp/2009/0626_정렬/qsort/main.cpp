#include <stdio.h> 
#include <stdlib.h> 
#include <windows.h>

int comp(const void *i, const void *j); 

int main(void) { 
	int sort[100], i; 

	for(i=0; i<100; i++) 
		sort[i] = rand(); 

	LARGE_INTEGER t1, t2, fr, diff;
	::QueryPerformanceFrequency(&fr);
	::QueryPerformanceCounter(&t1);
	qsort(sort, 100, sizeof(int), comp); 
	::QueryPerformanceCounter(&t2);

	//for(i=0; i<100; i++) printf("%d\n", sort[i]); 

	diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
	double ㎲ = (double)diff.QuadPart / ((double)fr.QuadPart);
	fprintf(stdout, "수행시간 : %f ㎳\n", ㎲*1000);
	return 0; 
} 

int comp(const void *i, const void *j) { 
	return *(int *)i - *(int *)j; 
}
