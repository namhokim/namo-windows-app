#define TRAY_NOTIFY	65123

#ifndef _WINDOWS_		// @@@ ����(2008/02/14, NHKim) @@@ windows.h �߰� ���� ����
#include <windows.h>
#endif
void SetTray(HWND hWindowHanlde, HICON hIconHandle, int message);