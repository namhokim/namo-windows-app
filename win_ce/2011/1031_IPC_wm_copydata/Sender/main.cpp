#include <windows.h>
#include <stdio.h>

void main()
{
	HWND hwnd = FindWindow(0, TEXT("Receiver"));

	if(hwnd == 0)
	{
		printf("B�� ���� �����ϼ���\n");
		return ;

	}

	//------------------------------------------------------------

	while(1)
	{
		char buf[1024];
		scanf("%s", buf);

		// ���� ���μ����� �޸𸮸� ��ä�� �ٸ� ���μ�����
		// ������ �ʹٸ�!!

		COPYDATASTRUCT cds;
		cds.cbData = 1024;      // ���� �޸� ũ��
		cds.dwData = 1;         // �ĺ���
		cds.lpData = buf;

		// WM_COPYDATA�� ������ ���������� CDS�ȿ� �ִ�
		// �޸𸮸� ��ä��
		// ��� ���μ����� �������ش�.

		SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&cds);
	}
}
