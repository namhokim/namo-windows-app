// ����� ���� ��ŷ ��

#include <stdio.h>
#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>

// �Լ������� Ÿ��
typedef int (__cdecl *MYPROC)(int, int);

int main()
{
	int a = 1;
	int b = 2;

	// DLL ����� �ε� (MSDN - http://msdn.microsoft.com/en-us/library/ms684175(VS.85).aspx)
	HMODULE hModule = ::LoadLibrary(TEXT("DLL_source.dll"));
	if(NULL!=hModule) {
		MYPROC pMYPROC;
		// �Լ��� �ּҸ� ����
		pMYPROC = (MYPROC)::GetProcAddress(hModule, "Add");
		if(NULL!=pMYPROC) {
			// DLL�� export�� �Լ��� �̿��Ͽ� ����� ����
			int r = pMYPROC(a, b);
			// ����� ���
			printf("%d + %d = %d\n", a, b, r);

			MYPROC pMYPROC2;
			pMYPROC2 = (MYPROC)::GetProcAddress(hModule, "?Sub@@YAHHH@Z");
			printf("%d + %d = %d\n", a, b, pMYPROC2(a,b));

			// ���̺귯�� ����
			::FreeLibrary(hModule);
		} else {
			printf("DLL���� �Լ��� �ּҸ� ���� �� �������ϴ�. %d\n", ::GetLastError());
		}
	} else {
		printf("DLL�� �ε��� �� �������ϴ�. %d\n", ::GetLastError());
	}

	return 0;
}