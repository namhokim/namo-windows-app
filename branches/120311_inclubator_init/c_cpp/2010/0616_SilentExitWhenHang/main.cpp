#include <Windows.h>
#include <stdio.h>

// ���� �Լ�
void foo();

// ���� ó�� ����
LONG WINAPI MyUnhandledExceptionFilter(
	__in  struct _EXCEPTION_POINTERS *ExceptionInfo
);


int main()
{
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);	// ���ο� ���� ���͸� ��ġ

	// �Լ� ����
	foo();
	return 0;
}

void foo()
{
	unsigned long loop = 20;
	while(loop>0) {		
		printf("%u> Some loop\n", loop);

		int i = 100 / (loop -10);	// 10�� �� ���� �߻�(������ 0 ����)

		Sleep(500);
		--loop;
	}
	return;
}

// ����ó�� ����
LONG WINAPI MyUnhandledExceptionFilter(__in  struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	
	fprintf(stderr, "Exception break out...%u\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
	return EXCEPTION_EXECUTE_HANDLER;
}