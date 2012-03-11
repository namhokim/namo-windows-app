// 명시적 동적 링킹 예

#include <stdio.h>
#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>

// 함수포인터 타입
typedef int (__cdecl *MYPROC)(int, int);

int main()
{
	int a = 1;
	int b = 2;

	// DLL 명시적 로드 (MSDN - http://msdn.microsoft.com/en-us/library/ms684175(VS.85).aspx)
	HMODULE hModule = ::LoadLibrary(TEXT("DLL_source.dll"));
	if(NULL!=hModule) {
		MYPROC pMYPROC;
		// 함수의 주소를 구함
		pMYPROC = (MYPROC)::GetProcAddress(hModule, "Add");
		if(NULL!=pMYPROC) {
			// DLL에 export한 함수를 이용하여 결과를 구함
			int r = pMYPROC(a, b);
			// 결과를 출력
			printf("%d + %d = %d\n", a, b, r);

			MYPROC pMYPROC2;
			pMYPROC2 = (MYPROC)::GetProcAddress(hModule, "?Sub@@YAHHH@Z");
			printf("%d + %d = %d\n", a, b, pMYPROC2(a,b));

			// 라이브러리 해제
			::FreeLibrary(hModule);
		} else {
			printf("DLL에서 함수의 주소를 구할 수 없었습니다. %d\n", ::GetLastError());
		}
	} else {
		printf("DLL을 로드할 수 없었습니다. %d\n", ::GetLastError());
	}

	return 0;
}