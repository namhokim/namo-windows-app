//#include "Performance.h"
#include <Windows.h>
#include <cstdio>

int main()
{
	LARGE_INTEGER t1, t2, fr, diff;
	if( ::QueryPerformanceFrequency(&fr) != 0) {	// �ϵ��� ���ػ� ī���͸� ������ ��
		bool isSuccess = true;
		if( ::QueryPerformanceCounter(&t1) == 0 ) isSuccess = false;
		
		// � �۾�...
		Sleep(1000);
		
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
