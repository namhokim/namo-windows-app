// 묵시적 동적 링킹 예

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

	// DLL에 export한 함수를 이용하여 결과를 구함
	int r = ::Add(a, b);

	// 결과를 출력
	printf("%d + %d = %d\n", a, b, r);

	printf("%d - %d = %d\n", a, b, Sub(a,b));

	return 0;
}