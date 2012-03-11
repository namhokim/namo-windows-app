#include <Windows.h>

int main(int argc, char *argv[])
{
	if (argc<2) exit(0);	// 인자가 없을 경우 종료
	int i = atoi(argv[1]);	// 문자가 들어가 있는 경우는 0이 들어감

	if (i<0)	exit(0);	// 음수일 경우는 0으로 지정
	else		Sleep( (DWORD)i );

	return 0;
}