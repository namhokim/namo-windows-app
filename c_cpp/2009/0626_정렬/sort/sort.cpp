// sort.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	if( ::QueryPerformanceFrequency(&fr) != 0) { // �ϵ��� ���ػ� ī���͸� ������ ��
		bool isSuccess = true;
		if( ::QueryPerformanceCounter(&t1) == 0 ) isSuccess = false;

		quickSortInC();
		
		if( ::QueryPerformanceCounter(&t2) == 0 ) isSuccess = false;
		if ( isSuccess ) {
			diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
			double �� = (double)diff.QuadPart / ((double)fr.QuadPart);
			fprintf(stdout, "����ð� : %f ��\n", ��*1000);
		}
		else {
			fprintf(stderr, "����ð� ������ ����(%d)\n", ::GetLastError());
		}
	}
	else {
		fprintf(stderr, "�ϵ��� ���ػ� ī���͸� �������� �ʽ��ϴ�.\n");
	}
	
	if( ::QueryPerformanceFrequency(&fr) != 0) { // �ϵ��� ���ػ� ī���͸� ������ ��
		bool isSuccess = true;
		if( ::QueryPerformanceCounter(&t1) == 0 ) isSuccess = false;

		stdSortInCpp();
		
		if( ::QueryPerformanceCounter(&t2) == 0 ) isSuccess = false;
		if ( isSuccess ) {
			diff.QuadPart = (LONGLONG)t2.QuadPart - t1.QuadPart;
			double �� = (double)diff.QuadPart / ((double)fr.QuadPart);
			fprintf(stdout, "����ð� : %f ��\n", ��*1000);
		}
		else {
			fprintf(stderr, "����ð� ������ ����(%d)\n", ::GetLastError());
		}
	}
	else {
		fprintf(stderr, "�ϵ��� ���ػ� ī���͸� �������� �ʽ��ϴ�.\n");
	}

	return 0;
}

