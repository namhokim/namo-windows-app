// ������ ���� ��ŷ ��

#include <stdio.h>

#include "../DLL_source/MyDll.h"
#ifdef	_DEBUG
#pragma comment(lib, "../Debug/DLL_source.lib")
#else
#pragma comment(lib, "../Release/DLL_source.lib")
#endif

int main()
{
	int a = 1;
	int b = 2;

	// DLL�� export�� �Լ��� �̿��Ͽ� ����� ����
	int r = ::Add(a, b);

	// ����� ���
	printf("%d + %d = %d\n", a, b, r);

	printf("%d - %d = %d\n", a, b, Sub(a,b));

	return 0;
}