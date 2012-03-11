#include <Windows.h>
#include <stdio.h>

// 임의 함수
void foo();

// 예외 처리 필터
LONG WINAPI MyUnhandledExceptionFilter(
	__in  struct _EXCEPTION_POINTERS *ExceptionInfo
);


int main()
{
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);	// 새로운 예외 필터를 설치

	// 함수 수행
	foo();
	return 0;
}

void foo()
{
	unsigned long loop = 20;
	while(loop>0) {		
		printf("%u> Some loop\n", loop);

		int i = 100 / (loop -10);	// 10일 때 에러 발생(나누기 0 에러)

		Sleep(500);
		--loop;
	}
	return;
}

// 예외처리 필터
LONG WINAPI MyUnhandledExceptionFilter(__in  struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	
	fprintf(stderr, "Exception break out...%u\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
	return EXCEPTION_EXECUTE_HANDLER;
}