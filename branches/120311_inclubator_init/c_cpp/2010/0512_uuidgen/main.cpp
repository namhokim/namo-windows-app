#include "uuidgen.h"
#include <tchar.h>
#include <stdio.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	// ����Ʈ ����Ʈ���̷� �Ҵ�� MAC �ּҸ� ������� �� UUID�� ����
	UUID uuid;
	DWORD dwErrUUID = UuidGenWithMac(&uuid);

	// ���ڿ��� ��ȯ(�ڵ� �Ҵ�, RpcStringFree�ʿ�)
	RPC_WSTR pRpcWstr;
	DWORD dwErrString = UuidToString(&uuid, &pRpcWstr);

	// �޽��� ���
	TCHAR msg[1024] = {0,};
	wsprintf(msg, TEXT("%s (%u/%u)\n"), pRpcWstr, dwErrUUID, dwErrString);
	MessageBox(NULL, msg, TEXT("����"), MB_OK);
	// �Ҵ� ����
	RpcStringFree(&pRpcWstr);

	return 0;
}
