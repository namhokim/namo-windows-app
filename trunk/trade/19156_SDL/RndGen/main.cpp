// RndGen.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "RndGen.h"

// 1 ~ to
//void GenDoubleRndNum(int to, int num, int* out)
//{
//	srand(GetTickCount());
//
//	for ( int i=0; i<num; i++) {
//		out[i] = (rand() % to);
//	}
//}

int _tmain(int argc, _TCHAR* argv[])
{
	const int Max = 25;
	const int Size = 12;
	int gen[Size] = {0};

	RndGen g(Max, Size);
	g.generate();

	for ( int i=0; i<Size; i++) {
		gen[i] = g.next();
	}

	//GenDoubleRndNum(Max, Size, gen);

	for ( int i=0; i<Size; i++) {
		printf("%d\n", gen[i]);
	}

	return 0;
}

