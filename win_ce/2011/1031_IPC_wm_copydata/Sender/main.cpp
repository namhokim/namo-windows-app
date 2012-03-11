#include <windows.h>
#include <stdio.h>

void main()
{
	HWND hwnd = FindWindow(0, TEXT("Receiver"));

	if(hwnd == 0)
	{
		printf("B를 먼저 실행하세요\n");
		return ;

	}

	//------------------------------------------------------------

	while(1)
	{
		char buf[1024];
		scanf("%s", buf);

		// 현재 프로세스의 메모리를 통채로 다른 프로세스로
		// 보내고 싶다면!!

		COPYDATASTRUCT cds;
		cds.cbData = 1024;      // 보낼 메모리 크기
		cds.dwData = 1;         // 식별자
		cds.lpData = buf;

		// WM_COPYDATA를 보내면 내부적으로 CDS안에 있는
		// 메모리를 통채로
		// 상대 프로세스에 전달해준다.

		SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&cds);
	}
}
