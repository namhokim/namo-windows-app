// sort.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <algorithm>
#include <iostream>
using namespace std;

const unsigned int TestSetNumber = 100000;

void makeTestSet(int *pIntArray)
{
	int data = TestSetNumber;
	for(int unsigned i = 0; i < TestSetNumber; ++i) {
		//if(data == 5000) data = 0;
		pIntArray[i] = (data--);
	}
}

inline int numcmp(const void *p1, const void *p2)
{
	int v1, v2;
	
	v1 = *(int *) p1;
	v2 = *(int *) p1;
	
	if (v1 < v2)
		return -1;
	else if(v1 == v2)
		return 0;
	else
		return 1;
}

inline void quickSortInC()
{
	int ia[TestSetNumber];		// integer array
	makeTestSet(ia);
	//std::cout << "ia[0] : " << ia[0] << std::endl;
	qsort(ia, TestSetNumber, sizeof(ia[0]), numcmp);
	//std::cout << "ia[0] : " << ia[0] << std::endl;
}
inline bool intGreater(int i1, int i2)
{
	return (i1 > i2);
}

inline void stdSortInCpp()
{
	int ia[TestSetNumber];		// integer array
	makeTestSet(ia);
	//std::cout << "ia[0] : " << ia[0] << std::endl;
	std::sort(&ia[0], &ia[TestSetNumber], intGreater);
	//std::cout << "ia[0] : " << ia[0] << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	LARGE_INTEGER t1, t2, fr, diff;

	if( ::QueryPerformanceFrequency(&fr) != 0) { // 하드웨어가 고해상도 카운터를 지원할 때
		bool isSuccess = true;
		if( ::QueryPerformanceCounter(&t1) == 0 ) isSuccess = false;

		quickSortInC();
		
		if( ::QueryPerformanceCounter(&t2) == 0 ) isSuccess = false;
		if ( isSuccess ) {
			diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
			double ㎲ = (double)diff.QuadPart / ((double)fr.QuadPart);
			fprintf(stdout, "수행시간 : %f ㎳\n", ㎲*1000);
		}
		else {
			fprintf(stderr, "수행시간 측정에 에러(%d)\n", ::GetLastError());
		}
	}
	else {
		fprintf(stderr, "하드웨어가 고해상도 카운터를 지원하지 않습니다.\n");
	}
	
	if( ::QueryPerformanceFrequency(&fr) != 0) { // 하드웨어가 고해상도 카운터를 지원할 때
		bool isSuccess = true;
		if( ::QueryPerformanceCounter(&t1) == 0 ) isSuccess = false;

		stdSortInCpp();
		
		if( ::QueryPerformanceCounter(&t2) == 0 ) isSuccess = false;
		if ( isSuccess ) {
			diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
			double ㎲ = (double)diff.QuadPart / ((double)fr.QuadPart);
			fprintf(stdout, "수행시간 : %f ㎳\n", ㎲*1000);
		}
		else {
			fprintf(stderr, "수행시간 측정에 에러(%d)\n", ::GetLastError());
		}
	}
	else {
		fprintf(stderr, "하드웨어가 고해상도 카운터를 지원하지 않습니다.\n");
	}

	return 0;
}

