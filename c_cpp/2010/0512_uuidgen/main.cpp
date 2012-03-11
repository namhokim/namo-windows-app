#include "uuidgen.h"
#include <tchar.h>
#include <stdio.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	// 디폴트 게이트웨이로 할당된 MAC 주소를 기반으로 한 UUID를 생성
	UUID uuid;
	DWORD dwErrUUID = UuidGenWithMac(&uuid);

	// 문자열로 변환(자동 할당, RpcStringFree필요)
	RPC_WSTR pRpcWstr;
	DWORD dwErrString = UuidToString(&uuid, &pRpcWstr);

	// 메시지 출력
	TCHAR msg[1024] = {0,};
	wsprintf(msg, TEXT("%s (%u/%u)\n"), pRpcWstr, dwErrUUID, dwErrString);
	MessageBox(NULL, msg, TEXT("정보"), MB_OK);
	// 할당 해제
	RpcStringFree(&pRpcWstr);

	return 0;
}
