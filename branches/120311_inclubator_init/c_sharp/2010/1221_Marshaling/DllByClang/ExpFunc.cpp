#include "ExpFuncCompile.h"
#include <Windows.h>
#include <string>

using namespace std;

void PrintDataTypes();

#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport)
BOOL
__stdcall							// WINAPI
GetSomeInfo(
	const char* szIP,
	LPSOME_INFO info,	// PRT_SYSTEM_INFO long pointer type
	unsigned int *dwSize
)
{
	printf("C : struct Address : %d\n", info);

	::OutputDebugString(TEXT("GetSomeInfo"));

	string ip(szIP);
	if(ip.empty()) {
		::OutputDebugString(TEXT("IP is empty"));
		return FALSE;
	} else {
		::OutputDebugStringA(szIP);
	}

	if( (*dwSize)==0) {
		PrintDataTypes();
		::OutputDebugString(TEXT("GetSomeInfo - Get Size"));
		*dwSize = 3;
	} else {
		::OutputDebugString(TEXT("GetSomeInfo - Get Data"));

		memset(info, 0, sizeof(SOME_INFO)*3);
		
		::OutputDebugString(TEXT("GetSomeInfo - Set data"));
		int i = 0;
		strcpy_s(info[i].szIPAddr, IP_ADDRESS_MAX, szIP);
		info[i].status = 123;
		wcscpy_s(info[i].szLocalizedName, IN_TRAY_LOCAL_NAME_MAX, L"유니코드1");
		++i;
		strcpy_s(info[i].szIPAddr, IP_ADDRESS_MAX, szIP);
		info[i].status = 12;
		wcscpy_s(info[i].szLocalizedName, IN_TRAY_LOCAL_NAME_MAX, L"유니코드12");
		++i;
		strcpy_s(info[i].szIPAddr, IP_ADDRESS_MAX, szIP);
		info[i].status = 1;
		wcscpy_s(info[i].szLocalizedName, IN_TRAY_LOCAL_NAME_MAX, L"유니코드123");
	}

	return TRUE;
}
#ifdef __cplusplus
}
#endif

void PrintDataTypes()
{
	printf("C : sizeof(SOME_INFO) : %d\n", sizeof(SOME_INFO));
}