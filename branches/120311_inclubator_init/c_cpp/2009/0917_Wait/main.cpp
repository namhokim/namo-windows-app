#include <Windows.h>

int main(int argc, char *argv[])
{
	if (argc<2) exit(0);	// ���ڰ� ���� ��� ����
	int i = atoi(argv[1]);	// ���ڰ� �� �ִ� ���� 0�� ��

	if (i<0)	exit(0);	// ������ ���� 0���� ����
	else		Sleep( (DWORD)i );

	return 0;
}