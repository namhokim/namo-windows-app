#define TRAY_NOTIFY		WM_USER+1	// ����� ���� �޽���

#ifndef _WINDOWS_		// @@@ ����(2008/02/14, NHKim) @@@ windows.h �߰� ���� ����
#include <windows.h>
#endif
void SetTray(HWND hWindowHanlde, HICON hIconHandle, int message, LPCTSTR tip=NULL);