#define TRAY_NOTIFY	65123

#ifndef _WINDOWS_		// @@@ 수정(2008/02/14, NHKim) @@@ windows.h 추가 정의 방지
#include <windows.h>
#endif
void SetTray(HWND hWindowHanlde, HICON hIconHandle, int message);