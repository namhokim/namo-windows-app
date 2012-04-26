#define TRAY_NOTIFY		WM_USER+1	// 사용자 정의 메시지

#ifndef _WINDOWS_		// @@@ 수정(2008/02/14, NHKim) @@@ windows.h 추가 정의 방지
#include <windows.h>
#endif
void SetTray(HWND hWindowHanlde, HICON hIconHandle, int message, LPCTSTR tip=NULL);