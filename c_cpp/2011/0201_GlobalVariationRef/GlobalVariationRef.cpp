// GlobalVariationRef.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

void foo();

int _tmain(int argc, _TCHAR* argv[])
{
	printf("bc::receiptCost::copyColor : %d\n", bc::receiptCost::copyColor);
	foo();
	bc::receiptCost::copyColor = 22;
	printf("bc::receiptCost::copyColor : %d\n", bc::receiptCost::copyColor);
	foo();
	return 0;
}

void foo()
{
	int &copyColorCost = bc::receiptCost::copyColor;
	printf("&copyColorCost : %d\n", copyColorCost);
	copyColorCost = 11;
	printf("&copyColorCost<=11 : %d\n", copyColorCost);
}