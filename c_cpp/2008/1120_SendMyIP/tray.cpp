#include "tray.h"
#include <tchar.h>	// for _T
#include <windows.h>

// message : NIM_ADD / NIM_DELETE
void SetTray(HWND hWindowHandle, HICON hIconHandle, int message)
{
	NOTIFYICONDATA nid;

	nid.cbSize=sizeof(NOTIFYICONDATA);
	nid.hWnd = hWindowHandle;
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = TRAY_NOTIFY;
	if(hIconHandle) nid.hIcon = hIconHandle;
	wsprintf(nid.szTip, _T("%s"), _T("Click to Restore"));
	//strcpy_s(nid.szTip, 64, _T("Click to Restore"));
	Shell_NotifyIcon(message, &nid);
	switch(message) {
		case NIM_ADD:
//			ShowWindow(hWindowHandle, SW_HIDE);
			ShowWindow(hWindowHandle, SW_MINIMIZE);
			ShowWindow(hWindowHandle, SW_HIDE);
			break;
		case NIM_DELETE:
			ShowWindow(hWindowHandle, SW_SHOW);	
			ShowWindow(hWindowHandle, SW_RESTORE);
			break;
	}
}